/// Copyright [year] <Copyright Owner>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <assert.h>
#include "js/src/jsscript.h"
#include "js/src/jsopcode.h"
#include "js/src/jsfun.h"
#include "js/src/jsatom.h"
#include "js/src/jscntxt.h"
#include "js/src/jsatominlines.h"
#include "js/src/vm/ScopeObject.h"
#include "../include/compiler.h"

const mapleir::OpcodeTable mapleir::opcodeinfo;

namespace mapleir {

enum js_builtin_string_id{
#define JSBUILTIN_STRING_DEF(id, length, str)  id,
#include "../include/jsbuiltinstrings.inc.h"
#undef JSBUILTIN_STRING_DEF
    JSBUILTIN_STRING_ID_COUNT
};

void JSCompiler::Init() {
  jsmain_ = jsbuilder_->jsmain_;
  jsvalue_type_ = jsbuilder_->jsvalue_type_;
  jsvalue_ptr_ = jsbuilder_->jsvalue_ptr_;

  // push main() on funcstack_
  char *name = "main";
  if (jsbuilder_->IsPlugin())
     name = jsbuilder_->GetWrapperName();
  scope_->GetOrCreateSN(name)->SetFunc(jsmain_);
  funcstack_.push(jsmain_);
  jsbuilder_->SetCurrentFunction(jsmain_);
  DEBUGPRINT2(jsmain_);

  funcFormals = closure_->funcFormals;

  dummyNode = CompileOpConstValue(JSTYPE_UNDEFINED, 0);
}

void JSCompiler::Finish() {
  if (jsbuilder_->WithMain())
    module_->AddFunction(jsmain_);  // add jsmain_ in the end

  // more forgiving about stack integrety
  int expected = scope_->GetDepth();
  DEBUGPRINT2(opstack_->GetDepth());
  DEBUGPRINT2(expected);
  assert(opstack_->CheckDepth(0) || opstack_->CheckDepth(expected));
}

void JSCompiler::SetupMainFuncRet(base_node_t *rval){
  jsbuilder_->CreateStmtReturn(rval, false, linenum_);
}

static bool IsCCall(const char *name) {
  return (strcmp(name, "ccall") == 0);
}

static bool IsXcCall(const char *name) {
  return (strcmp(name, "xccall") == 0);
}

MIRSymbol *JSCompiler::CreateTempVar(MIRType *type) {
  const char *name = Util::GetSequentialName("temp_var_", temp_var_no_, mp_);
  MIRSymbol *var = jsbuilder_->GetOrCreateLocalDecl(name, type);
  return var;
}

MIRSymbol *JSCompiler::CreateTempJSValueTypeVar() {
  return CreateTempVar(jsvalue_type_);
}

void JSCompiler::InitWithUndefined(bool doit, MIRSymbol *var) {
#if 0
  if (doit) {
    BaseNode *undefined = CompileOpConstValue(JSTYPE_UNDEFINED, 0);
    BaseNode *stmt = jsbuilder_->CreateStmtDassign(var, 0, undefined);
  }
#endif
}

uint32_t JSCompiler::GetFieldidFromTag(uint32_t tag) {
  char *tagname;
  switch (tag) {
  case JSTYPE_BOOLEAN: tagname = "boo"; break;
  case JSTYPE_STRING: tagname = "str"; break;
  case JSTYPE_OBJECT: tagname = "prop_list"; break;
  default: tagname = "i32"; break;
  }
  return jsbuilder_->GetStructFieldIdFromFieldName(jsvalue_type_, tagname);
}

MIRType *JSCompiler::DetermineTypeFromNode(base_node_t *node) {
  tyidx_t tyidx;
  if (opcodeinfo.IsCompare(node->op))
    return globaltable.GetTypeFromTyIdx((tyidx_t)PTY_u1);
  if (node->op == OP_intrinsicop) {
    // TODO: look up intrinsic table
    MIRIntrinsicId intrnid = static_cast<IntrinsicopNode *>(node)->intrinsic;
    IntrinDesc *intrndesc = &IntrinDesc::intrintable[intrnid];
    return intrndesc->GetTypeFromArgTy(intrndesc->argtypes_[0]);
  }
  if (node->op == OP_dread) {
    AddrofNode *dread = static_cast<AddrofNode *>(node);
    MIRSymbol *st = module_->CurFunction()->GetLocalOrGlobalSymbol(dread->stidx);
    tyidx = st->GetTyIdx();
  } else if (node->op == OP_iread) {
    IreadNode *iread = static_cast<IreadNode *>(node);
    tyidx = iread->tyidx;
  } else {
    tyidx = node->ptyp;
  }
  if (tyidx == jsvalue_type_->_ty_idx)
    return jsvalue_type_;

  return globaltable.GetTypeFromTyIdx(tyidx);
}

// create a new temporary, store expr to the temporary and return the temporary
MIRSymbol *JSCompiler::SymbolFromSavingInATemp(base_node_t *expr, bool jsvalue_p) {
  MIRType *exprty;
  if (jsvalue_p) {
    exprty = jsvalue_type_;
    expr = CheckConvertToJSValueType(expr);
  }
  else {
    exprty = jsbuilder_->GetPrimType(expr->ptyp);
  }
  MIRSymbol *temp_var = CreateTempVar(exprty);
  jsbuilder_->CreateStmtDassign(temp_var, 0, expr, linenum_); 
  return temp_var;
}

// create a new temporary, store expr to the temporary and return a dread node 
// of the new temporary
AddrofNode *JSCompiler::NodeFromSavingInATemp(base_node_t *expr) {
  MIRSymbol *temp_var = SymbolFromSavingInATemp(expr, false);
  return jsbuilder_->CreateExprDread(temp_var->GetType(&globaltable), temp_var);
}

// JSOP_UNDEFINED 1
// JSOP_ZERO 62
// JSOP_ONE 63
// JSOP_UINT16 88
// JSOP_UINT24 188
// JSOP_INT8 215
// JSOP_INT32 216
BaseNode *JSCompiler::CompileOpConstValue(uint32_t jsvalue_tag, int32_t payload) {
  PrimType pty;
  switch (jsvalue_tag) {
   case JSTYPE_NUMBER:
     return jsbuilder_->CreateIntConst((uint64_t)(uint32_t)payload, PTY_i32);
   case JSTYPE_UNDEFINED:
     pty = PTY_dynundef; break;
   case JSTYPE_NULL:
     pty = PTY_dynnull; break;
   case JSTYPE_BOOLEAN:
     pty = PTY_dynbool; break;
   case JSTYPE_NONE:
     pty = PTY_dynnone; break;
   default:
     assert(false && "NIY");
     break;
  }
  int64_t val = (int64_t)((uint64_t)(uint32_t)jsvalue_tag << 32 | (uint64_t)(uint32_t)payload);
  return jsbuilder_->CreateIntConst(val, pty);
}

// Return the corresponding intrinsic code for JSop binary or unary opcode.
uint32_t JSCompiler::FindIntrinsicForOp(JSOp opcode) {
  static const uint32_t op_to_intrinsic[][2] = {
    {JSOP_STRICTEQ, INTRN_JSOP_STRICTEQ},
    {JSOP_STRICTNE, INTRN_JSOP_STRICTNE},
    {JSOP_INSTANCEOF, INTRN_JSOP_INSTANCEOF},
    {JSOP_IN, INTRN_JSOP_IN},
    {JSOP_POS, INTRN_JS_NUMBER},
    {JSOP_OR, INTRN_JSOP_OR},
    {JSOP_AND, INTRN_JSOP_AND},
    {JSOP_TYPEOF, INTRN_JSOP_TYPEOF}};
  int32_t intrinsic_code = -1;
  for (uint32_t i = 0; i < sizeof (op_to_intrinsic) / 8; i++) {
    if (op_to_intrinsic[i][0] == opcode) {
      intrinsic_code = op_to_intrinsic[i][1];
      break;
    }
  }
  assert(intrinsic_code >= 0);
  return (uint32_t)intrinsic_code;
}

// JSOP_BITOR JSOP_BITXOR JSOP_BITAND JSOP_EQ JSOP_NE JSOP_LT JSOP_GT
// JSOP_GE JSOP_LSH JSOP_RSH JSOP_URSH JSOP_ADD JSOP_SUB JSOP_MUL JSOP_DIV
// JSOP_MOD 15 ~ 31
// JSOP_STRICTEQ 72
// JSOP_STRICTNE 73
base_node_t *JSCompiler::CompileOpBinary(JSOp opcode,
                                         base_node_t *op0,
                                         base_node_t *op1) {
  Opcode mop = (Opcode)0;
  MIRType *restype = jsbuilder_->GetDynany();
  switch (opcode) {
    case JSOP_BITOR: mop = OP_bior; restype = jsbuilder_->GetInt32(); break;
    case JSOP_BITXOR: mop = OP_bxor; restype = jsbuilder_->GetInt32(); break;
    case JSOP_BITAND: mop = OP_band; restype = jsbuilder_->GetInt32(); break;
    case JSOP_EQ: mop = OP_eq; restype = jsbuilder_->GetUInt1(); break;
    case JSOP_NE: mop = OP_ne; restype = jsbuilder_->GetUInt1(); break;
    case JSOP_LT: mop = OP_lt; restype = jsbuilder_->GetUInt1(); break;
    case JSOP_LE: mop = OP_le; restype = jsbuilder_->GetUInt1(); break;
    case JSOP_GT: mop = OP_gt; restype = jsbuilder_->GetUInt1(); break;
    case JSOP_GE: mop = OP_ge; restype = jsbuilder_->GetUInt1(); break;
    case JSOP_LSH: mop = OP_shl; restype = jsbuilder_->GetInt32(); break;
    case JSOP_RSH: mop = OP_ashr; restype = jsbuilder_->GetInt32(); break;
    case JSOP_URSH: mop = OP_lshr; restype = jsbuilder_->GetUInt32(); break;
    case JSOP_SUB: mop = OP_sub; restype = jsbuilder_->GetInt32(); break;
    case JSOP_MUL: mop = OP_mul; restype = jsbuilder_->GetInt32(); break;
    case JSOP_DIV: mop = OP_div; restype = jsbuilder_->GetInt32(); break;
    case JSOP_MOD: mop = OP_rem; restype = jsbuilder_->GetInt32(); break;
    case JSOP_ADD:
      return CompileGeneric2(INTRN_JSOP_ADD, CheckConvertToJSValueType(op0),
               CheckConvertToJSValueType(op1), true);
    default: break;
  }
  if (mop != 0) {
    if (op0->ptyp == PTY_i32 && op1->ptyp == PTY_i32) {
      if (opcodeinfo.IsCompare(mop))
        return jsbuilder_->CreateExprCompare(mop, restype, jsbuilder_->GetInt32(), op0, op1);
      return jsbuilder_->CreateExprBinary(mop, jsbuilder_->GetInt32(), op0, op1);
    }

    if (opcodeinfo.IsCompare(mop)) {
      return jsbuilder_->CreateExprCompare(mop, restype, jsbuilder_->GetDynany(),
           CheckConvertToJSValueType(op0), CheckConvertToJSValueType(op1)); }
    else if (restype->GetPrimType() == PTY_u32) {
      return jsbuilder_->CreateExprBinary(mop, restype,
           CheckConvertToUInt32(op0), CheckConvertToUInt32(op1));
    }
    else if (restype->GetPrimType() == PTY_dynany) {
      return jsbuilder_->CreateExprBinary(mop, restype,
           CheckConvertToJSValueType(op0), CheckConvertToJSValueType(op1));
    } else {
      assert(restype->GetPrimType() == PTY_i32);
      return jsbuilder_->CreateExprBinary(mop, restype,
           CheckConvertToInt32(op0), CheckConvertToInt32(op1));
    }
  }

  MIRIntrinsicId idx = (MIRIntrinsicId)FindIntrinsicForOp(opcode);
  IntrinDesc *intrindesc = &IntrinDesc::intrintable[idx];
  MIRType *retty = intrindesc->GetReturnType();
  return jsbuilder_->CreateExprIntrinsicop2(idx, retty, 
        CheckConvertToJSValueType(op0), CheckConvertToJSValueType(op1));
}

// JSOP_NOT JSOP_BITNOT JSOP_NEG JSOP_POS 32~35
base_node_t *JSCompiler::CompileOpUnary(JSOp opcode, base_node_t *val) {
  Opcode mop = (Opcode)0;
  MIRType *restype = jsbuilder_->GetDynany();
  switch (opcode) {
    case JSOP_NOT: mop = OP_lnot; restype = jsbuilder_->GetUInt1(); val = CheckConvertToBoolean(val); break;
    case JSOP_BITNOT: mop = OP_bnot; restype = jsbuilder_->GetInt32(); val = CheckConvertToInt32(val); break;
    case JSOP_NEG: mop = OP_neg; restype = jsbuilder_->GetInt32(); val = CheckConvertToInt32(val); break;
    default: break;
  }
  if (mop != 0)
    return jsbuilder_->CreateExprUnary(mop, restype, val);

  if (opcode == JSOP_POS) {
    PrimType pty;
    if (val->op == OP_constval)
      pty = val->ptyp;
    if (val->op == OP_dread) {
      AddrofNode *node = static_cast<AddrofNode *>(val);
      MIRSymbol *st = module_->CurFunction()->GetLocalOrGlobalSymbol(node->stidx);
      MIRType *type = st->GetType(&globaltable);
      pty = type->GetPrimType();
    }

    if (pty == PTY_dyni32 || IsPrimitiveInteger (pty))
      return val;
  }

  MIRIntrinsicId idx = (MIRIntrinsicId)FindIntrinsicForOp(opcode);
  IntrinDesc *intrindesc = &IntrinDesc::intrintable[idx];
  MIRType *retty = intrindesc->GetReturnType();
  return jsbuilder_->CreateExprIntrinsicop1(idx, retty, CheckConvertToJSValueType(val));
}

int32_t JSCompiler::GetBuiltinMethod(uint32_t argc, bool *need_this) {
  base_node_t *bn = GetOpAt(argc + 1);
  if (bn->op == OP_intrinsicop) {
    IntrinsicopNode *ion = static_cast<IntrinsicopNode *> (bn);
    if (ion->intrinsic == INTRN_JS_GET_BIOBJECT) {
      ConstvalNode *cval = static_cast<ConstvalNode *>(ion->Opnd(0));
      MIRIntConst *intconst = static_cast<MIRIntConst *>(cval->constval);
      switch (intconst->value_) {
        case JS_BUILTIN_OBJECT:
          if (argc == 0)
            return INTRN_JS_NEW_OBJECT_0;
          else
            return INTRN_JS_NEW_OBJECT_1;
        case JS_BUILTIN_STRING:
          return INTRN_JS_STRING;
        case JS_BUILTIN_BOOLEAN:
          return INTRN_JS_BOOLEAN;
          break;
        case JS_BUILTIN_NUMBER:
          return INTRN_JS_NUMBER;
          break;
        case JS_BUILTIN_ARRAY:
          if(argc == 1)
            return INTRN_JS_NEW_ARR_LENGTH;
          else
            return INTRN_JS_NEW_ARR_ELEMS;
          break;
        default:
          break;
      }
    }
  }

  if (bn->op != OP_dread)
    return -1;
  AddrofNode *drn = static_cast<AddrofNode *> (bn);
  assert (drn);
  // MIRSymbol *var = module_->symtab->GetSymbolFromStidx(drn->stidx.Idx());
  MIRSymbol *var = module_->CurFunction()->GetLocalOrGlobalSymbol(drn->stidx);
  std::string name = var->GetName();
  DEBUGPRINT3(name);

#define DEFBUILTINMETHOD(name, intrn_code, need_this)  \
{#name, intrn_code, need_this},
  struct builtin_method_map {
    const char *name;
    int32_t intrn_code;
    bool need_this;
  };
  builtin_method_map map[18] = {
    #include "../include/builtinmethod.def"
  };
#undef DEFBUILTINMETHOD
  for (uint32_t i = 0; i < sizeof (map) / sizeof (builtin_method_map); i++) {
    if (!map[i].name) break;
    if (!strcmp(name.c_str(), map[i].name)) {
      *need_this = map[i].need_this;
      return (int32_t) map[i].intrn_code;
    }
  }
  return -1;
}

base_node_t *JSCompiler::CompileBuiltinMethod(int32_t idx, int arg_num, bool need_this) {
  // Reverse the order of args.
  std::stack<base_node_t *> tmp_stack;
  if (arg_num == 0 && (MIRIntrinsicId)idx == INTRN_JS_BOOLEAN) {
    Pop();
    Pop();
    return CompileOpConstValue(JSTYPE_BOOLEAN, 0);
  }

  for (uint32_t i = 0; i < arg_num; i++) {
    tmp_stack.push(Pop());
  }
  if (need_this) {
    tmp_stack.push(Pop());
    arg_num += 1;
  } else {
      Pop();
  }
  Pop();

  if((MIRIntrinsicId)idx == INTRN_JS_NEW_ARR_ELEMS) {
    MapleVector<base_node_t *> args(module_->CurFuncCodeMpAllocator()->Adapter());
    MIRSymbol *arguments = NULL;
    arguments = jsbuilder_->GetCurrentFunction()->symtab->CreateSymbol(SCOPE_LOCAL);
    const char *temp_name = Util::GetSequentialName("js_arguments_", temp_var_no_, mp_);
    std::string argname(temp_name);
    arguments->SetNameStridx(globaltable.GetOrCreateGstridxFromName(argname));
    jsbuilder_->GetCurrentFunction()->symtab->AddToStringSymbolMap(arguments);
    arguments->sclass = SC_auto;
    arguments->skind  = ST_var;

    uint32_t size_array[1];
    size_array[0] = arg_num;
    MIRType *array_type = jsbuilder_->GetOrCreateArrayType(jsvalue_type_, 1, size_array);
    MIRType *array_ptr_type = jsbuilder_->GetOrCreatePointerType(array_type);
    tyidx_t tyidx = array_type->_ty_idx;
    arguments->SetTyIdx(tyidx);
    base_node_t *bn;
    MIRType *pargtype = jsbuilder_->GetOrCreatePointerType(arguments->GetType(&globaltable));
    base_node_t *addr_base = jsbuilder_->CreateExprAddrof(0, arguments);

    for (uint32_t i = 0; i < arg_num; i++) {
      bn = CheckConvertToJSValueType(tmp_stack.top());
      DEBUGPRINT3(bn->op);
      tmp_stack.pop();
      MapleVector<base_node_t *> opnds(module_->CurFuncCodeMpAllocator()->Adapter());
      opnds.push_back(addr_base);
      BaseNode *addr_offset = jsbuilder_->GetConstInt(i);
      opnds.push_back(addr_offset);
      BaseNode *array_expr = jsbuilder_->CreateExprArray(array_type, opnds);
      StmtNode *stmt = jsbuilder_->CreateStmtIassign(array_ptr_type, 0, array_expr, bn);
      stmt->srcpos.SetLinenum(linenum_);
      jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
    }

    base_node_t *addrof_arg;
    if (arguments) 
      addrof_arg = jsbuilder_->CreateExprAddrof(0, arguments);
    else addrof_arg = jsbuilder_->GetConstUInt32(0);
    args.push_back(addrof_arg);
    BaseNode *arg_length = jsbuilder_->GetConstUInt32(arg_num);
    args.push_back(arg_length);

    IntrinDesc *intrindesc = &IntrinDesc::intrintable[idx];
    MIRType *retty = intrindesc->GetReturnType();
    MIRSymbol *temp = CreateTempVar(retty);
    StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssignedN((MIRIntrinsicId)idx, args, temp);
    stmt->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
    return jsbuilder_->CreateExprDread(retty, temp);
  }

  if((MIRIntrinsicId)idx == INTRN_JS_NUMBER || (MIRIntrinsicId)idx == INTRN_JS_STRING) {
    base_node_t * argument;
    if(arg_num == 0) {
      argument = CompileOpConstValue(JSTYPE_NONE, 0);
    }
    else {
      base_node_t *bn = tmp_stack.top();
      argument = CheckConvertToJSValueType(bn);
    }
    IntrinDesc *intrindesc = &IntrinDesc::intrintable[idx];
    MIRType *retty = intrindesc->GetReturnType();
    MIRSymbol *temp = CreateTempVar(retty);
    StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned1((MIRIntrinsicId)idx, argument, temp);
    stmt->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
    return jsbuilder_->CreateExprDread(retty, temp);
  }

   IntrinDesc *intrindesc = &IntrinDesc::intrintable[idx];
   MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  // Push args into arguments.
  for (uint32_t i = 0; i < arg_num; i++) {
    base_node_t *bn = tmp_stack.top();
    tmp_stack.pop();
    arguments.push_back(CheckConvertToRespectiveType(bn, intrindesc->GetArgType(i)));
  }

  MIRType *retty = intrindesc->GetReturnType();
  MIRSymbol *temp = CreateTempVar(retty);
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssignedN((MIRIntrinsicId)idx, arguments, temp);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);

  return jsbuilder_->CreateExprDread(retty, temp);
}

base_node_t *JSCompiler::CompileOpCall(uint32_t argc) {
  DEBUGPRINT2(argc);
  // May be call the method of builtin-object, just emit the corresponding intrinsic call.
  bool need_this = false;
  int32_t idx = GetBuiltinMethod(argc, &need_this);
  if (idx != -1) {
    return CompileBuiltinMethod(idx, argc, need_this);
  }

  // to reverse the order of args
  std::vector<base_node_t *> argsvec;
  for (uint32_t i = 0; i < argc; i++) {
    // argsvec.insert(argsvec.begin(), Pop());
    argsvec.push_back(CheckConvertToJSValueType(Pop()));
  }

  MIRSymbol *var;

  // impnode: implicitethis - first arg for closure node if needed
  // funcnode: it is intervened with arg setup
  base_node_t *impnode = CheckConvertToJSValueType(Pop());
  base_node_t *funcnode = CheckConvertToJSValueType(Pop());
  StmtNode *stmt = NULL;
  MIRSymbol *symbol;
  char *name;
  if (js2mplDebug > 2) static_cast<BaseNode *>(funcnode)->Dump(module_);

  MapleVector<base_node_t *> args(module_->CurFuncCodeMpAllocator()->Adapter());
  bool useSimpleCall = false;
  char *funcname = NULL;
  puidx_t puidx;


  if (funcnode->op == OP_dread) {
    AddrofNode *dread = static_cast<AddrofNode *>(funcnode);
    MIRSymbol *funcobj = module_->CurFunction()->GetLocalOrGlobalSymbol(dread->stidx, true);
    // the function might not be a global one, embeded in obj
    if (funcobj) {
      std::string funcobjname = funcobj->GetName();
      DEBUGPRINT3(funcobjname);

      funcname = GetFuncName(funcobjname.c_str());
      if (funcname) {
        DEBUGPRINT3(funcname);
        JSMIRFunction *func = closure_->GetJSMIRFunc(funcname);
        if (func) {
          puidx = func->puidx;
          useSimpleCall = UseSimpleCall(funcname);
        }
      } else if(IsCCall(funcobjname.c_str()) || IsXcCall(funcobjname.c_str())) {
        funcname = (char *)funcobjname.c_str();
      }
    }
  }

  MIRSymbol *returnVar = CreateTempVar(jsvalue_type_);

  if (useSimpleCall) {
    JSMIRFunction *func = closure_->GetJSMIRFunc(funcname);
    BaseNode *undefined = CompileOpConstValue(JSTYPE_UNDEFINED, 0);
    // This value.
    args.push_back(undefined);
    // Discard redundant arguments.
    int32_t last = argc > func->argc ? argc - func->argc : 0;
    for (int32_t i = argc - 1; i >= last; i--)
      args.push_back(argsvec[i]);

    if (argc < func->argc) {
      for (int32_t i = argc; i < func->argc; i++)
        args.push_back(undefined);
    }

    stmt = jsbuilder_->CreateStmtCallAssigned(puidx, args, returnVar, OP_callassigned);
  } else if (funcname && IsCCall(funcname)) {
    args.push_back(argsvec[argc-1]);
    BaseNode *argc_node = jsbuilder_->GetConstUInt32((uint32_t)argc-1);
    args.push_back(argc_node);
    for (int32_t i = argc - 2; i >=0; i--)
      args.push_back(argsvec[i]);

    stmt = jsbuilder_->CreateStmtIntrinsicCallAssignedN(INTRN_JSOP_CCALL, args, returnVar);
  } else if (funcname && IsXcCall(funcname)) {
    for (int32_t i = argc - 1; i >= 0; i--)
      args.push_back(argsvec[i]);
    stmt = jsbuilder_->CreateStmtXintrinsicCallAssignedN((MIRIntrinsicId)0, args, returnVar);
  } else {
    args.push_back(funcnode);
    args.push_back(impnode);
    for (int32_t i = argc - 1; i >=0; i--)
      args.push_back(argsvec[i]);

    stmt = jsbuilder_->CreateStmtIntrinsicCallAssignedN(INTRN_JSOP_CALL, args, returnVar);
  }

  if (stmt)
    stmt->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(stmt);

  return jsbuilder_->CreateExprDread(jsvalue_type_, 0, returnVar);
}

base_node_t *JSCompiler::CompileOpNew(uint32_t argc) {
  // Reverse the order of args.
  std::vector<base_node_t *> argsvec;
  for (uint32_t i = 0; i < argc; i++) {
    argsvec.push_back(CheckConvertToJSValueType(Pop()));
  }

  // impnode: implicitethis - first arg for closure node if needed
  // funcnode: it is intervened with arg setup
  base_node_t *impnode = Pop();
  base_node_t *funcnode = CheckConvertToJSValueType(Pop());

  MapleVector<base_node_t *> args(module_->CurFuncCodeMpAllocator()->Adapter());
  args.push_back(funcnode);
  args.push_back(impnode);
  for (int32_t i = argc - 1; i >=0; i--)
    args.push_back(argsvec[i]);
  MIRSymbol *returnVar = CreateTempVar(jsvalue_type_);
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssignedN(INTRN_JSOP_NEW, args, returnVar);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return jsbuilder_->CreateExprDread(jsvalue_type_, 0, returnVar);
}

js_builtin_id JSCompiler::EcmaNameToId(char *name) {
  if (!strcmp(name, "Object")) return JS_BUILTIN_OBJECT;
  else if (!strcmp(name, "Array"))  return JS_BUILTIN_ARRAY;
  else if (!strcmp(name, "String")) return JS_BUILTIN_STRING;
  else if (!strcmp(name, "Boolean")) return JS_BUILTIN_BOOLEAN;
  else if (!strcmp(name, "Number")) return JS_BUILTIN_NUMBER;
  else if (!strcmp(name, "Function")) return JS_BUILTIN_FUNCTION;
  else if (!strcmp(name, "exports"))  return JS_BUILTIN_EXPORTS;  // for plugin
  else if (!strcmp(name, "module"))  return JS_BUILTIN_MODULE;
  else if (!strcmp(name, "Math"))  return JS_BUILTIN_MATH;
  else if (!strcmp(name, "JSON"))  return JS_BUILTIN_JSON;
  else return JS_BUILTIN_COUNT;
}

base_node_t *JSCompiler::CompileBuiltinObject(char *name) {
  js_builtin_id id = EcmaNameToId(name);
  if (id == JS_BUILTIN_COUNT)
    return NULL;
  BaseNode *id_node = jsbuilder_->GetConstUInt32((uint32_t) id);
  return CompileGeneric1(INTRN_JS_GET_BIOBJECT, id_node, false);
}

// JSOP_NAME 59
base_node_t *JSCompiler::CompileOpName(JSAtom *atom, jsbytecode *pc) {
  char *name = Util::GetString(atom, mp_, jscontext_);
  JS_ASSERT(!name && "empty name");

  // Report error when use unspported name.
  if (!strcmp(name, "NaN"))
    assert(false && "Can not support NaN.");
  if (!strcmp(name, "Infinity"))
    assert(false && "Can not support Infinity.");
  if (!strcmp(name, "parseInt"))
    assert(false && "Can not support parseInt.");
  if (!strcmp(name, "parseFloat"))
    assert(false && "Can not support parseFloat.");
  if (!strcmp(name, "isNaN"))
    assert(false && "Can not support isNaN.");
  if (!strcmp(name, "isFinite"))
    assert(false && "Can not support isFinite.");
  if (!strcmp(name, "decodeURI"))
    assert(false && "Can not support decodeURI.");
  if (!strcmp(name, "decodeURIComponent"))
    assert(false && "Can not support decodeURIComponent.");
  if (!strcmp(name, "encodeURI"))
    assert(false && "Can not support encodeURI.");
  if (!strcmp(name, "encodeURIComponent"))
    assert(false && "Can not support encodeURIComponent.");

  // Null or undefined.
  if (!strcmp(name, "null"))
    return CompileOpConstValue(JSTYPE_NULL, 0);
  if (!strcmp(name, "undefined"))
    return CompileOpConstValue(JSTYPE_UNDEFINED, 0);

  base_node_t *builtin_object = CompileBuiltinObject(name);
  if (builtin_object)
    return builtin_object;

  base_node_t *bn = NULL;
  if (scope_->IsFunction(name)) {
    DEBUGPRINT2(name);
    char * objname = Util::GetNameWithSuffix(name, "_obj_", mp_);
    if (!GetFuncName(objname)) {
      std::pair<char*, char*> P(objname, name);
      objFuncMap.push_back(P);
    }
    name = objname;
  }

  // ??? Generate a dread node to pass the name.
  MIRSymbol *var;
  bool created;
  if (jsbuilder_->IsGlobalName(name) || IsCCall(name) || IsXcCall(name)){
    var = jsbuilder_->GetOrCreateGlobalDecl(name, jsvalue_type_, created);
  } else {
    var = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_, created);
  }

  // print is a builtin function.
  if (!strcmp(name, "print") || !strcmp(name, "$ERROR") || !strcmp(name, "SetCycleHeader") ||
      IsCCall(name) || IsXcCall(name)) {
    created = false;
  }

  InitWithUndefined(created, var);

  stidx_t stidx = var->GetStIdx();
  DEBUGPRINT3(stidx.Idx());

  bn = jsbuilder_->CreateExprDread(jsvalue_type_, var);

  if (created && eh_->IsInEHrange(pc)) {
    StmtNode *throwstmt = jsbuilder_->CreateStmtThrow(bn);
    throwstmt->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(throwstmt);
  }

  return bn;
}

int32_t JSCompiler::GetBuiltinStringId(const jschar *chars, size_t length) {

  static const struct {
    const char *chars;
    uint32_t length;
  } builtin_strings[JSBUILTIN_STRING_ID_COUNT] = {
#define JSBUILTIN_STRING_DEF(id, length, str) {(const char *)str, length},
#include "../include/jsbuiltinstrings.inc.h"
#undef JSBUILTIN_STRING_DEF
  };
  uint32_t i;
  for (i = 0; i < JSBUILTIN_STRING_ID_COUNT; i++) {
    if (builtin_strings[i].length != length)
      continue;
    uint32_t j;
    for (j = 0; j < length; j++) {
      if (builtin_strings[i].chars[j+4] != chars[j])
        break;
    }
    if (j == length)
      break;
  }
  if (i == JSBUILTIN_STRING_ID_COUNT) return -1;
  return (int32_t)i;
}

bool IsAsciiChars(const jschar *chars, uint32_t length) {
  for (uint32_t i = 0; i < length; i++) {
    if (chars[i] >= 256) return false;
  }
  return true;
}

// JSOP_STRING 61
base_node_t *JSCompiler::CompileOpString(JSString *str) {
  size_t length = 0;
  const jschar *chars = JS_GetInternedStringCharsAndLength(str, &length);
  int32_t id = GetBuiltinStringId(chars, length);
  if (jsstring_map_[chars])
    return jsstring_map_[chars];
  if (id != -1) {
    base_node_t *expr = CompileGeneric1((MIRIntrinsicId)INTRN_JS_GET_BISTRING,
                           jsbuilder_->GetConstUInt32(id), false);
    jsstring_map_[chars] = expr;
    return expr;
  }

  if (length >= pow(2, 16))
    assert(false && "Not Support too long string now");
  MIRType *unit_type = IsAsciiChars(chars, length) ? jsbuilder_->GetUInt8() : jsbuilder_->GetUInt16();
  uint32_t pad = IsAsciiChars(chars, length) ? 4 : 2;
  uint32_t string_class = IsAsciiChars(chars, length) ? 0 : JSSTRING_UNICODE;

  size_t padding_length = length + pad;
  MIRType *type = jsbuilder_->GetOrCreateArrayType(unit_type, 1, &(padding_length));
  const char *temp_name = Util::GetSequentialName("const_chars_", temp_var_no_, mp_);
  MIRSymbol *var = jsbuilder_->GetOrCreateGlobalDecl(temp_name, type);
  MIRAggConst *init =  module_->mp_->New<MIRAggConst>(module_, type);

  uint8_t cl[4];
  cl[0] = string_class;
  cl[1] = 0;
  cl[2] = length & 0xff;
  cl[3] = (length & 0xff00) >> 8;

  if ((string_class & JSSTRING_UNICODE) == 0) {
    uint64_t val = (uint64_t)(cl[0]);
    MIRIntConst *int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);
    val = (uint64_t)(cl[1]);
    int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);

    val = (uint64_t)(cl[2]);
    int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);
    val = (uint64_t)(cl[3]);
    int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);
  } else {
    uint16_t *tmp = (uint16_t *)cl;
    uint64_t val = (uint64_t)(cl[0]);
    MIRIntConst *int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);
    val = (uint64_t)(tmp[1]);
    int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);
  }

  for (uint32_t i = 0; i < length; i++) {
    uint64_t val = chars[i];
    MIRIntConst *int_const = mp_->New<MIRIntConst>(val, unit_type);
    init->const_vec.push_back(int_const);
  }
  var->value_.const_ = init;
  base_node_t *expr = jsbuilder_->CreateExprAddrof(0, var);
  expr->ptyp = PTY_simplestr;
  jsstring_map_[chars] = expr;
  return expr;
}

//JSOP_ITER 75
base_node_t *JSCompiler::CompileOpNewIterator(base_node_t *bn, uint8_t flags) 
{
  MIRType *retty = jsbuilder_->GetOrCreatePointerType(jsbuilder_->GetVoid());
  MIRSymbol *retsy = CreateTempVar(retty);

  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned2(
      (MIRIntrinsicId)INTRN_JSOP_NEW_ITERATOR,bn,
      jsbuilder_->GetConstUInt32(flags), retsy);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return jsbuilder_->CreateExprDread(retty, 0, retsy);  
}

base_node_t *JSCompiler::CompileOpMoreIterator(base_node_t *iterator) 
{
  MIRSymbol *retsy = CreateTempVar(jsbuilder_->GetUInt32());
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned1(
                   (MIRIntrinsicId)INTRN_JSOP_MORE_ITERATOR, iterator, retsy);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);

  return jsbuilder_->CreateExprDread(jsbuilder_->GetUInt32(), retsy);
  
}

// JSOP_ITERNEXT 77
base_node_t *JSCompiler::CompileOpIterNext(base_node_t *iterator)
{
  MIRSymbol *var = CreateTempJSValueTypeVar();
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned1( 
      (MIRIntrinsicId)INTRN_JSOP_NEXT_ITERATOR,
      iterator,
      var);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);

  return jsbuilder_->CreateExprDread(jsvalue_type_, var);
} 

// JSOP_GETARG 84
base_node_t *JSCompiler::CompileOpGetArg(uint32_t i) {
  DEBUGPRINT2(i);
  JSMIRFunction *fun = jsbuilder_->GetCurrentFunction();
  int start = (fun->with_env_arg) ? 2 : 1;
  MIRSymbol *arg = jsbuilder_->GetFunctionArgument(fun, i+start); 
  base_node_t *irn = jsbuilder_->CreateExprDread(jsbuilder_->GetDynany(), arg);
  return irn;
}

// JSOP_SETARG 85
void JSCompiler::CompileOpSetArg(uint32_t i, base_node_t *val) {
  DEBUGPRINT2(i);
  JSMIRFunction *fun = jsbuilder_->GetCurrentFunction();
  int start = (fun->with_env_arg) ? 2 : 1;
  MIRSymbol *arg = jsbuilder_->GetFunctionArgument(fun, i+start);  // skip this and env parameters
  opstack_->ReplaceStackItemsWithTemps(this, arg);
  jsbuilder_->CreateStmtDassign(arg, 0, val, linenum_);
  base_node_t *bn = jsbuilder_->CreateExprDread(jsvalue_type_, arg);
  Push(bn);
  return;
}

// JSOP_GETLOCAL 86
base_node_t *JSCompiler::CompileOpGetLocal(uint32_t local_no) {
  JSMIRFunction *func = jsbuilder_->GetCurrentFunction();
  char *name = closure_->GetLocalVar(func, local_no);
  bool created;
  MIRSymbol *var;

  // for function name, use suffix _obj_
  if (scope_->IsFunction(name)) {
    char * objname = Util::GetNameWithSuffix(name, "_obj_", mp_);
    if (!GetFuncName(objname)) {
      std::pair<char*, char*> P(objname, name);
      objFuncMap.push_back(P);
    }
    name = objname;
  }
  var = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_, created);
  InitWithUndefined(created, var);
  if (!created) {
    MIRType *type = globaltable.GetTypeFromTyIdx(var->GetTyIdx());
    return jsbuilder_->CreateExprDread(type, var);
  }

  return jsbuilder_->CreateExprDread(jsvalue_type_, var);
}

// JSOP_SETLOCAL 87
StmtNode *JSCompiler::CompileOpSetLocal(uint32_t local_no, base_node_t *src) {
  JSMIRFunction *func = jsbuilder_->GetCurrentFunction();
  char *name = closure_->GetLocalVar(func, local_no);
  MIRSymbol *var;

  // for function name, use suffix _obj_
  if (scope_->IsFunction(name)) {
    char * objname = Util::GetNameWithSuffix(name, "_obj_", mp_);
    if (!GetFuncName(objname)) {
      std::pair<char*, char*> P(objname, name);
      objFuncMap.push_back(P);
    }
    name = objname;
  }
  var = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_);
  // if the stack is not empty, for each stack item that contains the 
  // variable being set, evaluate and store the result in a new temp and replace
  // the stack items by the temp
  opstack_->ReplaceStackItemsWithTemps(this, var);

  base_node_t *bn = CheckConvertToJSValueType(src);
  return jsbuilder_->CreateStmtDassign(var, 0, bn, linenum_);
}

// JSOP_NEWINIT 89
base_node_t *JSCompiler::CompileOpNewInit(uint32_t kind) {
  if (kind == JSProto_Array) {
    assert(false && "NIY");
  } else {
    assert(kind == JSProto_Object);
    return CompileGeneric0(INTRN_JS_NEW_OBJECT_0, true);
  }
  return NULL;
}


base_node_t *JSCompiler::CompileGenericN(int32_t intrin_id,
                                        MapleVector<base_node_t *> &arguments,
                                        bool is_call) {
  IntrinDesc *intrindesc = &IntrinDesc::intrintable[intrin_id];
  MIRType *retty = intrindesc->GetReturnType();
  if (is_call) {
    MIRSymbol *var = CreateTempVar(retty);
    StmtNode *call = jsbuilder_->CreateStmtIntrinsicCallAssignedN(
                       (MIRIntrinsicId)intrin_id, arguments, var);
    call->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(call);
    //  TODO: if retty is void, return NULL
    return jsbuilder_->CreateExprDread(retty, var);
  } else {
    return jsbuilder_->CreateExprIntrinsicopN(
                      (MIRIntrinsicId)intrin_id, retty, arguments);
  }
}

base_node_t *JSCompiler::CompileGeneric0(int32_t intrin_id, bool is_call) {
  MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  return CompileGenericN(intrin_id, arguments, is_call);
}

base_node_t *JSCompiler::CompileGeneric1(int32_t intrin_id,
                                      base_node_t *arg, bool is_call) {
  MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  arguments.push_back(arg);
  return CompileGenericN(intrin_id, arguments, is_call);
}

base_node_t *JSCompiler::CompileGeneric2(int32_t intrin_id, base_node_t *arg1,
                                      base_node_t *arg2, bool is_call) {
  MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  arguments.push_back(arg1);
  arguments.push_back(arg2);
  return CompileGenericN(intrin_id, arguments, is_call);
}

base_node_t *JSCompiler::CompileGeneric3(int32_t intrin_id, base_node_t *arg1,
                                      base_node_t *arg2, base_node_t *arg3, bool is_call) {
  MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  arguments.push_back(arg1);
  arguments.push_back(arg2);
  arguments.push_back(arg3);
  return CompileGenericN(intrin_id, arguments, is_call);
}

base_node_t *JSCompiler::CompileGeneric4(int32_t intrin_id, base_node_t *arg1,
                                      base_node_t *arg2, base_node_t *arg3, base_node_t *arg4, bool is_call) {
  MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  arguments.push_back(arg1);
  arguments.push_back(arg2);
  arguments.push_back(arg3);
  arguments.push_back(arg4);
  return CompileGenericN(intrin_id, arguments, is_call);
}

bool JSCompiler::CompileOpSetElem(base_node_t *obj, base_node_t *index, base_node_t *val) {
  index = CheckConvertToJSValueType(index);
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(
                     INTRN_JSOP_SETPROP,
                     obj, index, CheckConvertToJSValueType(val), NULL);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return true;
}

bool JSCompiler::CompileOpInitPropGetter(base_node_t *obj, JSString *str, base_node_t *val) {
  base_node_t *name = CheckConvertToJSValueType(CompileOpString(str));
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_INITPROP_GETTER,
                                                        obj, name, val, NULL);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return true;
}

bool JSCompiler::CompileOpInitPropSetter(base_node_t *obj, JSString *str, base_node_t *val) {
  base_node_t *name = CheckConvertToJSValueType(CompileOpString(str));
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_INITPROP_SETTER,
                                                        obj, name, val, NULL);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return true;
}

bool JSCompiler::CompileOpInitElemGetter(base_node_t *obj, base_node_t *index, base_node_t *val) {
  index = CheckConvertToJSValueType(index);
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_INITPROP_GETTER,
                                                        obj, index, val, NULL);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return true;
}

bool JSCompiler::CompileOpInitElemSetter(base_node_t *obj, base_node_t *index, base_node_t *val) {
  index = CheckConvertToJSValueType(index);
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_INITPROP_SETTER,
                                                        obj, index, val, NULL);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return true;
}

// JSOP_SETPROP 54
bool JSCompiler::CompileOpSetProp(base_node_t *obj, JSString *str,
                                  base_node_t *val) {
  base_node_t *name = CompileOpString(str);
  StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_SETPROP_BY_NAME, obj,
                                                        name,
                                                        CheckConvertToJSValueType(val), NULL);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return true;
}

// JSOP_BINDNAME 110
base_node_t *JSCompiler::CompileOpBindName(JSAtom *atom) {
  char *name = Util::GetString(atom, mp_, jscontext_);
  JS_ASSERT(!name && "empty name");
  base_node_t *builtin_object = CompileBuiltinObject(name);
  if (builtin_object)
    return builtin_object;

  MIRSymbol *var;
  JSMIRFunction *func = funcstack_.top();
  bool created;
  // search the scope chain
  while (func) {
    if (closure_->IsLocalVar(func, name)) {
      var = jsbuilder_->GetOrCreateDeclInFunc(name, jsvalue_type_, func, created);
      InitWithUndefined(created, var);
      break;
    }

    // function introduced a global var
    if (func == jsmain_) {
      var = jsbuilder_->GetOrCreateGlobalDecl(name, jsvalue_type_, created);
      InitWithUndefined(created, var);
      jsbuilder_->InsertGlobalName(name);
      break;
    }

    func = func->scope->GetParentFunc();
  }

  // ??? Generate a dread node to pass the name.
  base_node_t *bn = jsbuilder_->CreateExprDread(jsvalue_type_, var);

  return bn;
}

// JSOP_SETNAME 110
bool JSCompiler::CompileOpSetName(JSAtom *atom, base_node_t *val) {
  char *name = Util::GetString(atom, mp_, jscontext_);
  JS_ASSERT(!name && "empty name");
  JSMIRFunction *func = funcstack_.top();
  MIRSymbol *var = closure_->GetSymbolFromEnclosingScope(func, name);
  if (!var) {
    var = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_);
  }

  // if the stack is not empty, for each stack item that contains the 
  // variable being set, evaluate and store the result in a new temp and replace
  // the stack items by the temp
  opstack_->ReplaceStackItemsWithTemps(this, var);
  jsbuilder_->CreateStmtDassign(var, 0, CheckConvertToJSValueType(val), linenum_);
  base_node_t *bn = jsbuilder_->CreateExprDread(jsvalue_type_, var);
  Push(bn);
  return true;
}

// JSOP_DEFFUN 127
bool JSCompiler::CompileOpDefFun(JSFunction *jsfun) {
  JSScript *scr = jsfun->getOrCreateScript(jscontext_);
  JSAtom *atom = jsfun->displayAtom();
  DEBUGPRINT2(atom);
  char *funcname = Util::GetString(atom, mp_, jscontext_);
  if (!funcname)
    return false;

  JSMIRFunction *mfun = jsbuilder_->GetFunction(funcname);
  mfun->SetUserFunc();

  MIRSymbol *func_st = jsbuilder_->GetOrCreateGlobalDecl(funcname, jsvalue_type_);
  base_node_t *ptr = jsbuilder_->CreateExprAddroffunc(func_st->value_.func_->puidx);
  assert(jsfun && "not a jsfunction");

  char *name = Util::GetNameWithSuffix(funcname, "_obj_", mp_);
  if (jsbuilder_->GetStringIndex(name).idx == 0) {
    DEBUGPRINT2(name);
    uint32_t varg_p = 0;
    uint32_t nargs = (uint32_t)(uint8_t)jsfun->nargs();
    uint32_t length = nargs;
    uint32_t flag = jsfun->strict() ? JSFUNCPROP_STRICT | JSFUNCPROP_USERFUNC : JSFUNCPROP_USERFUNC;
    uint32_t attrs = varg_p << 24 | nargs << 16 | length << 8 | flag;
    base_node_t *func_node = CompileGeneric3(INTRN_JS_NEW_FUNCTION, ptr, jsbuilder_->GetConstInt(0), jsbuilder_->GetConstUInt32(attrs), true);

    MIRSymbol *func_obj;
    if (mfun->scope->IsWithEnv()) {
      func_obj = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_);
    } else {
      func_obj = jsbuilder_->GetOrCreateGlobalDecl(name, jsvalue_type_);
      jsbuilder_->InsertGlobalName(name);
    }

    StmtNode *stmt = jsbuilder_->CreateStmtDassign(func_obj, 0, func_node, linenum_);
    stmt->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  }

  funcstack_.push(mfun);
  jsbuilder_->SetCurrentFunction(mfun);
  DEBUGPRINTfunc(funcname);

  CompileScript(scr);
  return true;
}

// JSOP_DEFVAR 129
bool JSCompiler::CompileOpDefVar(JSAtom *atom) {
  char *name = Util::GetString(atom, mp_, jscontext_);
  JS_ASSERT(!name && "empty name");
  JSMIRFunction *fun = jsbuilder_->GetCurrentFunction();
  DEBUGPRINT2((fun == funcstack_.top()));
  DEBUGPRINT2(fun);
  bool created;
  MIRSymbol *var = jsbuilder_->GetOrCreateGlobalDecl(name, jsvalue_type_, created);
  InitWithUndefined(created, var);
  if (fun == jsmain_) {
    jsbuilder_->InsertGlobalName(name);
  }
  return true;
}

// JSOP_LAMBDA 131
base_node_t *JSCompiler::CompileOpLambda(jsbytecode *pc, JSFunction *jsfun) {
  JSAtom *atom = jsfun->displayAtom();

  // isLambda() does not seem reliable
  DEBUGPRINT3((jsfun->isLambda()));
  // we already know it is a Lambda so only check other two parts in isNamedLambda()
  // isLambda() && displayAtom() && !hasGuessedAtom()
  // if((jsfun->isNamedLambda()))
  char *funcname = NULL;
  bool has_name = false;
  if (atom && !jsfun->hasGuessedAtom()) {
    funcname = Util::GetString(atom, mp_, jscontext_);
    has_name = true;
  } else {
    funcname = scope_->GetAnonyFunctionName(pc);
  }

  JSMIRFunction *lambda = jsbuilder_->GetFunction(funcname);
  lambda->SetUserFunc();
  DEBUGPRINT2(lambda);

  JSMIRFunction *parentFunc = funcstack_.top();

  MIRSymbol *funcsymbol = jsbuilder_->GetOrCreateGlobalDecl(funcname, jsvalue_type_);
  base_node_t *ptr = jsbuilder_->CreateExprAddroffunc(funcsymbol->value_.func_->puidx);
  MIRSymbol * env_var = NULL;
  base_node_t *node;
  DEBUGPRINT2((lambda->scope->GetName()));
  DEBUGPRINT2((lambda->scope->IsTopLevel()));
  if (parentFunc->scope->IsWithEnv()) {
    env_var = jsbuilder_->GetOrCreateLocalDecl("environment", jsbuilder_->GetDynany());
    node = jsbuilder_->CreateExprDread(jsbuilder_->GetDynany(), env_var);
    lambda->penvtype = parentFunc->envptr;
  } else {
    node = jsbuilder_->GetConstInt(0);
    DEBUGPRINTs3("lambda->scope->IsTopLevel()");
  }

  uint32_t varg_p = 0;
  uint32_t nargs = (uint32_t)(uint8_t)jsfun->nargs();
  uint32_t length = nargs;
  uint32_t flag = jsfun->strict() ? JSFUNCPROP_STRICT | JSFUNCPROP_USERFUNC : JSFUNCPROP_USERFUNC;
  uint32_t attrs = varg_p << 24 | nargs << 16 | length << 8 | flag;
  base_node_t *bn = CompileGeneric3(INTRN_JS_NEW_FUNCTION, ptr, node, jsbuilder_->GetConstUInt32(attrs), true);

  if(has_name) {
    char *name = Util::GetNameWithSuffix(funcname, "_obj_", mp_);
    MIRSymbol *func_obj = jsbuilder_->GetOrCreateGlobalDecl(name, jsvalue_type_);
    jsbuilder_->InsertGlobalName(name);
    jsbuilder_->CreateStmtDassign(func_obj, 0, bn, linenum_);
    bn = jsbuilder_->CreateExprDread(func_obj);
  }
  std::pair<JSScript *, JSMIRFunction *> P(jsfun->nonLazyScript(), lambda);
  scriptstack_.push(P);
  return bn;
}

// JSOP_GETALIASEDVAR 136
int JSCompiler::ProcessAliasedVar(JSAtom *atom, MIRType *&env_ptr, base_node_t *&env_node, int &depth) {
  JSMIRFunction *func = funcstack_.top();
  DEBUGPRINT3(func);
  char *name = Util::GetString(atom, mp_, jscontext_);
  JS_ASSERT(!name && "empty name");
  MIRSymbol *func_st = globaltable.GetSymbolFromStidx(func->stidx.Idx());
  const char *funcname = func_st->GetName().c_str();
  ScopeNode *sn = scope_->GetOrCreateSN((char *)funcname);
  ScopeNode *psn = sn->GetParent();

  int idx = 0;
  if (!psn) {
    DEBUGPRINT3("alias var from catch block"); 
    return idx;
  }

  JSMIRFunction *pfunc = psn->GetFunc();

  depth = 0;
  MIRSymbol *env_var = NULL;
  const char *env_name;
  base_node_t *bn = NULL;
  StmtNode *stmt = NULL;

  // search in current func's alias list
  // depth = 0
  if (!idx && sn->IsWithEnv()) {
    env_var = jsbuilder_->GetOrCreateLocalDecl("environment", func->envptr);
    DEBUGPRINTsv3("environment", func->envptr);
    idx = jsbuilder_->GetStructFieldIdFromFieldName(func->envtype, name);
    DEBUGPRINT3(idx);
    depth = 0;

    DEBUGPRINTsv3("func to get env_ptr", func);

    env_ptr = func->envptr;
    env_node = jsbuilder_->CreateExprDread(func->envptr, env_var);
  }

  // recursively search in parent's alias list
  // depth >= 1
  if (pfunc != jsmain_ && !idx) {
    depth = 1;
    DEBUGPRINTs3("recursively search in parent's alias lists");
    env_var = func->_formals[ENV_POSITION_IN_ARGS];

    env_ptr = pfunc->envptr;
    env_node = jsbuilder_->CreateExprDread(pfunc->envptr, env_var);
    idx = jsbuilder_->GetStructFieldIdFromFieldName(pfunc->envtype, name);
    DEBUGPRINT3(idx);

    sn = psn;
    psn = psn->GetParent();

    if (psn) {
      func = pfunc;
      pfunc = psn->GetFunc();

      while (pfunc != jsmain_ && !idx && psn && psn->IsWithEnv()) {
        depth ++;
        DEBUGPRINTsv3("func to get env_ptr", pfunc);

        env_ptr = pfunc->envptr;
        env_node = jsbuilder_->CreateExprDread(pfunc->envptr, env_var);
        idx = jsbuilder_->GetStructFieldIdFromFieldName(pfunc->envtype, "parentenv");
        env_node = jsbuilder_->CreateExprIread(pfunc->envptr, pfunc->envptr, idx, env_node);

        env_name = Util::GetSequentialName("env_", temp_var_no_, mp_);
        env_var = jsbuilder_->GetOrCreateLocalDecl(env_name, pfunc->envptr);
        stmt = jsbuilder_->CreateStmtDassign(env_var, 0, env_node, linenum_);
        env_node = jsbuilder_->CreateExprDread(pfunc->envptr, env_var);
        idx = jsbuilder_->GetStructFieldIdFromFieldName(pfunc->envtype, name);
        DEBUGPRINT3(idx);

        sn = psn;
        psn = psn->GetParent();
        func = pfunc;

        if (psn)
          pfunc = psn->GetFunc();
      }
    }
  }

  DEBUGPRINT2(depth);

  return idx;
}


base_node_t *JSCompiler::CompileOpGetAliasedVar(JSAtom *atom) {
  MIRType *env_ptr;
  base_node_t *env_node;
  int depth = 0;

  int idx = ProcessAliasedVar(atom, env_ptr, env_node, depth);

  base_node_t *bn = NULL;
  if (idx) {
    bn = jsbuilder_->CreateExprIread(jsvalue_type_, env_ptr, idx, env_node);
  } else {
    // add to local
    DEBUGPRINT3("alias var not found, could be from block");
    char *name = Util::GetString(atom, mp_, jscontext_);
    MIRSymbol *var = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_);
    bn = jsbuilder_->CreateExprDread(jsvalue_type_, var);
  }

  return bn;
}

// JSOP_SETALIASEDVAR 137
base_node_t *JSCompiler::CompileOpSetAliasedVar(JSAtom *atom, base_node_t *val) {
  MIRType *env_ptr;
  base_node_t *env_node;
  int depth = 0;

  int idx = ProcessAliasedVar(atom, env_ptr, env_node, depth);

  StmtNode *bn = NULL;
  if (idx) {
    bn = jsbuilder_->CreateStmtIassign(env_ptr, idx, env_node, val);
  } else {
    // add to local
    DEBUGPRINT3("alias var not found, could be from block");
    char *name = Util::GetString(atom, mp_, jscontext_);
    MIRSymbol *var = jsbuilder_->GetOrCreateLocalDecl(name, jsvalue_type_);
    bn = jsbuilder_->CreateStmtDassign(var, 0, val, linenum_);
  }
  bn->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(bn);

  return bn;
}

void JSCompiler::CloseFuncBookKeeping() {
  DEBUGPRINTfuncEnd();

  funcstack_.pop();

  // process inner functions
  DEBUGPRINT3((scriptstack_.size()));
  while (scriptstack_.size()) {
    JSScript *scr = scriptstack_.top().first;
    JSMIRFunction *lambda = scriptstack_.top().second;
    jsbuilder_->SetCurrentFunction(lambda);
    DEBUGPRINT0;
    MIRSymbol *lambda_st = globaltable.GetSymbolFromStidx(lambda->stidx.Idx());
    DEBUGPRINTfunc((lambda_st->GetName().c_str()));
    funcstack_.push(lambda);
    scriptstack_.pop();

    CompileScript(scr);
  }

  if (funcstack_.size()) {
    JSMIRFunction * currFunc = funcstack_.top();
    jsbuilder_->SetCurrentFunction(currFunc);
    
    if (funcstack_.size() == 1) {
      DEBUGPRINT0;
      DEBUGPRINTfunc("main");
    }
  }
}

StmtNode *JSCompiler::CompileOpIfJump(JSOp op, base_node_t *cond, jsbytecode *pcend) 
{
  labidx_t labidx = GetorCreateLabelofPc(pcend);
  StmtNode* gotonode =  jsbuilder_->CreateStmtCondGoto(cond, (op == JSOP_IFEQ || op==JSOP_AND)?OP_brfalse:OP_brtrue, labidx);
  gotonode->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(gotonode);
  return gotonode;
}

int64_t JSCompiler::GetIntValue(jsbytecode *pc){
  JSOp op = JSOp(*pc);
  switch(op)
  {
    case JSOP_ZERO:{
      return (int64_t)0;
    }
    case JSOP_ONE:{
      return (int64_t)1;
    }
    case JSOP_INT8:{
      int ival = GET_INT8(pc);
      return (int64_t)ival;
    }
    case JSOP_INT32:{
      int ival = GET_INT32(pc);
      return (int64_t)ival;
    }
    case JSOP_UINT16:{
      unsigned uval = GET_UINT16(pc);
      return (int64_t)uval;
    }
    case JSOP_UINT24:{
      unsigned uval = GET_UINT24(pc);
      return (int64_t)uval;
    }
    default:{
      assert(0);
    }
  }
}

SwitchNode *JSCompiler::CompileOpCondSwitch(base_node_t *opnd, JSScript *script,
                                          jsbytecode *pcstart, jsbytecode *pcend) {
  int64_t const_val;
  int offset;
  jsbytecode *pcjump;
  jsbytecode *pctemp1 = pcstart;
  jsbytecode *pctemp2 = js::GetNextPc(pcend);
  labidx_t mirlabel;
  CaseVector switchtable(module_->CurFuncCodeMpAllocator()->Adapter());
  labidx_t defaultlabel;

  while(pctemp1 < pctemp2){
    JSOp op = JSOp(*pctemp1);
    unsigned lineNo = js::PCToLineNumber(script, pctemp1);
    linenum_ = lineNo;
    Util::SetIndent(4);
    DEBUGPRINTnn(lineNo, Util::getOpcodeName[op]);
    DEBUGPRINT0;
    pctemp1 = js::GetNextPc(pctemp1);
  }

  while(pcstart < pcend){
    const_val = GetIntValue(pcstart);
    pcstart = js::GetNextPc(pcstart);
    offset = GET_JUMP_OFFSET(pcstart);
    pcjump = pcstart + offset;
    mirlabel = GetorCreateLabelofPc(pcjump);
    switchtable.push_back(CasePair(const_val, mirlabel));
    pcstart = js::GetNextPc(pcstart);
  }

  offset = GET_JUMP_OFFSET(pcstart);
  pcjump = pcstart + offset;
  mirlabel = GetorCreateLabelofPc(pcjump);
  defaultlabel = mirlabel;
  SwitchNode *switchnode = jsbuilder_->CreateStmtSwitch(opnd,
                                                    defaultlabel, switchtable);
  switchnode->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(switchnode);
  return switchnode;
}

SwitchNode *JSCompiler::CompileOpTableSwitch(base_node_t *opnd, int32_t len,
                                           JSScript *script, jsbytecode *pc) {
  int offset,i;
  jsbytecode *pcjump,*pctemp1,*pctemp2;
  labidx_t mirlabel;
  CaseVector switchtable(module_->CurFuncCodeMpAllocator()->Adapter());
  labidx_t defaultlabel;

  pctemp1 = pc;
  pctemp1 += JUMP_OFFSET_LEN;
  int32_t low = GET_JUMP_OFFSET(pctemp1);
  pctemp1 += JUMP_OFFSET_LEN;
  int32_t high = GET_JUMP_OFFSET(pctemp1);
  pctemp1 += JUMP_OFFSET_LEN;

  for(i = 0; i < high - low + 1; i++)
  {
    pctemp2 = pctemp1 + JUMP_OFFSET_LEN*i;
    offset = GET_JUMP_OFFSET(pctemp2);
    if(!offset)
    {
      continue;
    }
    pcjump = pc + offset;
    mirlabel = GetorCreateLabelofPc(pcjump);
    switchtable.push_back(CasePair((int64_t)(low + i), mirlabel));
  }

  pcjump = pc + len;
  mirlabel = GetorCreateLabelofPc(pcjump);
  defaultlabel = mirlabel;
  base_node_t *cond = CheckConvertToInt32(opnd);
   
  SwitchNode *stmt = jsbuilder_->CreateStmtSwitch(cond, defaultlabel, switchtable);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return stmt;
}

labidx_t JSCompiler::CreateLabel(char *pref) {
  const char *temp_name = Util::GetSequentialName(pref ? pref : "label", temp_var_no_, mp_);
  labidx_t labidx = jsbuilder_->GetorCreateMIRLabel(temp_name);
  return labidx;
}

labidx_t JSCompiler::GetorCreateLabelofPc(jsbytecode *pc, char *pref) {
  labidx_t labidx;
  if (label_map_[pc] != 0) {
    labidx = label_map_[pc];
  } else {
    labidx = CreateLabel(pref);
    label_map_[pc] = labidx;
  }
  return labidx;
}

GotoNode *JSCompiler::CompileOpGoto(jsbytecode *pc, jsbytecode *jumptopc, MIRSymbol *tempvar) {
  // check if it iss in try range and will jump out of it, add cleanuptry stmt
  EHstruct *eh = eh_->GetEHstruct(pc);
  EHstruct *ehjump = eh_->GetEHstruct(jumptopc);
  if (eh && eh != ehjump) {
    DEBUGPRINTs("creating cleanuptry");
    StmtNode* cleanuptrynode = module_->CurFuncCodeMp()->New<StmtNode>(OP_cleanuptry);
    cleanuptrynode->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(cleanuptrynode);
  }

  // use special endtry label for goto within the try range
  // to simplify interpreting endtry
  labidx_t labidx;
  if (eh && jumptopc == eh->endtrypc)
    labidx = eh->label;
  else
    labidx = GetorCreateLabelofPc(jumptopc);

  GotoNode* gotonode = jsbuilder_->CreateStmtGoto(OP_goto, labidx);
  gotonode->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(gotonode);
  if (tempvar)  // the label will be the merge point of a conditional expression
    label_tempvar_map_[labidx] = tempvar;
  return gotonode;
}

GotoNode *JSCompiler::CompileOpGosub(jsbytecode *pc) {
  labidx_t mirlabel = GetorCreateLabelofPc(pc, "f@");
  GotoNode* gosubnode = jsbuilder_->CreateStmtGoto(OP_gosub, mirlabel);
  gosubnode->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(gosubnode);
  return gosubnode;
}

TryNode *JSCompiler::CompileOpTry(jsbytecode *catch_pc) {
  TryNode* trynode = jsbuilder_->CreateStmtTry(OP_try, 0, 0);
  trynode->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(trynode);

  // set up label for endtry
  labidx_t mirlabel = CreateLabel();
  DEBUGPRINT3(mirlabel);
  // passed in is the catchpc which could also be finallypc in case there is no catch
  EHstruct *eh = eh_->GetEHstruct(0, catch_pc, catch_pc, 0);
  MIR_ASSERT(eh);
  eh_->SetEHLabel(eh, mirlabel);

  // set catch label in the try statement
  if (eh->catchpc) {
    mirlabel = GetorCreateLabelofPc(eh->catchpc, "h@");
    trynode->catch_offset = (uint16)mirlabel;
  }

  // set finally label in the try statement
  if (eh->finallypc) {
    mirlabel = GetorCreateLabelofPc(eh->finallypc, "f@");
    trynode->finally_offset = (uint16)mirlabel;
  }

  return trynode;
}

BaseNode *JSCompiler::CompileOpLoopHead(jsbytecode *pc) {
  labidx_t mirlabel = NULL;
  if (label_map_[pc] != 0) {
    mirlabel = label_map_[pc];
  } else {
    const char *temp_name = Util::GetSequentialName("label", temp_var_no_, mp_);
    mirlabel = jsbuilder_->GetorCreateMIRLabel(temp_name);
    label_map_[pc] = mirlabel;
  }
  StmtNode *stmt = jsbuilder_->CreateStmtLabel(label_map_[pc]);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  return stmt;
}

base_node_t *JSCompiler::CheckConvertToJSValueType(base_node_t *data)
{
  MIRType *to_type = NULL;
  if (IsPrimitiveDynType(data->ptyp))
    return data;
  switch (data->ptyp) {
    case PTY_ptr:
      return data;
    case PTY_u1:
      to_type = jsbuilder_->GetDynbool();
      break;
    case PTY_i32:
    case PTY_u32:
      to_type = jsbuilder_->GetDyni32();
      if (data->op == OP_constval) {
        ConstvalNode *cv = static_cast<ConstvalNode *>(data);
        if (cv) {
          cv->ptyp = PTY_dyni32;
          MIRIntConst *ic = static_cast<MIRIntConst *>(cv->constval);
          ic->value_ = (int64_t)((uint64_t)JSTYPE_NUMBER << 32 | (ic->value_ & 0xffffffff));
          cv->constval = ic;
          return cv;
        }
      }
      break;
    case PTY_simplestr:
      to_type = jsbuilder_->GetDynstr();
      break;
    case PTY_simpleobj:
      to_type = jsbuilder_->GetDynobj();
      break;
    default:
      assert("NIY");
      break;
  }
  return jsbuilder_->CreateExprTypeCvt(OP_cvt, to_type,
    jsbuilder_->GetPrimType(data->ptyp), data);
}

// Pops the top two values on the stack as rval and lval, compare them with ===,
// if the result is true, jumps to a 32-bit offset from the current bytecode,
// re-pushes lval onto the stack if false.
void JSCompiler::CompileOpCase(jsbytecode *pc, int offset, base_node_t *rval, base_node_t *lval)
{
  MIRIntrinsicId idx = (MIRIntrinsicId)FindIntrinsicForOp(JSOP_STRICTEQ);
  IntrinDesc *intrindesc = &IntrinDesc::intrintable[idx];
  MIRType *retty = intrindesc->GetReturnType();
  base_node_t *expr = jsbuilder_->CreateExprIntrinsicop2(idx, retty, CheckConvertToJSValueType(rval), CheckConvertToJSValueType(lval));
  base_node_t *cond = CheckConvertToBoolean(expr);

  labidx_t mirlabel = GetorCreateLabelofPc(pc + offset);
  CondGotoNode *gotonode = jsbuilder_->CreateStmtCondGoto(cond, OP_brtrue, mirlabel);
  gotonode->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(gotonode);
  Push(lval);
  return;
}

base_node_t *JSCompiler::CheckConvertToBoolean(base_node_t *node)
{
  if (IsPrimitiveInteger(node->ptyp)) {
    if (node->ptyp == PTY_u1) return node;
    return jsbuilder_->CreateExprTypeCvt(OP_cvt, jsbuilder_->GetUInt1(), jsbuilder_->GetPrimType(node->ptyp), node);
  }
  return jsbuilder_->CreateExprIntrinsicop1(INTRN_JS_BOOLEAN, jsbuilder_->GetUInt1(), node);
}

base_node_t *JSCompiler::CheckConvertToInt32(base_node_t *node)
{
  if (IsPrimitiveInteger(node->ptyp))
    return node;
#ifdef DYNAMICLANG
  return jsbuilder_->CreateExprTypeCvt(OP_cvt, jsbuilder_->GetInt32(), jsbuilder_->GetPrimType(node->ptyp), node);
#else
  BaseNode *expr = jsbuilder_->CreateExprIntrinsicop1(INTRN_JS_INT32, jsbuilder_->GetInt32(), node);
  MIRSymbol *var = CreateTempVar(jsbuilder_->GetInt32());
  return jsbuilder_->CreateExprDread(jsbuilder_->GetInt32(), var);
#endif
}

base_node_t *JSCompiler::CheckConvertToUInt32(base_node_t *node)
{
  if (IsPrimitiveInteger(node->ptyp))
    return node;
#ifdef DYNAMICLANG
  return jsbuilder_->CreateExprTypeCvt(OP_cvt, jsbuilder_->GetUInt32(), jsbuilder_->GetPrimType(node->ptyp), node);
#else
  BaseNode *expr = jsbuilder_->CreateExprIntrinsicop1(INTRN_JS_INT32, jsbuilder_->GetInt32(), node);
  MIRSymbol *var = CreateTempVar(jsbuilder_->GetInt32());
  return jsbuilder_->CreateExprDread(jsbuilder_->GetInt32(), var);
#endif
}

base_node_t *JSCompiler::CheckConvertToRespectiveType(base_node_t *node, MIRType *ty)
{
  if (ty == NULL || ty == jsvalue_type_)
    return CheckConvertToJSValueType(node);
  if (ty == jsbuilder_->GetPrimType(PTY_u1))
    return CheckConvertToBoolean(node);
  return CheckConvertToInt32(node);
}

// This variable holds a special opcode value which is greater than all normal
// opcodes, and is chosen such that the bitwise or of this value with any
// opcode is this value.
static const jsbytecode EnableInterruptsPseudoOpcode = -1;

#define NOTHANDLED DEBUGPRINTs("--- not yet handled <<<<<<<<<")
#define SIMULATESTACK(nuses, ndefs) \
  do { \
    DEBUGPRINTs("--- not yet handled but stack is simulated <<<<<<<<<"); \
    for (uint32_t i = 0; i < nuses; i++) Pop();  \
    for (uint32_t i = 0; i < ndefs; i++) Push(dummyNode);  \
  }while (0);

void JSCompiler::EnvInit(JSMIRFunction *func) {
  if (!func)
    return;

  if (!func->scope->IsWithEnv())
    return;

  DEBUGPRINT3((func->env_setup));
  if (func->env_setup)
    return;

  base_node_t *bn;
  StmtNode *stmt;

  MIRType * env_type = func->envtype;
  MIRType * env_ptr = func->envptr;
  MIRSymbol *env_var = jsbuilder_->GetOrCreateLocalDecl("environment", jsbuilder_->GetDynany());
  DEBUGPRINTsv3("environment", env_ptr);

  base_node_t *size = jsbuilder_->CreateExprSizeoftype(env_type);
  stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned1(INTRN_JS_NEW, size, env_var);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  func->env_setup = true;

  base_node_t *env = jsbuilder_->CreateExprDread(env_ptr, 0, env_var);
  MIRStructType *env_struct = static_cast<MIRStructType *>(env_type);
  bn = jsbuilder_->GetConstInt(env_struct->fields.size() - 2);
  int idx = jsbuilder_->GetStructFieldIdFromFieldName(env_type, "argnums");
  stmt = jsbuilder_->CreateStmtIassign(env_ptr, idx, env, bn);
  stmt->srcpos.SetLinenum(linenum_);
  jsbuilder_->AddStmtInCurrentFunctionBody(stmt);

  // set up parentenv in env
  if (func->with_env_arg) {
    MIRSymbol *env_arg = jsbuilder_->GetFunctionArgument(func, ENV_POSITION_IN_ARGS);
    bn = jsbuilder_->CreateExprDread(env_ptr, env_arg);
    idx = jsbuilder_->GetStructFieldIdFromFieldName(env_type, "parentenv");
    StmtNode *stmt = jsbuilder_->CreateStmtIassign(env_ptr, idx, env, bn);
    stmt->srcpos.SetLinenum(linenum_);
    jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
  }

#ifdef DYNAMICLANG
  // set up arguments in env

  std::vector<funcArgPair>::iterator I;
  for (I = funcFormals.begin(); I != funcFormals.end(); I++) {
    if (func == (*I).first) {
      std::vector<char *>::iterator IN;
      int i = (func->with_env_arg) ? 2 : 1;
      for (IN = (*I).second.begin(); IN != (*I).second.end(); IN++, i++) {
        MIRSymbol *arg = jsbuilder_->GetFunctionArgument(func, i);
        bn = jsbuilder_->CreateExprDread(arg->GetType(&globaltable), 0, arg);
        uint32_t id = jsbuilder_->GetStructFieldIdFromFieldName(env_type, *IN);
        stmt = jsbuilder_->CreateStmtIassign(env_ptr, id, env, bn);
        stmt->srcpos.SetLinenum(linenum_);
        jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
      }
      break;
    }
  }
#else
  // set up arguments in env
  MIRSymbol *formal = jsbuilder_->GetFunctionArgument(func, FORMAL_POSITION_IN_ARGS);
  BaseNode *base = jsbuilder_->CreateExprDread(jsvalue_ptr_, 0, formal);
  BaseNode *env = jsbuilder_->CreateExprDread(env_ptr, 0, env_var);
  BaseNode *offset;
  BaseNode *addr;
  size = jsbuilder_->CreateExprSizeoftype(jsvalue_type_);

  std::vector<funcArgPair>::iterator I;
  for (I = funcFormals.begin(); I != funcFormals.end(); I++) {
    if (func == (*I).first) {
      int i = 0;
      addr = base;
      std::vector<char *>::iterator IN;
      for (IN = (*I).second.begin(); IN != (*I).second.end(); IN++, i++) {
        if (i) {
          offset = jsbuilder_->CreateExprBinary(OP_mul, jsbuilder_->GetOrCreateVoidPtr(),
                                                jsbuilder_->GetConstInt(i), size);
          addr = jsbuilder_->CreateExprBinary(OP_add,
                                              jsbuilder_->GetOrCreateVoidPtr(),
                                              base,
                                              offset);
        }
        bn = jsbuilder_->CreateExprIread(jsvalue_type_, jsvalue_ptr_, 0, addr);
        uint32_t id = jsbuilder_->GetStructFieldIdFromFieldName(env_type, *IN);
        stmt = jsbuilder_->CreateStmtIassign(env_ptr, id, env, bn);
        stmt->srcpos.SetLinenum(linenum_);
        jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
      }
      break;
    }
  }
#endif

  return;
}

// created labels beforehand to avoid issue of missing labeled target for back edges
bool JSCompiler::MarkLabels(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend) {
  jsbytecode *pc = pcstart;
  while (pc < pcend) {
    JSOp op = JSOp(*pc);
    jsbytecode *target = NULL;
    switch (op) {
      case JSOP_IFEQ:
      case JSOP_IFNE:
      case JSOP_GOTO:
      case JSOP_GOSUB:
        target = pc + GET_JUMP_OFFSET(pc);
        break;
    }

    if (target && label_map_[target] == 0) {
      if (js2mplDebug>0) printf("      0x%x jump target: 0x%x\n", pc, target);
      labidx_t mirlabel;
      if (op == JSOP_GOSUB || op == JSOP_FINALLY)
        mirlabel = GetorCreateLabelofPc(target, "f@");
      else
        mirlabel = GetorCreateLabelofPc(target);
      label_map_[target] = mirlabel;
    }

    pc = js::GetNextPc(pc);
  }
}

// The main entry to convert a js script to mapleir.
bool JSCompiler::CompileScript(JSScript *script) {
  jsbytecode *start = script->code();
  jsbytecode *end = script->codeEnd();

  JSMIRFunction *func = funcstack_.top();
  
  // TODO: is here opstack_ none-empty an error here?
  // to make it work, empty opstack_ now.
  // Check the case of app_inline.js
  while (!opstack_->Empty()) Pop();

  EnvInit(func);

  // mark labels to avoid issue of missing labeled target for back edges
  MarkLabels(script, start, end);

  bool ret = CompileScriptBytecodes(script, start, end, NULL);

  return ret;
}

bool JSCompiler::CompileScriptBytecodes(JSScript *script,
                                       jsbytecode *pcstart, jsbytecode *pcend, jsbytecode **newpc) {
  jsbytecode *pc = pcstart;
  unsigned lastLineNo = 0;
  unsigned lastLinePrinted = 0;
  FILE *srcfileptr = script->sourceObject() ? fopen(script->filename(), "r") : NULL;  // for src line printing
  char linenoText[1040];  // for printing current src line number
  char srcText[1024];   // for content of current src line to be printed

  JSOp lastOp;
  while (pc < pcend) {
    JSOp op = JSOp(*pc); //Convert *pc to JSOP
    unsigned lineNo = js::PCToLineNumber(script, pc);
    linenum_ = lineNo;
    Util::SetIndent(2);
    //DEBUGPRINTnn(lineNo, Util::getOpcodeName[op]);
    if (js2mplDebug>0) printf("  %4d %-25s pc = 0x%x\n", lineNo, Util::getOpcodeName[op], pc);
    if (lastLineNo != lineNo && module_->curfunction != NULL) {
      sprintf(linenoText, "LINE %d: ", lineNo);

      srcText[0] = 0;
      if (lineNo > lastLinePrinted && srcfileptr != NULL) {
        // read and skip source file to start of current line
        for (unsigned i = lastLinePrinted+1; i < lineNo; i++)
          fgets(srcText, sizeof(srcText), srcfileptr);
        fgets(srcText, sizeof(srcText), srcfileptr);  // read current line
        srcText[strlen(srcText)-1] = 0;  // trim away the last \n character
        lastLinePrinted = lineNo;
      }
      //Create Comments node, line no text and src text
      StmtNode *cmntstmt = jsbuilder_->CreateStmtComment(strcat(linenoText, srcText));
      cmntstmt->srcpos.SetLinenum(lineNo);
      jsbuilder_->AddStmtInCurrentFunctionBody(cmntstmt);
      lastLineNo = lineNo;
    }
    Util::SetIndent(4);

    // add endtry node
    EHstruct *eh = eh_->GetEHstruct(0, 0, 0, pc);
    if (eh) {
      labidx_t labidx;
      labidx = eh->label;
      StmtNode *stmt = jsbuilder_->CreateStmtLabel(labidx);
      stmt->srcpos.SetLinenum(lineNo);
      jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
      StmtNode* endtrynode = module_->CurFuncCodeMp()->New<StmtNode>(OP_endtry);
      endtrynode->srcpos.SetLinenum(lineNo);
      jsbuilder_->AddStmtInCurrentFunctionBody(endtrynode);
    }

    if (label_map_[pc] != 0) {
      labidx_t labidx = label_map_[pc];
      if (label_tempvar_map_[labidx] != 0) { 
        // handle the else part of the conditional expression by storing to 
        // the same temp
        //assert(! opstack_->Empty());
        if (! opstack_->Empty()) {
          MIRSymbol *tempvar = label_tempvar_map_[labidx];
          base_node_t *expr = CheckConvertToJSValueType(Pop());
          jsbuilder_->CreateStmtDassign(tempvar, 0, expr, linenum_); 
          if (! opstack_->Empty()) {
            base_node_t *top_value = Top();
            if (!(top_value->op == OP_dread && static_cast<AddrofNode *>(top_value)->stidx.Islocal() &&
               ((addrof_node_t*)top_value)->stidx == tempvar->GetStIdx())) {
              Push(jsbuilder_->CreateExprDread(tempvar->GetType(&globaltable), tempvar));
            } else {
              scope_->DecDepth();
            }
          } else {
            Push(jsbuilder_->CreateExprDread(tempvar->GetType(&globaltable), tempvar));
          }
        }
        label_tempvar_map_[labidx] = 0;  // re-initialize to 0
      }
      StmtNode *stmt = jsbuilder_->CreateStmtLabel(labidx);
      stmt->srcpos.SetLinenum(lineNo);
      jsbuilder_->AddStmtInCurrentFunctionBody(stmt);

      // jump to finally for catch = pc
      eh = eh_->GetEHstruct(0, pc, 0, 0);
      if (eh) {
        StmtNode* catchnode = module_->CurFuncCodeMp()->New<StmtNode>(OP_catch);
        catchnode->srcpos.SetLinenum(lineNo);
        jsbuilder_->AddStmtInCurrentFunctionBody(catchnode);
      }

    }

    switch (op) {
      case EnableInterruptsPseudoOpcode: { NOTHANDLED; break; }
   // case JSOP_NOP: /*OPC, length, stackUse, stackDef*/
      case JSOP_NOP: /*0, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED2: /*2, 1, 1, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED45: /*45, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED46: /*46, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED47: /*47, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED48: /*48, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED49: /*49, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED50: /*50, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED51: /*51, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED52: /*52, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED57: /*57, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED101: /*101, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED102: /*102, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED103: /*103, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED104: /*104, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED105: /*105, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED107: /*107, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED124: /*124, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED125: /*125, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED126: /*126, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED138: /*138, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED139: /*139, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED140: /*140, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED141: /*141, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED142: /*142, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED146: /*146, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED147: /*147, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED148: /*148, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED150: /*150, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED156: /*156, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED157: /*157, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED158: /*158, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED159: /*159, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED161: /*161, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED162: /*162, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED163: /*163, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED164: /*164, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED165: /*165, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED166: /*166, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED167: /*167, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED168: /*168, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED169: /*169, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED170: /*170, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED171: /*171, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED172: /*172, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED173: /*173, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED174: /*174, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED175: /*175, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED176: /*176, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED177: /*177, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED178: /*178, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED179: /*179, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED180: /*180, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED181: /*181, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED182: /*182, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED183: /*183, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED185: /*185, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED186: /*186, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED187: /*187, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED189: /*189, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED190: /*190, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED191: /*191, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED192: /*192, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED196: /*196, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED201: /*201, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED205: /*205, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED206: /*206, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED207: /*207, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED208: /*208, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED209: /*209, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED210: /*210, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED211: /*211, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED212: /*212, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED213: /*213, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED219: /*219, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED220: /*220, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED221: /*221, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED222: /*222, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNUSED223: /*223, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_BACKPATCH: /*149, 5, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_CONDSWITCH: /*120, 1, 0, 0*/  {
        pc= js::GetNextPc(pc);
        continue;
      }
      case JSOP_TRY: /*134, 1, 0, 0*/  { 
        JSTryNote *tn = script->trynotes()->vector;
        JSTryNote *tnlimit = tn + script->trynotes()->length;
        for (; tn < tnlimit; tn++) {
          if ((tn->start + script->mainOffset()) == (pc - script->code() + 1)) {
            jsbytecode *catchpc = pc + 1 + tn->length;
            CompileOpTry(catchpc);
            break;
          }
        }
        break; 
      }
      case JSOP_LOOPHEAD: /*109, 1, 0, 0*/  {
        //jsbytecode *headpc = pc;
        //CompileOpLoopHead(headpc);
        break;
      }
      case JSOP_LABEL: /*106, 5, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_LOOPENTRY: /*227, 2, 0, 0*/  {
        break;
      }
      case JSOP_LINENO: /*119, 3, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_UNDEFINED: /*1, 1, 0, 1*/  {
        BaseNode *undefined = CompileOpConstValue(JSTYPE_UNDEFINED, 0);
        Push(undefined);
        break;
      }
      case JSOP_POP: /*81, 1, 1, 0*/  {
        Pop();
        opstack_->flag_after_throwing = false;
        break;
      }
      case JSOP_POPN: /*11, 3, -1, 0*/  {
        uint32_t n = GET_UINT16(pc);
        DEBUGPRINT2(n);
        //for (uint32_t i = 0; i < n; i++) Pop();
        break;
      }
      case JSOP_DUPAT: /*44, 4, 0, 1*/  {
        uint32_t n = GET_UINT8(pc);
        base_node_t *bn = GetOpAt(n);
        Push(bn);
        break;
      }
      case JSOP_SETRVAL: /*152, 1, 1, 0*/  {
        opstack_->flag_has_rval = true;
        base_node_t *rval = Pop();
        opstack_->rval = rval;
        break;
      }
      case JSOP_ENTERWITH: /*3, 5, 1, 0*/  { SIMULATESTACK(1, 0); break; }
      case JSOP_LEAVEWITH: /*4, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_RETURN: /*5, 1, 1, 0*/  {
        base_node_t *rval = Pop();
        base_node_t *bn = CheckConvertToJSValueType(rval);
        jsbuilder_->CreateStmtReturn(bn, true, linenum_);
        break;
      }
      case JSOP_RETRVAL: /*153, 1, 0, 0*/  {
        if(true == opstack_->flag_has_rval){
          JSMIRFunction *func = jsbuilder_->GetCurrentFunction();
          if (jsbuilder_->IsPluginFunc(func)) {
            // set to return exports anyway
            // BaseNode *id_node = jsbuilder_->GetConstUInt32((uint32_t) JS_BUILTIN_MODULE);
            base_node_t *node1 = CheckConvertToJSValueType(CompileGeneric1(INTRN_JS_GET_BIOBJECT,
                  jsbuilder_->GetConstUInt32((uint32_t) JS_BUILTIN_MODULE), false));
            base_node_t *node2 = CheckConvertToJSValueType(CompileGeneric1(INTRN_JS_GET_BISTRING,
                  jsbuilder_->GetConstUInt32((uint32_t)JSBUILTIN_STRING_EXPORTS), false));
            base_node_t *ret_expr = CompileGeneric2(INTRN_JSOP_GETPROP, node1, node2, true);
            SetupMainFuncRet(ret_expr);
          } else if (func == jsmain_) {
            SetupMainFuncRet(jsbuilder_->GetConstInt(0));  // main function always returns 0
          } else {
            jsbuilder_->CreateStmtReturn(CheckConvertToJSValueType(opstack_->rval), false, linenum_);
          }
          opstack_->flag_has_rval = false;
        }
        else{
          JSMIRFunction *func = jsbuilder_->GetCurrentFunction();
          if (jsbuilder_->IsPluginFunc(func)) {
            // BaseNode *id_node = jsbuilder_->GetConstUInt32((uint32_t) JS_BUILTIN_MODULE);
            base_node_t *node1 = CheckConvertToJSValueType(CompileGeneric1(INTRN_JS_GET_BIOBJECT,
                  jsbuilder_->GetConstUInt32((uint32_t) JS_BUILTIN_MODULE), false));
            base_node_t *node2 = CheckConvertToJSValueType(CompileGeneric1(INTRN_JS_GET_BISTRING,
                  jsbuilder_->GetConstUInt32((uint32_t)JSBUILTIN_STRING_EXPORTS), false));
            base_node_t *ret_expr = CompileGeneric2(INTRN_JSOP_GETPROP, node1, node2, true);
            jsbuilder_->CreateStmtReturn(ret_expr, false, linenum_);
          } else if (func == jsmain_) {
            BaseNode *undefined = CompileOpConstValue(JSTYPE_UNDEFINED, 0);
            jsbuilder_->CreateStmtReturn(undefined, false, linenum_);
          } else {
            jsbuilder_->CreateStmtReturn(jsbuilder_->GetConstInt(0), false, linenum_);
          }
        }
        break;
      }
      case JSOP_DEFAULT: /*122, 5, 1, 0*/  { 
        Pop();
      }
      case JSOP_GOTO: /*6, 5, 0, 0*/  {
        int offset = GET_JUMP_OFFSET(pc);
        if (! opstack_->flag_has_iter && ! opstack_->Empty() && 
            ! opstack_->flag_after_throwing) {
          // in middle of conditional expression; save in a temp and associate
          // the temp with the goto label
          // TODO should not pop from opstack_
          base_node_t *expr = Pop();
          MIRSymbol *tempvar = SymbolFromSavingInATemp(expr, true);
          Push(jsbuilder_->CreateExprDread(tempvar->GetType(&globaltable), tempvar));
          CompileOpGoto(pc, pc + offset, tempvar);
        }
        else CompileOpGoto(pc, pc + offset, NULL);
        break;
      }
      
      case JSOP_IFEQ: /*7, 5, 1, 0*/
      case JSOP_IFNE: /*8, 5, 1, 0*/  {
        int offset = GET_JUMP_OFFSET(pc);
        base_node_t *opnd = Pop();
        base_node_t *cond = CheckConvertToBoolean(opnd);
        CompileOpIfJump(op, cond, pc+offset);
        break;
      }
      
      case JSOP_OR: /*68, 5, 1, 1*/
      case JSOP_AND: /*69, 5, 1, 1*/  {
        int offset = GET_JUMP_OFFSET(pc);
        base_node_t *opnd0 = CheckConvertToJSValueType(Pop());  //Pop IFEQ stmt
        base_node_t *cond0 = CheckConvertToBoolean(opnd0);
        MIRSymbol *temp_var = SymbolFromSavingInATemp(opnd0, true);
        opnd0 = jsbuilder_->CreateExprDread(jsbuilder_->GetUInt1(), temp_var);
        Push(opnd0);

        labidx_t mirlabel = GetorCreateLabelofPc(pc+offset);
        CondGotoNode* gotonode =  jsbuilder_->CreateStmtCondGoto(cond0, (op==JSOP_AND)?OP_brfalse:OP_brtrue, mirlabel);
        jsbuilder_->AddStmtInCurrentFunctionBody(gotonode);
  
        jsbytecode *start = js::GetNextPc(pc);
        pc = pc + offset;
        //Pop(); comment out because CompileScriptBytecodes() has a Pop() which is not listed on command list
        CompileScriptBytecodes(script, start, pc, NULL);
        base_node_t *opnd1 = CheckConvertToJSValueType(Pop());
        jsbuilder_->CreateStmtDassign(temp_var, 0, opnd1, linenum_);
        opnd0 = jsbuilder_->CreateExprDread(temp_var->GetType(&globaltable), temp_var);
        Push(opnd0);
        continue;
      }
      case JSOP_ITER: /*75, 2, 1, 1*/  { 
        uint8_t flags = GET_UINT8(pc);
        base_node_t *bn = CheckConvertToJSValueType(Pop());
        base_node_t *itr = CompileOpNewIterator(bn, flags);
        Push(itr);
        opstack_->flag_has_iter = true;
        break;
      }
      case JSOP_MOREITER: /*76, 1, 1, 2*/  { 
        base_node_t *iterator = Top();
        base_node_t *is_iterator = CompileOpMoreIterator(iterator);
        Push(is_iterator);
        break;
      }
      case JSOP_ITERNEXT: /*77, 1, 0, 1*/  { 
        base_node_t *iterator= Top();
        base_node_t *bn = CompileOpIterNext(iterator);
        Push(bn);
        break;
      }
      case JSOP_ENDITER: /*78, 1, 1, 0*/ {
        Pop();
        opstack_->flag_has_iter = false;
        break;
      }
      case JSOP_DUP: /*12, 1, 1, 2*/  {
        base_node_t *bn = Pop();
        Push(bn);
        Push(GenericCloneTree(module_, bn));
        break;
      }
      case JSOP_DUP2: /*13, 1, 2, 4*/  {
        base_node_t *bn1 = Pop();
        base_node_t *bn2 = Pop();
        Push(bn2);
        Push(bn1);
        Push(GenericCloneTree(module_, bn2));
        Push(GenericCloneTree(module_, bn1));
        break;
      }
      case JSOP_SWAP: /*10, 1, 2, 2*/  {
        base_node_t *bn1 = Pop();
        base_node_t *bn2 = Pop();
        Push(bn1);
        Push(bn2);
        break;
      }
      case JSOP_PICK: /*133, 2, 0, 0*/  {
        uint32_t n = GET_UINT8(pc);
        std::vector <base_node_t *> temp_stack;
        for (uint32_t i = 0; i < n; i++) {
          temp_stack.push_back(Pop());
        }
        base_node_t *bn = Pop();
        for (uint32_t i = 0; i < n; i++) {
          Push(temp_stack[temp_stack.size() - 1]);
          temp_stack.pop_back();
        }
        Push(bn);
        break;
      }
      case JSOP_SETCONST: /*14, 5, 1, 1*/  { SIMULATESTACK(1, 1); break; }
      case JSOP_BINDINTRINSIC: /*145, 5, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_BINDGNAME: /*214, 5, 0, 1*/  {
      case JSOP_BINDNAME: /*110, 5, 0, 1*/
        JSAtom *atom = script->getName(pc);
        base_node_t *node = CompileOpBindName(atom);
        Push(node);
        break;
      }
    case JSOP_CASE: /*121, 5, 2, 1*/  { 
      base_node_t *rval = Pop();
      base_node_t *lval = Pop();
      int offset = GET_JUMP_OFFSET(pc);
      CompileOpCase(pc, offset, rval, lval);
      break;
    }
      // Binary operations.
      case JSOP_BITOR: /*15, 1, 2, 1*/
      case JSOP_BITXOR: /*16, 1, 2, 1*/
      case JSOP_BITAND: /*17, 1, 2, 1*/
      case JSOP_EQ: /*18, 1, 2, 1*/
      case JSOP_NE: /*19, 1, 2, 1*/
      case JSOP_LT: /*20, 1, 2, 1*/
      case JSOP_LE: /*21, 1, 2, 1*/
      case JSOP_GT: /*22, 1, 2, 1*/
      case JSOP_GE: /*23, 1, 2, 1*/
      case JSOP_LSH: /*24, 1, 2, 1*/
      case JSOP_RSH: /*25, 1, 2, 1*/
      case JSOP_URSH: /*26, 1, 2, 1*/
      case JSOP_ADD: /*27, 1, 2, 1*/
      case JSOP_SUB: /*28, 1, 2, 1*/
      case JSOP_MUL: /*29, 1, 2, 1*/
      case JSOP_DIV: /*30, 1, 2, 1*/
      case JSOP_MOD: /*31, 1, 2, 1*/
      case JSOP_STRICTEQ: /*72, 1, 2, 1*/
      case JSOP_STRICTNE: /*73, 1, 2, 1*/
      case JSOP_INSTANCEOF: /*114, 1, 2, 1*/
      case JSOP_IN: /*113, 1, 2, 1*/  {
        base_node_t *opnd1 = Pop();
        base_node_t *opnd0 = Pop();
        base_node_t *result = CompileOpBinary(op, opnd0, opnd1);
        Push(result);
        break;
      }
      case JSOP_NOT: /*32, 1, 1, 1*/
      case JSOP_BITNOT: /*33, 1, 1, 1*/
      case JSOP_NEG: /*34, 1, 1, 1*/
      case JSOP_POS: /*35, 1, 1, 1*/  {
        base_node_t *opnd = Pop();
        base_node_t *result = CompileOpUnary(op, opnd);
        Push(result);
        break;
      }
      case JSOP_DELNAME: /*36, 5, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_DELPROP: /*37, 5, 1, 1*/  {
        JSString *str = script->getAtom(pc);
        base_node_t *nameIndex = CheckConvertToJSValueType(CompileOpString(str));
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        base_node_t *res = CompileGeneric2(INTRN_JSOP_DELPROP, obj, nameIndex, true);
        Push(res);
        break;
      }
      case JSOP_DELELEM: /*38, 1, 2, 1*/  {
        base_node_t *index = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        index = CheckConvertToJSValueType(index);
        base_node_t *bn = CompileGeneric2(INTRN_JSOP_DELPROP, obj, index,  true);
        Push(bn);
        break;
      }
      case JSOP_TOID: /*225, 1, 1, 1*/  { SIMULATESTACK(1, 1); break; }
      case JSOP_TYPEOFEXPR: /*197, 1, 1, 1*/
      case JSOP_TYPEOF: /*39, 1, 1, 1*/  {
        base_node_t *opnd = Pop();
        base_node_t *result = CompileOpUnary(JSOP_TYPEOF, opnd);
        Push(result);
        break;
      }
      case JSOP_VOID: /*40, 1, 1, 1*/  {
        (void)Pop();
        Push(CompileOpConstValue(JSTYPE_UNDEFINED, 0));
        break;
      }
      case JSOP_THIS: /*65, 1, 0, 1*/  {
        base_node_t *bn = CompileGeneric0(INTRN_JSOP_THIS, false);
        Push(bn);
        break;
      }
      case JSOP_GETPROP: /*53, 5, 1, 1*/
      case JSOP_CALLPROP: /*184, 5, 1, 1*/  {
        JSString *str = script->getAtom(pc);
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        base_node_t *name = CompileOpString(str);
        base_node_t *val = CompileGeneric2(INTRN_JSOP_GETPROP_BY_NAME, obj, name, true);
        Push(val);
        break;
      }
      case JSOP_GETXPROP: /*195, 5, 1, 1*/  {
        // See BytecodeEmitter.cpp:3678, JSOP_GETXPROP seems like JSOP_NAME.
        // Actually I think it is a bug in SpiderMonkey.
        Pop();
        JSAtom *atom = script->getAtom(GET_UINT32_INDEX(pc));
        base_node_t *bn = CompileOpName(atom, pc);
        Push(bn);
        break;
      }
      case JSOP_LENGTH: /*217, 5, 1, 1*/  {
        base_node_t *array = CheckConvertToJSValueType(Pop());
        base_node_t *length = CompileGeneric1(INTRN_JSOP_LENGTH, array, false);
        Push(length);
        break;
      }
      case JSOP_SETINTRINSIC: /*144, 5, 2, 1*/  { SIMULATESTACK(2, 1); break; }
      case JSOP_SETGNAME: /*155, 5, 2, 1*/  {
      case JSOP_SETNAME: /*111, 5, 2, 1*/
        JSAtom *atom = script->getName(pc);
        base_node_t *val = Pop();
        Pop(); // pop the scope
        if (!CompileOpSetName(atom, val))
          return false;
        break;
      }
      case JSOP_GETELEM: /*55, 1, 2, 1*/  {
        base_node_t *index = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        index = CheckConvertToJSValueType(index);
        base_node_t *elem = CompileGeneric2(INTRN_JSOP_GETPROP, obj, index, true);
        Push(elem);
        break;
      }
      case JSOP_CALLELEM: /*193, 1, 2, 1*/  { SIMULATESTACK(2, 1); break; }
      case JSOP_INITELEM: /*94, 1, 3, 1*/
      case JSOP_SETELEM: /*56, 1, 3, 1*/  {
        base_node_t *val = Pop();
        base_node_t *index = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        index = CheckConvertToJSValueType(index);
        StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_SETPROP, obj,
                                                              index,
                                                              CheckConvertToJSValueType(val), NULL);
        stmt->srcpos.SetLinenum(lineNo);
        jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
        Push(obj);
        break;
      }
      case JSOP_EVAL: /*123, 3, -1, 1*/  { assert("false && Can not support eval!"); SIMULATESTACK(2 + GET_ARGC(pc), 1); break; }
      case JSOP_SPREADNEW: /*42, 1, 3, 1*/  { SIMULATESTACK(3, 1); break; }
      case JSOP_SPREADCALL: /*41, 1, 3, 1*/  { SIMULATESTACK(3, 1); break; }
      case JSOP_SPREADEVAL: /*43, 1, 3, 1*/  { SIMULATESTACK(3, 1); break; }
      case JSOP_NEW: /*82, 3, -1, 1*/ {
        uint32_t argc = GET_ARGC(pc);
        base_node_t *bn = CompileOpNew(argc);
        Push(bn);
        break;
      }
      case JSOP_FUNAPPLY: /*79, 3, -1, 1*/
      case JSOP_FUNCALL: /*108, 3, -1, 1*/
      case JSOP_CALL: /*58, 3, -1, 1*/  {
        uint32_t argc = GET_ARGC(pc);
        base_node_t *bn = CompileOpCall(argc);
        Push(bn);
        break;
      }
      case JSOP_SETCALL: /*74, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_IMPLICITTHIS: /*226, 5, 0, 1*/  {
        base_node_t *bn = CompileOpConstValue(JSTYPE_UNDEFINED, 0);
        Push(bn);
        break;
      }
      case JSOP_GETGNAME: /*154, 5, 0, 1*/
      case JSOP_NAME: /*59, 5, 0, 1*/  {
        JSAtom *atom = script->getAtom(GET_UINT32_INDEX(pc));
        base_node_t *bn = CompileOpName(atom, pc);
        Push(bn);
        break;
      }
      case JSOP_GETINTRINSIC: /*143, 5, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_UINT16: /*88, 3, 0, 1*/ {
        unsigned uval = GET_UINT16(pc);
        BaseNode *bn = CompileOpConstValue(JSTYPE_NUMBER, uval);
        DEBUGPRINT2(uval);
        Push(bn);
        break;
      }
      case JSOP_UINT24: /*188, 4, 0, 1*/ {
        unsigned uval = GET_UINT24(pc);
        BaseNode *bn = CompileOpConstValue(JSTYPE_NUMBER, uval);
        DEBUGPRINT2(uval);
        Push(bn);
        break;
      }
      case JSOP_INT8: /*215, 2, 0, 1*/ {
        int ival = GET_INT8(pc);
        BaseNode *bn = CompileOpConstValue(JSTYPE_NUMBER, ival);
        DEBUGPRINT2(ival);
        Push(bn);
        break;
      }
      case JSOP_INT32: /*216, 5, 0, 1*/ {
        int ival = GET_INT32(pc);
        BaseNode *bn = CompileOpConstValue(JSTYPE_NUMBER, ival);
        DEBUGPRINT2(ival);
        Push(bn);
        break;
      }
      case JSOP_DOUBLE: /*60, 5, 0, 1*/  {
        double dval = script->getConst(GET_UINT32_INDEX(pc)).toDouble();
        assert(false && "Can not support double.");
        SIMULATESTACK(0, 1);
        break;
      }
      case JSOP_STRING: /*61, 5, 0, 1*/  {
        JSString *str = script->getAtom(pc);
        base_node_t *bn = CompileOpString(str);
        Push(bn);
        break;
      }
      case JSOP_OBJECT: /*80, 5, 0, 1*/  { SIMULATESTACK(0, 1);  break; }
      case JSOP_REGEXP: /*160, 5, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_ZERO: /*62, 1, 0, 1*/  {
        BaseNode *bn = CompileOpConstValue(JSTYPE_NUMBER, 0);
        Push(bn);
        break;
      }
      case JSOP_ONE: /*63, 1, 0, 1*/  {
        BaseNode *bn = CompileOpConstValue(JSTYPE_NUMBER, 1);
        Push(bn);
        break;
      }
      case JSOP_NULL: /*64, 1, 0, 1*/  {
        BaseNode *bn = CompileOpConstValue(JSTYPE_NULL, 0);
        Push(bn);
        break;
      }
      case JSOP_FALSE: /*66, 1, 0, 1*/  {
        BaseNode *bn = CompileOpConstValue(JSTYPE_BOOLEAN, 0);
        Push(bn);
        break;
      }
      case JSOP_TRUE: /*67, 1, 0, 1*/  {
        BaseNode *bn = CompileOpConstValue(JSTYPE_BOOLEAN, 1);
        Push(bn);
        break;
      }
      case JSOP_TABLESWITCH: /*70, -1, 1, 0*/  {
        int32_t len = GET_JUMP_OFFSET(pc);
        base_node_t *opnd = Pop();
        CompileOpTableSwitch(opnd, len, script, pc);
        break;
      }
      case JSOP_ARGUMENTS: /*9, 1, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_RUNONCE: /*71, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_REST: /*224, 1, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_GETALIASEDVAR: /*136, 5, 0, 1*/  {
        JSAtom * atom = ScopeCoordinateName(
            jscontext_->runtime()->scopeCoordinateNameCache, script, pc);
        DEBUGPRINT2(atom);
        base_node_t *bn = CompileOpGetAliasedVar(atom);
        Push(bn);
        break;
      }
      case JSOP_SETALIASEDVAR: /*137, 5, 1, 1*/  {
        base_node_t *val = CheckConvertToJSValueType(Pop());
        JSAtom * atom = ScopeCoordinateName(
            jscontext_->runtime()->scopeCoordinateNameCache, script, pc);
        base_node_t *bn = CompileOpSetAliasedVar(atom, val);
        Push(val);
        break;
      }
      case JSOP_GETARG: /*84, 3, 0, 1*/  {
        uint32_t i = GET_ARGNO(pc);
        base_node_t *bn = CompileOpGetArg(i);
        Push(bn);
        break;
      }
      case JSOP_SETARG: /*85, 3, 1, 1*/  {
        uint32_t i = GET_ARGNO(pc);
        base_node_t *bn = CheckConvertToJSValueType(Pop());
        CompileOpSetArg(i, bn);
        break;
      }
      case JSOP_GETLOCAL: /*86, 4, 0, 1*/  {
        uint32_t i = GET_LOCALNO(pc);
        base_node_t *bn = CompileOpGetLocal(i);
        Push(bn);
        break;
      }
      case JSOP_SETLOCAL: /*87, 4, 1, 1*/  {
        uint32_t i = GET_LOCALNO(pc);
        base_node_t *src = CheckConvertToJSValueType(Pop());
        base_node_t *bn = CompileOpSetLocal(i, src);
        Push(src);
        break;
      }
      case JSOP_NEWINIT: /*89, 5, 0, 1*/  {
        // Array or Object.
        uint8_t i = GET_UINT8(pc);
        base_node_t *obj = CompileOpNewInit(i);
        Push(obj);
        break;
      }
      case JSOP_NEWARRAY: /*90, 4, 0, 1*/  {
        uint32_t length = GET_UINT24(pc);
        BaseNode *len = jsbuilder_->GetConstUInt32(length);
#if 1
        base_node_t *arr = CompileGeneric1(INTRN_JS_NEW_ARR_LENGTH, CheckConvertToJSValueType(len), true);
        Push(arr);
        break;
#else
        std::stack<base_node_t *> tmp_stack;
        pc = js::GetNextPc(pc);
        op = JSOp(*pc);
        jsbytecode *new_pc;
        while (op != JSOP_ENDINIT) {
          if (op != JSOP_INITELEM_ARRAY) {
            if (op == JSOP_NEWARRAY) {
              CompileScriptBytecodes (script, pc, js::GetNextPc(pc), &new_pc);
              pc = new_pc;
            } else {
                CompileScriptBytecodes (script, pc, js::GetNextPc(pc), NULL);
            }
          } else {
              base_node_t *init = Pop();
              tmp_stack.push (init);
          }
          pc = js::GetNextPc(pc);
          op = JSOp(*pc);
        }
        if (newpc)
          *newpc = pc;

        if (length == 0) {
          base_node_t *arr = CompileGeneric1(INTRN_JS_NEW_ARR_LENGTH, CheckConvertToJSValueType(CompileOpConstValue(JSTYPE_NUMBER, 0)), true);
          Push(arr);
          break;
        }
        MIRSymbol *arguments = NULL;
        arguments = jsbuilder_->GetCurrentFunction()->symtab->CreateSymbol();
        const char *temp_name = Util::GetSequentialName("js_arguments_", temp_var_no_, mp_);
        std::string argname(temp_name, mp_);
        arguments->SetNameStridx(module_->stringtable.GetOrCreateStridxFromName(argname));
        jsbuilder_->GetCurrentFunction()->symtab->AddToStringSymbolMap(arguments);
        arguments->sclass = SC_auto;
        arguments->skind  = ST_var;

        uint32_t size_array[1];
        size_array[0] = length;
        MIRType *array_type = jsbuilder_->GetOrCreateArrayType(jsvalue_type_, 1, size_array);
        MIRType *array_ptr_type = jsbuilder_->GetOrCreatePointerType(array_type);
        tyidx_t tyidx = array_type->_ty_idx;
        arguments->SetTyIdx(tyidx);
        base_node_t *bn;
        MIRType *pargtype = jsbuilder_->GetOrCreatePointerType(arguments->GetType());
        base_node_t *addr_base = jsbuilder_->CreateExprAddrof(0, arguments);

        for (uint32_t i = 0; i < length; i++) {
          bn = CheckConvertToJSValueType(tmp_stack.top());
          DEBUGPRINT3(bn->op);
          tmp_stack.pop();
          MapleVector<base_node_t *> opnds(module_.CurFuncCodeMpAllocator()->Adapter());
          opnds.push_back(addr_base);
          BaseNode *addr_offset = jsbuilder_->GetConstInt(length - i - 1);
          opnds.push_back(addr_offset);
          BaseNode *array_expr = jsbuilder_->CreateExprArray(array_type, opnds);
          BaseNode *stmt = jsbuilder_->CreateStmtIassign(array_ptr_type, 0, array_expr, bn);
          jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
        }

        base_node_t *arr = CompileGeneric2(INTRN_JS_NEW_ARR_ELEMS, addr_base, len, true);
        Push(arr);
        break;
#endif
      }
      case JSOP_NEWOBJECT: /*91, 5, 0, 1*/  { SIMULATESTACK(0, 1); break; }
      case JSOP_ENDINIT: /*92, 1, 0, 0*/  {
        // Do nothing.
        //JS_ASSERT(REGS.stackDepth() >= 1);
        //JS_ASSERT(REGS.sp[-1].isObject() || REGS.sp[-1].isUndefined());
        break;
      }
      case JSOP_DEFCONST: /*128, 5, 0, 0*/  {
        DEBUGPRINT2(JSOP_DEFCONST);
        break;
      }
      case JSOP_DEFVAR: /*129, 5, 0, 0*/  {
        DEBUGPRINT2(JSOP_DEFVAR);
        JSAtom * atom = script->getAtom(GET_UINT32_INDEX(pc));
        CompileOpDefVar(atom);
        break;
      }
      case JSOP_DEFFUN: /*127, 5, 0, 0*/  {
        DEBUGPRINT2(JSOP_DEFFUN);
        JSFunction *jsfun = script->getFunction(GET_UINT32_INDEX(pc));
        CompileOpDefFun(jsfun);
        break;
      }
      case JSOP_LAMBDA: /*130, 5, 0, 1*/  {
        JSFunction *jsfun = script->getFunction(GET_UINT32_INDEX(pc));
        base_node_t * bn = CompileOpLambda(pc, jsfun);
        Push(bn);
        break;
      }
      case JSOP_LAMBDA_ARROW: /*131, 5, 1, 1*/  { SIMULATESTACK(1, 1); break; }
      case JSOP_CALLEE: /*132, 1, 0, 1*/  {
        char *name = funcstack_.top()->scope->GetName();
        char * objname = Util::GetNameWithSuffix(name, "_obj_", mp_);
        MIRSymbol *var = jsbuilder_->GetOrCreateGlobalDecl(objname, jsvalue_type_);
        base_node_t *bn = jsbuilder_->CreateExprDread(var);
        Push(bn);
        break;
      }
      case JSOP_INITPROP_GETTER: /*97, 5, 2, 1*/  {
        JSString *str = script->getAtom(pc);
        DEBUGPRINT2(str);
        base_node_t *val = CheckConvertToJSValueType(Pop());
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        if (!CompileOpInitPropGetter(obj, str, val))
          return false;
        Push(obj);
        break;
      }
      case JSOP_INITPROP_SETTER: /*98, 5, 2, 1*/  { 
        JSString *str = script->getAtom(pc);
        DEBUGPRINT2(str);
        base_node_t *val = CheckConvertToJSValueType(Pop());
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        if (!CompileOpInitPropSetter(obj, str, val))
          return false;
        Push(obj);
        break;
      }
      case JSOP_INITELEM_GETTER: /*99, 1, 3, 1*/  {
        base_node_t *val = CheckConvertToJSValueType(Pop());
        base_node_t *id = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        if (!CompileOpInitElemGetter(obj, id, val))
          return false;
        Push(obj);
        break;
      }
      case JSOP_INITELEM_SETTER: /*100, 1, 3, 1*/  {
        base_node_t *val = CheckConvertToJSValueType(Pop());
        base_node_t *id = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        if (!CompileOpInitElemSetter(obj, id, val))
          return false;
        Push(obj);
        break;
      }
      case JSOP_HOLE: /*218, 1, 0, 1*/  {
        BaseNode *hole_elem = CompileOpConstValue(JSTYPE_NONE, 0);
        Push(hole_elem);
        break;
      }
      case JSOP_MUTATEPROTO: /*194, 1, 2, 1*/  { SIMULATESTACK(2, 1); break; }
      case JSOP_SETPROP: /*54, 5, 2, 1*/ {
        JSString *str = script->getAtom(pc);
        DEBUGPRINT2(str);
        base_node_t *val = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        if (!CompileOpSetProp(obj, str, val))
          return false;
        Push(val);
        break;
      }
      case JSOP_INITPROP: /*93, 5, 2, 1*/  {
        JSString *str = script->getAtom(pc);
        DEBUGPRINT2(str);
        base_node_t *val = Pop();
        base_node_t *obj = CheckConvertToJSValueType(Pop());
        base_node_t *name = CompileOpString(str);
        StmtNode *stmt = jsbuilder_->CreateStmtIntrinsicCallAssigned3(INTRN_JSOP_INITPROP_BY_NAME, obj,
                                                                 name,
                                                                 CheckConvertToJSValueType(val), NULL);
        stmt->srcpos.SetLinenum(lineNo);
        jsbuilder_->AddStmtInCurrentFunctionBody(stmt);
        Push(obj);
        break;
      }
      case JSOP_INITELEM_ARRAY: /*96, 4, 2, 1*/  {
        base_node_t *init = Pop();
        base_node_t *arr = CheckConvertToJSValueType(Pop());
        BaseNode *index = CompileOpConstValue(JSTYPE_NUMBER, (int32_t)GET_UINT24(pc));
        if (!CompileOpSetElem(arr, index, init))
          return false;
        Push(arr);
        break;
      }
      case JSOP_INITELEM_INC: /*95, 1, 3, 2*/  { SIMULATESTACK(3, 2); break; }
      case JSOP_SPREAD: /*83, 1, 3, 2*/  { SIMULATESTACK(3, 2); break; }
      case JSOP_GOSUB: /*116, 5, 0, 0*/  {
        int offset = GET_JUMP_OFFSET(pc);
        CompileOpGosub(pc + offset);
        break;
      }
      case JSOP_RETSUB: /*117, 1, 2, 0*/  { 
        base_node_t *lval = Pop();
        base_node_t *rval = Pop();
        StmtNode* retsub = module_->CurFuncCodeMp()->New<StmtNode>(OP_retsub);
        retsub->srcpos.SetLinenum(lineNo);
        jsbuilder_->AddStmtInCurrentFunctionBody(retsub);
        break;
        }
      case JSOP_EXCEPTION: /*118, 1, 0, 1*/  {
        base_node_t *rr = jsbuilder_->CreateExprRegread(PTY_dynany, -SREG_thrownval);
        Push(rr);
        break; 
        }
      case JSOP_FINALLY: /*135, 1, 0, 2*/  {
        StmtNode* finally = module_->CurFuncCodeMp()->New<StmtNode>(OP_finally);
        finally->srcpos.SetLinenum(lineNo);
        jsbuilder_->AddStmtInCurrentFunctionBody(finally);
        // TODO: need to Push two entries onto stack.  false, (next bytecode's PC)
        base_node_t *bval = CompileOpConstValue(JSTYPE_BOOLEAN, 0);
        base_node_t *tval = CompileOpConstValue(JSTYPE_NUMBER, GET_UINT32_INDEX(js::GetNextPc(pc)));
        Push(tval);
        Push(bval);
        break;
        }
      case JSOP_THROWING: /*151, 1, 1, 0*/
        opstack_->flag_after_throwing = true;
        SIMULATESTACK(1, 0);
        break;
      case JSOP_THROW: /*112, 1, 1, 0*/  { 
        base_node_t *rval = Pop();
        StmtNode *throwstmt = jsbuilder_->CreateStmtThrow(CheckConvertToJSValueType(rval));
        throwstmt->srcpos.SetLinenum(lineNo);
        jsbuilder_->AddStmtInCurrentFunctionBody(throwstmt);
        break;
        }
      case JSOP_DEBUGGER: /*115, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_PUSHBLOCKSCOPE: /*198, 5, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_POPBLOCKSCOPE: /*199, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_DEBUGLEAVEBLOCK: /*200, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_GENERATOR: /*202, 1, 0, 0*/  { NOTHANDLED; break; }
      case JSOP_YIELD: /*203, 1, 1, 1*/  { SIMULATESTACK(1, 1); break; }
      case JSOP_ARRAYPUSH: /*204, 1, 2, 0*/  { SIMULATESTACK(2, 0); break; }
      default: {
        return false;
      }
    }  // End switch (op)

    lastOp = op;
    pc = js::GetNextPc(pc);
  }  // End while (pc < script->codeEnd())

  if (lastOp == JSOP_RETRVAL)
    CloseFuncBookKeeping();

  return true;
}  // End JSCompiler::CompileScript(JSScript *script)

}  // namespace mapleir

