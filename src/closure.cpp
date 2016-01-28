/// Copyright [year] <Copyright Owner>
#include <string>
#include "js/src/vm/ScopeObject.h"
#include "../include/closure.h"

namespace mapleir {

bool JSClosure::IsFuncModified(char *name) {
  std::vector<char *>::iterator I;
  for (I=funcMod.begin(); I!=funcMod.end(); I++)
    if (!strcmp(name, *I))
      return true;
  return false;
}

void JSClosure::UpdateFuncMod(char *name) {
  if (scope_->IsFunction(name))
    if (!IsFuncModified(name))
      funcMod.push_back(name);
}

// ??? Use scope chain.
MIRSymbol *JSClosure::GetSymbolFromEnclosingScope(JSMIRFunction *func,
                                                  const char *name) {
  stridx_t idx = jsbuilder_->GetStringIndex(name);
  if (!idx)
    return NULL;

  MIRSymbol *st = func->symtab->GetSymbolFromStridx(idx);
  if (st) return st;
  st = jsbuilder_->module_->symtab->GetSymbolFromStridx(idx);
  if (st) return st;
  return NULL;
}

MIRSymbol *JSClosure::GetSymbolFromEnclosingScope(JSMIRFunction *func,
                                                  stidx_t stidx) {
  if (!stidx)
    return NULL;

  MIRSymbol *st = func->symtab->GetSymbolFromStidx(stidx);
  if (st) return st;
  st = jsbuilder_->module_->symtab->GetSymbolFromStidx(stidx);
  if (st) return st;
  return NULL;
}

MIRType *JSClosure::GetOrCreateEnvType(JSMIRFunction *func) {
  std::stringstream ss;
  MIRSymbol *func_st = jsbuilder_->module_->symtab->GetSymbolFromStidx(func->stidx);
  ss << func_st->GetName();
  std::string env_name = ss.str() + "_env_type";
  DEBUGPRINT2(env_name);

  if (func->envtype) {
    DEBUGPRINTsv2("env for func has been setup!", env_name);
    return func->envtype;
  }

  FieldVector env_fields(jsbuilder_->module_->mp_allocator_.Adapter());

  stridx_t parentenv = jsbuilder_->GetOrCreateStringIndex("parentenv");
  if (func->scope->IsTopLevel()) {
    env_fields.push_back(FieldPair(parentenv, jsbuilder_->GetVoidPtr()->_ty_idx));
  } else {
    ScopeNode *sn = func->scope;
    JSMIRFunction *parent = sn->GetParentFunc();
    MIRType *parentenv_type = parent->envtype;
    DEBUGPRINT3(parentenv_type);
    MIRType *envptr = jsbuilder_->GetOrCreatePointerType(parentenv_type);
    DEBUGPRINT3(envptr);
    func->penvtype = parentenv_type;
    func->penvptr = envptr;
    env_fields.push_back(FieldPair(parentenv, envptr->_ty_idx));
  }

  MIRType *env_type = jsbuilder_->CreateStructType(env_name.c_str(), env_fields);
  DEBUGPRINT2(env_name);
  DEBUGPRINT2(env_type);
  MIRStructType *stf = (MIRStructType *)(env_type);
  DEBUGPRINT2(stf->GetElemType(0));
  stridx_t idxf = jsbuilder_->module_->symtab->GetSymbolFromStidx(func->stidx)->GetNameStridx();
  DEBUGPRINT2(idxf);
  DEBUGPRINT2(func->stidx);

  func->envtype = env_type;
  func->envptr = jsbuilder_->GetOrCreatePointerType(env_type);

  return env_type;
}

void JSClosure::AddAliasToEnvType(MIRType *env_type, char *name, MIRType *T) {
  DEBUGPRINTsv3("add env", name);
  stridx_t stridx = jsbuilder_->GetOrCreateStringIndex(name);
  MIRStructType *env_struct = static_cast<MIRStructType *>(env_type);

  env_struct->fields.push_back(FieldPair(stridx, T->_ty_idx));
}

void JSClosure::AddFuncFormalsToEnvType(JSMIRFunction *func) {
  // add formal parameters to env, they are posible aliased vars
  typedef std::pair<JSFunction *, std::vector<JSAtom *>> funcVarVecPair;
  std::vector<funcVarVecPair> formals = jsscript_->funcFormals;

  char *name;
  char *funcname;
  JSFunction *jsfun;
  std::vector<funcVarVecPair>::iterator I;
  std::vector<char *> nameVec;
  for (I=formals.begin(); I!=formals.end(); I++) {
    jsfun = (*I).first;
    funcname = Util::GetString(jsfun->name(), mp_, jscontext_);
    DEBUGPRINT2(jsfun);
    // anonymous function.
    if (!funcname) {
      funcname = (char*)Util::GetSequentialName0("anonymous_func_", scope_->GetAnonyidx(jsfun), mp_);
    }
    DEBUGPRINT2(funcname);
    MIRSymbol *func_st = jsbuilder_->module_->symtab->GetSymbolFromStidx(func->stidx);
    if (func_st && strcmp(funcname, func_st->GetName().c_str()) == 0) {
      std::vector<JSAtom *> args = (*I).second;
      std::vector<JSAtom *>::iterator IA;
      DEBUGPRINTsv3("AddFuncFormalsToEnvType", funcname);
      DEBUGPRINT3((args.size()));
      for (IA = args.begin(); IA != args.end(); IA++) {
        name = Util::GetString(*IA, mp_, jscontext_);
        nameVec.push_back(name);
        AddAliasToEnvType(func->envtype, name, jsvalue_type_);
      }
      funcArgPair P(func, nameVec);
      funcFormals.push_back(P);

      break;
    }
  }

  return;
}

JSMIRFunction *JSClosure::ProcessFunc(JSFunction *jsfun, char *funcname) {
  MIRType *retuen_type = jsvalue_type_;
  ArgVector arguments(jsbuilder_->module_->mp_allocator_.Adapter());

  JSMIRFunction *func = jsbuilder_->GetOrCreateFunction(funcname, retuen_type, arguments, false);
  jsbuilder_->module_->AddFunction(func);
  SetJSMIRFunc(funcname, func);
  DEBUGPRINT2(funcname);
  DEBUGPRINT2(func);

  ScopeNode *sn = scope_->GetOrCreateSN(funcname);
  sn->SetFunc(func);

  func->initAliasList();
  arguments.push_back(ArgPair("_this", jsbuilder_->GetDynany()));

  DEBUGPRINT3((sn->IsWithEnv()));
  if (sn->IsWithEnv()) {
    GetOrCreateEnvType(func);
    AddFuncFormalsToEnvType(func);
  }

  if (!sn->IsTopLevel() && (sn->IsWithEnv() || sn->UseAliased())) {
    if (!func->with_env_arg) {
      JSMIRFunction *parent = sn->GetParentFunc();
      arguments.push_back(ArgPair("_env", parent->envptr));
      DEBUGPRINTsv3("_env", funcname);
      func->with_env_arg = true;
    }
  }

  typedef std::pair<JSFunction *, std::vector<JSAtom *>> funcVarVecPair;
  std::vector<funcVarVecPair> formals = jsscript_->funcFormals;

  func->argc = 0;
  for (int j=0; j<formals.size(); j++) {
    JSFunction *fun = formals[j].first;
    if (fun == jsfun) {
      std::vector<JSAtom *> args = formals[j].second;
      func->argc = args.size();
      for (uint32 i = 0; i < jsfun->nargs(); i++) {
        char *name = Util::GetString(args[i], mp_, jscontext_);
        DEBUGPRINT3(name);
        MapleString argname(name, jsbuilder_->module_->mp_);
        arguments.push_back(ArgPair(argname.c_str(), jsbuilder_->GetDynany()));
      }
      break;
    }
  }

  jsbuilder_->UpdateFunction(func, NULL, arguments);

  std::pair<JSScript *, JSMIRFunction *> P(jsfun->nonLazyScript(), func);
  scriptstack_.push(P);

  return func;
}

// JSOP_DEFFUN 127
bool JSClosure::ProcessOpDefFun(jsbytecode *pc) {
  JSFunction *jsfun = currscr_->getFunction(GET_UINT32_INDEX(pc));
  JSScript *scr = jsfun->nonLazyScript();
  MIRType *retuen_type = jsvalue_type_;
  ArgVector arguments(jsbuilder_->module_->mp_allocator_.Adapter());
  JSAtom *atom = jsfun->displayAtom();
  DEBUGPRINT2(atom);
  char *funcname = Util::GetString(atom, mp_, jscontext_);
  if (!funcname)
    return false;

  JSMIRFunction *func = ProcessFunc(jsfun, funcname);

  funcstack_.push(func);

  return true;
}

// JSOP_LAMBDA 131
void JSClosure::ProcessOpLambda(jsbytecode *pc) {
  JSFunction *jsfun = currscr_->getFunction(GET_UINT32_INDEX(pc));
  JSAtom *atom = jsfun->displayAtom();

  // isLambda() does not seem reliable
  DEBUGPRINT3((jsfun->isLambda()));
  // we already know it is a Lambda so only check other two parts in isNamedLambda()
  // isLambda() && displayAtom() && !hasGuessedAtom()
  // if((jsfun->isNamedLambda()))
  char *funcname;
  if (atom && !jsfun->hasGuessedAtom())
    funcname = Util::GetString(atom, mp_, jscontext_);
  else
    funcname = scope_->GetAnonyFunctionName(pc);

  ProcessFunc(jsfun, funcname);

  return;
}

bool JSClosure::IsLocalVar(JSMIRFunction *func, char *name) {
  typedef std::pair<JSFunction *, std::vector<JSAtom *>> funcVarVec;
  std::vector<funcVarVec> locals = jsscript_->funcLocals;

  char *funcname;
  JSFunction *jsfun;
  std::vector<funcVarVec>::iterator I;
  std::vector<char *> nameVec;
  DEBUGPRINT2(name);
  for (I=locals.begin(); I!=locals.end(); I++) {
    jsfun = (*I).first;
    funcname = Util::GetString(jsfun->name(), mp_, jscontext_);
    // anonymous function.
    if (!funcname) {
      funcname = (char*)Util::GetSequentialName0("anonymous_func_", scope_->GetAnonyidx(jsfun), mp_);
    }
    DEBUGPRINT2(funcname);
    MIRSymbol *func_st = jsbuilder_->module_->symtab->GetSymbolFromStidx(func->stidx);
    if (func_st && strcmp(funcname, func_st->GetName().c_str()) == 0) {
      std::vector<JSAtom *> vars = (*I).second;
      std::vector<JSAtom *>::iterator IA;
      DEBUGPRINTsv3("IsLocalVar", funcname);
      DEBUGPRINT3((vars.size()));
      char *varname;
      for (IA = vars.begin(); IA != vars.end(); IA++) {
        varname = Util::GetString(*IA, mp_, jscontext_);
        DEBUGPRINT2(varname);
        if (strcmp(name, varname) == 0)
          return true;
      }
      return false;
    }
  }
  return false;
}

char *JSClosure::GetLocalVar(JSMIRFunction *func, uint32_t local_no) {
  typedef std::pair<JSFunction *, std::vector<JSAtom *>> funcVarVecPair;
  std::vector<funcVarVecPair> locals = jsscript_->funcLocals;

  char *name = NULL;
  char *funcname;
  JSFunction *jsfun;
  int i;
  for (i=0; i<locals.size(); i++) {
    jsfun = locals[i].first;
    funcname = Util::GetString(jsfun->name(), mp_, jscontext_);
    DEBUGPRINT2(jsfun);
    // set name for anonymous functions same as in jsscript_->funcLocals.
    if (!funcname) {
      funcname = (char*)Util::GetSequentialName0("anonymous_func_", scope_->GetAnonyidx(jsfun), mp_);
    }
    DEBUGPRINT2(funcname);
    // found the function
    MIRSymbol *func_st = jsbuilder_->module_->symtab->GetSymbolFromStidx(func->stidx);
    if (func_st && strcmp(funcname, func_st->GetName().c_str()) == 0) {
      std::vector<JSAtom *> args = locals[i].second;
      if (local_no < args.size())
        name = Util::GetString(args[local_no], mp_, jscontext_);
      break;
    }
  }
  if (!name)
    name = Util::GetSequentialName("local_var_", local_no, mp_);
  return name;
}

// JSOP_GETALIASEDVAR 136
void JSClosure::ProcessAliasedVar(jsbytecode *pc) {
  JSAtom * atom = ScopeCoordinateName(
      jscontext_->runtime()->scopeCoordinateNameCache, currscr_, pc);
  JSMIRFunction *func = funcstack_.top();
  DEBUGPRINT2((func->scope->GetName()));
  char *name = Util::GetString(atom, mp_, jscontext_);
  if (!name)
    return;
  DEBUGPRINT3(name);
  MIRSymbol *func_st = jsbuilder_->module_->symtab->GetSymbolFromStidx(func->stidx);
  const char *funcname = func_st->GetName().c_str();
  ScopeNode *sn = scope_->GetOrCreateSN((char *)funcname);
  ScopeNode *psn = sn->GetParent();

  if (!psn) {
    DEBUGPRINT3("alias var not found, could be from block"); 
    return;
  }

  JSMIRFunction *parent = psn->GetFunc();

  int idx = 0;
  MIRType *env_type = NULL;

  // check if this alias is in current func's env
  if (sn->IsWithEnv()) {
    env_type = func->envtype;
    idx = jsbuilder_->GetStructFieldIdFromFieldName(env_type, name);

    if (idx)
      return;
  }

  // check if this alias is a local var need to be added into env
  if (sn->IsWithEnv() && IsLocalVar(func, name)) {
    env_type = func->envtype;
    AddAliasToEnvType(env_type, name, jsvalue_type_);
    idx = jsbuilder_->GetStructFieldIdFromFieldName(env_type, name);
    DEBUGPRINT3(idx);
    return;
  }

  // recursively search in parent's alias list
  JSMIRFunction *p = parent;
  while (p != jsmain_ && !idx && psn->IsWithEnv()) {
    env_type = p->envtype;
    idx = jsbuilder_->GetStructFieldIdFromFieldName(env_type, name);
    DEBUGPRINT3(idx);
    if (idx)
      return;
    psn = psn->GetParent();

    if (psn)
      p = psn->GetFunc();
  }

  if (!idx) {
    DEBUGPRINT3("alias var not found, could be from block"); 
  }

  return;
}

void JSClosure::CloseFuncBookKeeping() {
  funcstack_.pop();

  // process inner functions
  DEBUGPRINT3((scriptstack_.size()));
  while (scriptstack_.size()) {
    JSScript *scr = scriptstack_.top().first;
    JSMIRFunction *lambda = scriptstack_.top().second;
    jsbuilder_->SetCurrentFunction(lambda);
    funcstack_.push(lambda);
    scriptstack_.pop();

    Build(scr);
  }

  if (funcstack_.size()) {
    JSMIRFunction * currFunc = funcstack_.top();
    jsbuilder_->SetCurrentFunction(currFunc);
  }
}

void JSClosure::Init() {
  jsmain_ = jsbuilder_->jsmain_;
  funcstack_.push(jsmain_);
  char *name = "main";
  if (jsbuilder_->IsPlugin() && !jsbuilder_->WithMain())
     name = jsbuilder_->GetWrapperName();
  // TODO: since GetOrCreateSN() doesn't change name, so we should declare the parameter as const name
  scope_->GetOrCreateSN(name)->SetFunc(jsmain_);

  jsvalue_type_ = jsbuilder_->jsvalue_type_;
  jsvalue_ptr_ = jsbuilder_->jsvalue_ptr_;
}

bool JSClosure::Build(JSScript *script) {
  jsbytecode *start = script->code();
  jsbytecode *end = script->codeEnd();
  currscr_ = script;

  bool ret = BuildSection(script, start, end);

  return ret;
}

bool JSClosure::BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend) {
  jsbytecode *pc = pcstart;
  JSFunction *jsfun;

  char *name;
  char *parent;
  JSOp lastOp;
  while (pc < pcend) {
    JSOp op = JSOp(*pc);
    JSScript *scr;
    DEBUGPRINT3((Util::getOpcodeName[op]));

    Util::SetIndent(4);

    switch (op) {
      case JSOP_DEFFUN: /*127, 5, 0, 0*/ {
        ProcessOpDefFun(pc);
        break;
      }
      case JSOP_LAMBDA: /*130, 5, 0, 1*/ {
        ProcessOpLambda(pc);
        break;
      }
      case JSOP_GETALIASEDVAR: /*136, 5, 0, 1*/
      case JSOP_SETALIASEDVAR: /*137, 5, 1, 1*/ {
        ProcessAliasedVar(pc);
        break;
      }
    }

    // check if func is reassigned
    switch (op) {
      case JSOP_SETLOCAL: {
        uint32_t i = GET_LOCALNO(pc);
        JSMIRFunction *func = jsbuilder_->GetCurrentFunction();
        char *name = GetLocalVar(func, i);
        // not count the init
        if (lastOp != JSOP_LAMBDA)
          UpdateFuncMod(name);
        break;
      }
      case JSOP_SETGNAME:
      case JSOP_SETNAME: {
        JSAtom *atom = script->getName(pc);
        char *name = Util::GetString(atom, mp_, jscontext_);
        UpdateFuncMod(name);
        break;
      }
    }

    lastOp = op;
    pc = js::GetNextPc(pc);
  }

  if (lastOp == JSOP_RETRVAL)
    CloseFuncBookKeeping();

  return true;
}

bool JSClosure::FuncUseEnv(char *name) {
  ScopeNode * sn = scope_->GetOrCreateSN(name);
  return sn->IsWithEnv() || sn->UseAliased();
}

JSMIRFunction *JSClosure::GetJSMIRFunc(char *name) {
  std::vector<std::pair<char *, JSMIRFunction *>>::iterator I;
  for(I = nameJSMIRfunc_.begin(); I != nameJSMIRfunc_.end(); I++) {
    if(strcmp(name, (*I).first) == 0) {
      return (*I).second;
    }
  }
  return NULL;
}

void JSClosure::SetJSMIRFunc(char *name, JSMIRFunction *func) {
  if (GetJSMIRFunc(name))
    return;
  std::pair<char *, JSMIRFunction *> P(name, func);
  nameJSMIRfunc_.push_back(P);
}

}  // namespace mapleir
