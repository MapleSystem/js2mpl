/// Copyright [year] <Copyright Owner>
#include <string>
#include "../include/jsmirbuilder.h"

namespace maple {
// Create jsvalue_type as the JS::Value from mozjs-31.2.0/js/public/Value.h.
// We only consider littel_endian and 32-bit architectures here.
MIRType *JSMIRBuilder::CreateJSValueType() {
  return globaltable.GetDynany();
}

JSMIRFunction *JSMIRBuilder::CreateJSMain() {
  ArgVector arguments(module_->mp_allocator_.Adapter());
  JSMIRFunction *jsmain = NULL;
  if (IsPlugin()) {
    jsmain = GetOrCreateFunction(GetWrapperName(), globaltable.GetDynany(), arguments, false);
    SetCurrentFunction(jsmain);
  } else {
    jsmain = GetOrCreateFunction("main", globaltable.GetInt32(), arguments, false);
    SetCurrentFunction(jsmain);
    IntrinsiccallNode *stmt = CreateStmtIntrinsicCallAssigned0((MIRIntrinsicId)INTRN_JS_INIT_CONTEXT, NULL);
    AddStmtInCurrentFunctionBody(stmt);
  }
  return jsmain;
}

void JSMIRBuilder::InitBuiltinMethod() {
#define DEFBUILTINMETHOD(name, intrn_code, need_this) #name,
  const char *name[18] = {
#include "../include/builtinmethod.def"
  };
#undef DEFBUILTINMETHOD

  for (uint32_t i = 0; i < sizeof(name) / sizeof(const char *); i++) {
    if (!name[i]) {
      return;
    }
    MIRSymbol *st = globaltable.CreateSymbol(SCOPE_GLOBAL);
    st->SetNameStridx(GetOrCreateStringIndex(name[i]));
    if (!globaltable.AddToStringSymbolMap(st)) {
      return;
    }
    st->sclass = kScText;
    st->skind = kStFunc;
    InsertGlobalName((char *)name[i]);
  }
}

void JSMIRBuilder::Init() {
  jsvalueType = CreateJSValueType();
  jsvalue_ptr_ = globaltable.GetOrCreatePointerType(jsvalueType);

  InitGlobalName();
  // InitBuiltinMethod();
  // Now we create the main function as jsmain.
  // If the script is called by another program, the name should be jsmain.
  jsmain_ = CreateJSMain();
  char *name = "main";
  if (IsPlugin()) {
    name = GetWrapperName();
  }
  InsertGlobalName(name);
}

JSMIRFunction *JSMIRBuilder::GetFunction(const char *name) {
  JSMIRFunction *fn = GetFunc(name);
  if (!fn) {
    assert(false && "function is not created");
  }
  return fn;
}

JSMIRFunction *JSMIRBuilder::GetOrCreateFunction(const char *name, MIRType *returnType, ArgVector arguments,
                                                 bool isvarg) {
  DEBUGPRINTsv2("GetOrCreateFunction", name);
  JSMIRFunction *fn = GetFunc(name);
  if (fn) {
    DEBUGPRINTsv2("function is alread created: ", name);
    return fn;
  }

  MapleString fname(name, module_->mp_);
  MIRSymbol *funcst = globaltable.CreateSymbol(SCOPE_GLOBAL);
  gstridx_t stridx = GetOrCreateStringIndex(fname);
  stidx_t stidx = globaltable.GetStidxFromStridx(stridx);
  DEBUGPRINT3(fname);
  DEBUGPRINT3(stridx.idx);
  DEBUGPRINT3(stidx.Idx());

  funcst->SetNameStridx(stridx);
  if (!globaltable.AddToStringSymbolMap(funcst)) {
    return NULL;
  }
  funcst->sclass = kScText;
  funcst->skind = kStFunc;

  fn = module_->mp_->New<JSMIRFunction>(module_, funcst->GetStIdx());
  fn->Init();
  fn->puidx = globaltable.functable.size();
  globaltable.functable.push_back(fn);

  fn->returnTyidx = returnType->_ty_idx;

  std::vector<tyidx_t> funcvectype;
  std::vector<TypeAttrs> funcvecattr;
  for (uint32 i = 0; i < arguments.size(); i++) {
    MapleString var(arguments[i].first, module_->mp_);
    MIRSymbol *argst = fn->symtab->CreateSymbol(SCOPE_LOCAL);
    argst->SetNameStridx(GetOrCreateStringIndex(var));
    MIRType *ty = arguments[i].second;
    argst->SetTyIdx(ty->_ty_idx);
    argst->sclass = kScFormal;
    argst->skind = kStVar;
    fn->symtab->AddToStringSymbolMap(argst);
    fn->AddArgument(argst);
    funcvectype.push_back(ty->_ty_idx);
    funcvecattr.push_back(TypeAttrs());
  }
  funcst->SetTyIdx(globaltable.GetOrCreateFunctionType(returnType->_ty_idx, funcvectype, funcvecattr, isvarg)->_ty_idx);
  funcst->SetFunction(fn);
  fn->body = fn->code_mp->New<BlockNode>();

  AddNameFunc(name, fn);

  DEBUGPRINTsv2("function created: ", name);
  DEBUGPRINT2(fn);
  return fn;
}

void JSMIRBuilder::AddStmtInCurrentFunctionBody(StmtNode *n) {
  MIRBuilder::AddStmtInCurrentFunctionBody(n);
  DEBUGPRINTnode(n);
}

NaryStmtNode *JSMIRBuilder::CreateStmtReturn(BaseNode *rval, bool adjType, unsigned linenum) {
  NaryStmtNode *stmt = MIRBuilder::CreateStmtReturn(rval);
  stmt->srcpos.SetLinenum(linenum);
  AddStmtInCurrentFunctionBody(stmt);

  JSMIRFunction *func = GetCurrentFunction();
  if (adjType && !IsMain(func) && rval->op == OP_dread) {
    DEBUGPRINTsv2("modify _return_type", (rval->op));
    AddrofNode *dn = (AddrofNode *)rval;
    stidx_t stidx = dn->stidx;
    MIRSymbol *var = module_->CurFunction()->GetLocalOrGlobalSymbol(stidx);
    MIRType *type = var->GetType();
    DEBUGPRINT3(type);
    int fid = dn->fieldid;
    if (fid) {
      MIRStructType *stype = static_cast<MIRStructType *>(type);
      // fieldid in a structure starts from 1 while type vector starts from 0
      type = stype->GetElemType(fid - 1);
    }
  }
  return stmt;
}

void JSMIRBuilder::UpdateFunction(JSMIRFunction *func, MIRType *returnType, ArgVector arguments) {
  if (returnType) {
    func->returnTyidx = returnType->_ty_idx;
  }

  for (uint32 i = 0; i < arguments.size(); i++) {
    MapleString var(arguments[i].first, module_->mp_);
    MIRSymbol *st = func->symtab->CreateSymbol(SCOPE_LOCAL);
    st->SetNameStridx(GetOrCreateStringIndex(var));
    MIRType *ty = arguments[i].second;
    st->SetTyIdx(ty->_ty_idx);
    st->sclass = kScFormal;
    st->skind = kStVar;
    func->symtab->AddToStringSymbolMap(st);
    func->AddArgument(st);
  }
}

#if 0
void JSMIRBuilder::SaveReturnValue(MIRSymbol *var)
{
    DEBUGPRINT4("in SaveReturnValue")

    BaseNode *bn = CreateExprRegread(globaltable.type_table_[var->GetTyIdx().idx]->GetPrimType(), -kSregRetval0);
    StmtNode *stmt = CreateStmtDassign(var, 0, bn);
    stmt->srcpos.SetLinenum(lineNo);
    MIRBuilder::AddStmtInCurrentFunctionBody(stmt);
}

#endif

StmtNode *JSMIRBuilder::CreateStmtDassign(MIRSymbol *symbol, fldid_t fieldId, BaseNode *src, unsigned linenum) {
  DEBUGPRINT4("in CreateStmtDassign")

  StmtNode *stmt = MIRBuilder::CreateStmtDassign(symbol, fieldId, src);
  stmt->srcpos.SetLinenum(linenum);
  AddStmtInCurrentFunctionBody(stmt);
  return stmt;
}

IntrinsiccallNode *JSMIRBuilder::CreateStmtIntrinsicCall1N(MIRIntrinsicId idx, BaseNode *arg0,
                                                           MapleVector<BaseNode *> &args) {
  IntrinsiccallNode *stmt = module_->CurFuncCodeMp()->New<IntrinsiccallNode>(module_, OP_intrinsiccall);
  MapleVector<BaseNode *> arguments(module_->CurFuncCodeMpAllocator()->Adapter());
  arguments.push_back(arg0);
  for (int i = 0; i < args.size(); i++) {
    arguments.push_back(args[i]);
  }
  stmt->intrinsic = idx;
  stmt->nopnd = arguments;
  return stmt;
}

}  // namespace maple
