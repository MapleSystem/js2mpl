/// Copyright [year] <Copyright Owner>
#include <string>
#include "../include/jsmirbuilder.h"

namespace maple {
// Create jsvalue_type as the JS::Value from mozjs-31.2.0/js/public/Value.h.
// We only consider littel_endian and 32-bit architectures here.
MIRType *JSMIRBuilder::CreateJSValueType() {
  return GlobalTables::GetTypeTable().GetDynany();
}

JSMIRFunction *JSMIRBuilder::CreateJSMain() {
  ArgVector arguments(mirModule->memPoolAllocator.Adapter());
  JSMIRFunction *jsmain = NULL;
  if (IsPlugin()) {
    // jsmain = GetOrCreateFunction(GetWrapperName(), GlobalTables::GetTypeTable().GetDynany(), arguments, false);
    jsmain = GetOrCreateFunction("main", GlobalTables::GetTypeTable().GetDynany(), arguments, false);
    SetCurrentFunction(jsmain);
  } else {
    MapleVector<BaseNode *> argsVec(mirModule->memPoolAllocator.Adapter());
    jsmain = GetOrCreateFunction("main", GlobalTables::GetTypeTable().GetInt32(), arguments, false);
    SetCurrentFunction(jsmain);
    IntrinsiccallNode *stmt = CreateStmtIntrinsicCallAssigned((MIRIntrinsicID)INTRN_JS_INIT_CONTEXT, argsVec, NULL);
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
    MIRSymbol *st = GlobalTables::GetGsymTable().CreateSymbol(kScopeGlobal);
    st->SetNameStridx(GetOrCreateStringIndex(name[i]));
    if (!GlobalTables::GetGsymTable().AddToStringSymbolMap(st)) {
      return;
    }
    st->storageClass = kScText;
    st->sKind = kStFunc;
    InsertGlobalName((char *)name[i]);
  }
}

void JSMIRBuilder::Init() {
  jsvalueType = CreateJSValueType();
  jsvalue_ptr_ = GlobalTables::GetTypeTable().GetOrCreatePointerType(jsvalueType);

  InitGlobalName();
  // InitBuiltinMethod();
  // Now we create the main function as jsmain.
  // If the script is called by another program, the name should be jsmain.
  jsmain_ = CreateJSMain();
  char *name = "main";
  if (IsPlugin()) {
   // name = GetWrapperName();
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
    fn->dup_name = true;
    DEBUGPRINTsv2("function is alread created: ", name);
    return fn;
  }

  MapleString fname(name, mirModule->memPool);
  MIRSymbol *funcst = GlobalTables::GetGsymTable().CreateSymbol(kScopeGlobal);
  GStrIdx stridx = GetOrCreateStringIndex(fname);
  StIdx stidx = GlobalTables::GetGsymTable().GetStIdxFromStrIdx(stridx);
  DEBUGPRINT3(fname);
  DEBUGPRINT3(stridx.GetIdx());
  DEBUGPRINT3(stidx.Idx());

  funcst->SetNameStridx(stridx);
  if (!GlobalTables::GetGsymTable().AddToStringSymbolMap(funcst)) {
    return NULL;
  }
  funcst->storageClass = kScText;
  funcst->sKind = kStFunc;

  fn = mirModule->memPool->New<JSMIRFunction>(mirModule, funcst->GetStIdx());
  fn->Init();
  fn->puIdx = GlobalTables::GetFunctionTable().funcTable.size();
  GlobalTables::GetFunctionTable().funcTable.push_back(fn);

  fn->inferredReturnTyIdx = returnType->tyIdx;

  std::vector<TyIdx> funcvectype;
  std::vector<TypeAttrs> funcvecattr;
  for (uint32 i = 0; i < arguments.size(); i++) {
    MIRType *ty = arguments[i].second;
    FormalDef formalDef(GetOrCreateStringIndex(arguments[i].first.c_str()), nullptr, ty->tyIdx, TypeAttrs());
    fn->formalDefVec.push_back(formalDef);
    funcvectype.push_back(ty->tyIdx);
    funcvecattr.push_back(TypeAttrs());
  }
  funcst->SetTyIdx(GlobalTables::GetTypeTable().GetOrCreateFunctionType(mirModule, returnType->tyIdx, funcvectype, funcvecattr, isvarg)->tyIdx);
  funcst->SetFunction(fn);
  // create symTab and etc
  fn->symTab = fn->dataMemPool->New<MIRSymbolTable>(&fn->dataMPAllocator);
  fn->pregTab = fn->dataMemPool->New<MIRPregTable>(&fn->dataMPAllocator);
  fn->typeNameTab = fn->dataMemPool->New<MIRTypeNameTable>(&fn->dataMPAllocator);
  fn->labelTab = fn->dataMemPool->New<MIRLabelTable>(&fn->dataMPAllocator);
  fn->body = fn->codeMemPool->New<BlockNode>();
  fn->funcType = dynamic_cast<MIRFuncType *>(funcst->GetType());

  AddNameFunc(name, fn);

  DEBUGPRINTsv2("function created: ", name);
  DEBUGPRINT2(fn);
  return fn;
}

void JSMIRBuilder::AddStmtInCurrentFunctionBody(StmtNode *n) {
  MIRBuilder::AddStmtInCurrentFunctionBody(n);
  DEBUGPRINTnode(n);
}

void JSMIRBuilder::ClearStmtsInCurrentFunctionBody(void) {
  JSMIRFunction *func = GetCurrentFunction();
  func->body->ResetBlock();
}

NaryStmtNode *JSMIRBuilder::CreateStmtReturn(BaseNode *rval, bool adjType, unsigned linenum) {
  NaryStmtNode *stmt = MIRBuilder::CreateStmtReturn(rval);
  stmt->srcPosition.SetLinenum(linenum);
  AddStmtInCurrentFunctionBody(stmt);

  JSMIRFunction *func = GetCurrentFunction();
  if (adjType && !IsMain(func) && rval->op == OP_dread) {
    DEBUGPRINTsv2("modify _return_type", (rval->op));
    AddrofNode *dn = (AddrofNode *)rval;
    StIdx stidx = dn->stIdx;
    MIRSymbol *var = mirModule->CurFunction()->GetLocalOrGlobalSymbol(stidx);
    MIRType *type = var->GetType();
    DEBUGPRINT3(type);
    int fid = dn->fieldID;
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
    func->inferredReturnTyIdx = returnType->tyIdx;
  }

  for (uint32 i = 0; i < arguments.size(); i++) {
    MapleString var(arguments[i].first, mirModule->memPool);
    MIRSymbol *st = func->symTab->CreateSymbol(kScopeLocal);
    st->SetNameStridx(GetOrCreateStringIndex(var));
    MIRType *ty = arguments[i].second;
    st->SetTyIdx(ty->tyIdx);
    st->storageClass = kScFormal;
    st->sKind = kStVar;
    func->symTab->AddToStringSymbolMap(st);
    func->AddArgument(st);
  }
}

#if 0
void JSMIRBuilder::SaveReturnValue(MIRSymbol *var)
{
    DEBUGPRINT4("in SaveReturnValue")

    BaseNode *bn = CreateExprRegread(GlobalTables::GetTypeTable().type_table_[var->GetTyIdx().idx]->GetPrimType(), -kSregRetval0);
    StmtNode *stmt = CreateStmtDassign(var, 0, bn);
    stmt->srcPosition.SetLinenum(lineNo);
    MIRBuilder::AddStmtInCurrentFunctionBody(stmt);
}

#endif

StmtNode *JSMIRBuilder::CreateStmtDassign(MIRSymbol *symbol, FieldID fieldId, BaseNode *src, unsigned linenum) {
  DEBUGPRINT4("in CreateStmtDassign")

  StmtNode *stmt = MIRBuilder::CreateStmtDassign(symbol, fieldId, src);
  stmt->srcPosition.SetLinenum(linenum);
  AddStmtInCurrentFunctionBody(stmt);
  return stmt;
}

IntrinsiccallNode *JSMIRBuilder::CreateStmtIntrinsicCall1N(MIRIntrinsicID idx, BaseNode *arg0,
                                                           MapleVector<BaseNode *> &args) {
  IntrinsiccallNode *stmt = mirModule->CurFuncCodeMemPool()->New<IntrinsiccallNode>(mirModule, OP_intrinsiccall);
  MapleVector<BaseNode *> arguments(mirModule->CurFuncCodeMemPoolAllocator()->Adapter());
  arguments.push_back(arg0);
  for (int i = 0; i < args.size(); i++) {
    arguments.push_back(args[i]);
  }
  stmt->intrinsic = idx;
  stmt->nOpnd = arguments;
  return stmt;
}

ConstvalNode* JSMIRBuilder::CreateDynf64Const(double val) {
  ConstvalNode *constvalNode = GetCurrentFuncCodeMp()->New<ConstvalNode>();
  MIRDoubleConst *dblConst = GetCurrentFunction()->dataMemPool->New<MIRDoubleConst>(val,
                                               GlobalTables::GetTypeTable().GetPrimType(PTY_dynf64));
  constvalNode->primType = PTY_dynf64;
  constvalNode->constVal = dblConst;
  return constvalNode;
}

}  // namespace maple
