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
  JSMIRFunction *jsmain = GetOrCreateFunction("main", GetInt32(), arguments, false);
  BaseNode *stmt = CreateStmtIntrinsicCall0((MIRIntrinsicId)INTRN_JS_INIT_CONTEXT);
  SetCurrentFunction(jsmain);
  AddStmtInCurrentFunctionBody(stmt);
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
    MIRSymbol *st = module_->symtab->CreateSymbol();
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
  InsertGlobalName("main");

  SetMain(jsmain_);
}

JSMIRFunction *JSMIRBuilder::GetFunction(const char *name) {
  JSMIRFunction *fn = GetNameFunc(name);
  if (!fn) 
    assert(false && "function is not created");
  return fn;
}

JSMIRFunction *JSMIRBuilder::GetOrCreateFunction(const char *name,
                                                 MIRType *return_type,
                                                 ArgVector arguments,
                                                 bool isvarg) {
  DEBUGPRINTsv2("GetOrCreateFunction", name);
  JSMIRFunction *fn = GetNameFunc(name);
  if (fn) {
    DEBUGPRINTsv2("function is alread created: ", name);
    return fn;
  }

  MapleString fname(name, module_->mp_);
  MIRSymbol *funcst = module_->symtab->CreateSymbol();
  stridx_t stridx = GetOrCreateStringIndex(fname);
  stidx_t stidx = module_->symtab->GetStidxFromStridx(stridx);
  DEBUGPRINT3(fname);
  DEBUGPRINT3(stridx);
  DEBUGPRINT3(stidx);

  funcst->SetNameStridx(stridx);
  if (!module_->symtab->AddToStringSymbolMap(funcst))
    return NULL;
  funcst->sclass = SC_text;
  funcst->skind = ST_func;

  fn =  MP_NEW(module_->mp_, JSMIRFunction(funcst->GetStIdx()));
  fn->Init();
  fn->puidx = module_->functable.size();
  module_->functable.push_back(fn);

  fn->_return_tyidx = return_type->_ty_idx;

  MapleVector<tyidx_t> funcvectype(module_->mp_allocator_.Adapter());
  for (uint32 i = 0; i < arguments.size(); i++) {
    MapleString var(arguments[i].first, module_->mp_);
    MIRSymbol *argst = fn->symtab->CreateSymbol();
    argst->SetNameStridx(GetOrCreateStringIndex(var));
    MIRType *ty = arguments[i].second;
    argst->SetTyIdx(ty->_ty_idx);
    argst->sclass = SC_formal;
    argst->skind = ST_var;
    fn->symtab->AddToStringSymbolMap(argst);
    fn->AddArgument(argst);
    funcvectype.push_back(ty->_ty_idx);
  }
  funcst->SetTyIdx(GetOrCreateFunctionType (return_type->_ty_idx, funcvectype, isvarg)->_ty_idx);
  funcst->SetFunction(fn);
  fn->body = module_->mp_->New<BlockNode>();

  AddNameFunc(name, fn);

  DEBUGPRINTsv2("function created: ", name);
  DEBUGPRINT2(fn);
  return fn;
}

void JSMIRBuilder::AddStmtInCurrentFunctionBody(BaseNode *n) {
  MIRBuilder::AddStmtInCurrentFunctionBody(n);
  DEBUGPRINTnode(n);
}

BaseNode *JSMIRBuilder::CreateStmtReturn(BaseNode *rval, bool adj_type) {
  BaseNode *stmt = MIRBuilder::CreateStmtReturn(rval);
  AddStmtInCurrentFunctionBody(stmt);

  JSMIRFunction *func = GetCurrentFunction();
  if (adj_type && !IsMain(func) && rval->op == OP_dread) {
    DEBUGPRINTsv2("modify _return_type", (rval->op));
    DreadNode *dn = (DreadNode *)rval;
    stidx_t stidx = dn->stidx;
    MIRSymbol *var;
    if (dn->islocal) {
      var = func->symtab->GetSymbolFromStidx(stidx);
    } else {
      var = module_->symtab->GetSymbolFromStidx(stidx);
    }
    MIRType *type = var->GetType();
    DEBUGPRINT3(type);
    int fid = dn->fieldid;
    if (fid) {
      MIRStructType *Stype = static_cast<MIRStructType *>(type);
      // fieldid in a structure starts from 1 while type vector starts from 0
      type = Stype->GetElemType(fid - 1);
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
    MIRSymbol *st = func->symtab->CreateSymbol();
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

  BaseNode *bn = CreateExprRegread(module_->type_table_[var->GetTyIdx()]->GetPrimType(), -SREG_retval0);
  BaseNode *stmt = CreateStmtDassign(var, 0, bn);
  MIRBuilder::AddStmtInCurrentFunctionBody(stmt);
}

BaseNode *JSMIRBuilder::CreateStmtDassign(MIRSymbol *symbol,
                                          uint32_t field_id,
                                          BaseNode *src) {
  DEBUGPRINT4("in CreateStmtDassign")

  BaseNode *stmt = MIRBuilder::CreateStmtDassign(symbol, field_id, src);
  AddStmtInCurrentFunctionBody(stmt);
  return stmt;
}

BaseNode *JSMIRBuilder::CreateStmtIcall(BaseNode *puptrexp,
                                      MapleVector<BaseNode *> args) {
  IcallNode *stmt = module_->mp_->New<IcallNode>();
  stmt->puptrexp = puptrexp;
  stmt->nopnd = args;
  return stmt;
}

BaseNode *JSMIRBuilder::CreateStmtIntrinsicCall1N(
  MIRIntrinsicId idx, BaseNode *arg0, MapleVector<BaseNode *> &args) {
  IntrinsiccallNode *stmt = module_->mp_->New<IntrinsiccallNode>();
  MapleVector<BaseNode *> arguments(module_->mp_allocator_.Adapter());
  arguments.push_back(arg0);
  for (int i=0; i<args.size(); i++)
    arguments.push_back(args[i]);
  stmt->intrinsic = idx;
  stmt->nopnd = arguments;
  return stmt;
}

}  // namespace mapleir
