/// Copyright [year] <Copyright Owner>
#include <string>
#include "../include/jsmirbuilder.h"

namespace mapleir {
// Create jsvalue_type as the JS::Value from mozjs-31.2.0/js/public/Value.h.
// We only consider littel_endian and 32-bit architectures here.
MIRType *JSMIRBuilder::CreateJSValueType() {
  return GetDynany();
}

JSMIRFunction *JSMIRBuilder::CreateJSMain() {
  ArgVector arguments(module_->mp_allocator_.Adapter());
  JSMIRFunction *jsmain = NULL;
  if (IsPlugin()) {
    jsmain = GetOrCreateFunction(GetWrapperName(), GetDynany(), arguments, false);
    SetCurrentFunction(jsmain);
  } else {
    jsmain = GetOrCreateFunction("main", GetInt32(), arguments, false);
    SetCurrentFunction(jsmain);
    IntrinsiccallNode *stmt = CreateStmtIntrinsicCallAssigned0((MIRIntrinsicId)INTRN_JS_INIT_CONTEXT, NULL);
    AddStmtInCurrentFunctionBody(stmt);
  }
  return jsmain;
}

void JSMIRBuilder::InitBuiltinMethod() {
#define DEFBUILTINMETHOD(name, intrn_code, need_this) \
#name,
  const char *name[18] = {
    #include "../include/builtinmethod.def"
  };
#undef DEFBUILTINMETHOD

  for (uint32_t i = 0; i < sizeof (name) / sizeof (const char *); i++) {
    if (!name[i]) return;
    MIRSymbol *st = module_->symtab->CreateSymbol(SCOPE_LOCAL + 1);
    st->SetNameStridx(GetOrCreateStringIndex(name[i]));
    if (!module_->symtab->AddToStringSymbolMap(st))
      return;
    st->sclass = SC_text;
    st->skind = ST_func;
    InsertGlobalName((char *)name[i]);
  }
}

void JSMIRBuilder::Init() {
  jsvalue_type_ = CreateJSValueType();
  jsvalue_ptr_ = GetOrCreatePointerType(jsvalue_type_);

  InitGlobalName();
  // InitBuiltinMethod();
  // Now we create the main function as jsmain.
  // If the script is called by another program, the name should be jsmain.
  jsmain_ = CreateJSMain();
  char *name = "main";
  if (IsPlugin())
    name = GetWrapperName();
  InsertGlobalName(name);
}

JSMIRFunction *JSMIRBuilder::GetFunction(const char *name) {
  JSMIRFunction *fn = GetFunc(name);
  if (!fn) 
    assert(false && "function is not created");
  return fn;
}

JSMIRFunction *JSMIRBuilder::GetOrCreateFunction(const char *name,
                                                 MIRType *return_type,
                                                 ArgVector arguments,
                                                 bool isvarg) {
  DEBUGPRINTsv2("GetOrCreateFunction", name);
  JSMIRFunction *fn = GetFunc(name);
  if (fn) {
    DEBUGPRINTsv2("function is alread created: ", name);
    return fn;
  }

  MapleString fname(name, module_->mp_);
  MIRSymbol *funcst = module_->symtab->CreateSymbol(SCOPE_LOCAL + 1);
  stridx_t stridx = GetOrCreateStringIndex(fname);
  stidx_t stidx = module_->symtab->GetStidxFromStridx(stridx);
  DEBUGPRINT3(fname);
  DEBUGPRINT3(stridx.idx);
  DEBUGPRINT3(stidx);

  funcst->SetNameStridx(stridx);
  if (!module_->symtab->AddToStringSymbolMap(funcst))
    return NULL;
  funcst->sclass = SC_text;
  funcst->skind = ST_func;

  fn =  MP_NEW(module_->mp_, JSMIRFunction(module_, funcst->GetStIdx()));
  fn->Init();
  fn->puidx = module_->functable.size();
  module_->functable.push_back(fn);

  fn->_return_tyidx = return_type->_ty_idx;

  MapleVector<tyidx_t> funcvectype(module_->mp_allocator_.Adapter());
  MapleVector<TypeAttrs> funcvecattr(module_->mp_allocator_.Adapter());
  for (uint32 i = 0; i < arguments.size(); i++) {
    MapleString var(arguments[i].first, module_->mp_);
    MIRSymbol *argst = fn->symtab->CreateSymbol(SCOPE_LOCAL);
    argst->SetNameStridx(GetOrCreateStringIndex(var));
    MIRType *ty = arguments[i].second;
    argst->SetTyIdx(ty->_ty_idx);
    argst->sclass = SC_formal;
    argst->skind = ST_var;
    fn->symtab->AddToStringSymbolMap(argst);
    fn->AddArgument(argst);
    funcvectype.push_back(ty->_ty_idx);
    funcvecattr.push_back(TypeAttrs());
  }
  funcst->SetTyIdx(GetOrCreateFunctionType (return_type->_ty_idx, funcvectype, funcvecattr, isvarg)->_ty_idx);
  funcst->SetFunction(fn);
  fn->body = fn->code_mp->New<BlockNode>();

  AddNameFunc(name, fn);

  DEBUGPRINTsv2("function created: ", name);
  DEBUGPRINT2(fn);
  return fn;
}

void JSMIRBuilder::AddStmtInCurrentFunctionBody(stmt_node_t *n) {
  MIRBuilder::AddStmtInCurrentFunctionBody(n);
  DEBUGPRINTnode(n);
}

NaryStmtNode *JSMIRBuilder::CreateStmtReturn(base_node_t *rval, bool adj_type) {
  NaryStmtNode *stmt = MIRBuilder::CreateStmtReturn(rval);
  AddStmtInCurrentFunctionBody(stmt);

  JSMIRFunction *func = GetCurrentFunction();
  if (adj_type && !IsMain(func) && rval->op == OP_dread) {
    DEBUGPRINTsv2("modify _return_type", (rval->op));
    AddrofNode *dn = (AddrofNode *)rval;
    stidx_t stidx = dn->stidx;
    MIRSymbol *var = module_->GetStFromCurFuncOrMd(stidx);
    MIRType *type = var->GetType(&gtypetable);
    DEBUGPRINT3(type);
    int fid = dn->fieldid;
    if (fid) {
      MIRStructType *Stype = static_cast<MIRStructType *>(type);
      // fieldid in a structure starts from 1 while type vector starts from 0
      type = Stype->GetElemType(&gtypetable, fid - 1);
    }
  }
  return stmt;
}

void JSMIRBuilder::UpdateFunction(JSMIRFunction *func,
                                  MIRType *return_type,
                                  ArgVector arguments) {
  if (return_type) {
    func->_return_tyidx = return_type->_ty_idx;
  }

  for (uint32 i = 0; i < arguments.size(); i++) {
    MapleString var(arguments[i].first, module_->mp_);
    MIRSymbol *st = func->symtab->CreateSymbol(SCOPE_LOCAL);
    st->SetNameStridx(GetOrCreateStringIndex(var));
    MIRType *ty = arguments[i].second;
    st->SetTyIdx(ty->_ty_idx);
    st->sclass = SC_formal;
    st->skind = ST_var;
    func->symtab->AddToStringSymbolMap(st);
    func->AddArgument(st);
  }
}

void JSMIRBuilder::SaveReturnValue(MIRSymbol *var) {
  DEBUGPRINT4("in SaveReturnValue")

  base_node_t *bn = CreateExprRegread(gtypetable.type_table_[var->GetTyIdx()]->GetPrimType(), -SREG_retval0);
  StmtNode *stmt = CreateStmtDassign(var, 0, bn);
  MIRBuilder::AddStmtInCurrentFunctionBody(stmt);
}

StmtNode *JSMIRBuilder::CreateStmtDassign(MIRSymbol *symbol,
                                          uint32_t field_id,
                                          base_node_t *src) {
  DEBUGPRINT4("in CreateStmtDassign")

  StmtNode *stmt = MIRBuilder::CreateStmtDassign(symbol, field_id, src);
  AddStmtInCurrentFunctionBody(stmt);
  return stmt;
}

IntrinsiccallNode *JSMIRBuilder::CreateStmtIntrinsicCall1N(
      MIRIntrinsicId idx, base_node_t *arg0, MapleVector<base_node_t *> &args) {
  IntrinsiccallNode *stmt = MP_NEW(module_->CurFuncCodeMp(), IntrinsiccallNode(module_, OP_intrinsiccall));
  MapleVector<base_node_t *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  arguments.push_back(arg0);
  for (int i=0; i<args.size(); i++)
    arguments.push_back(args[i]);
  stmt->intrinsic = idx;
  stmt->nopnd = arguments;
  return stmt;
}

}  // namespace mapleir
