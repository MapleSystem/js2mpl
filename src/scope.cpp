/// Copyright [year] <Copyright Owner>
#include "js2mpl-vm/include/scope.h"

namespace mapleir {

  void ScopeNode::clear() {
    node_ = NULL;
    func_ = NULL;
    parent_ = NULL;
    children_.empty();
    isLeaf_ = false;
    isTopLevel_ = false;
    useAliased_ = false;
    withEnv_ = false;
    flag_ = false;
  }

  ScopeNode::ScopeNode(BaseNode *node) {
    clear();
    node_ = node;
  }

  ScopeNode::ScopeNode(JSMIRFunction *func) : func_(func) {
    clear();
    func_ = func;
  }

  ScopeNode::ScopeNode(char *name) : name_(name) {
    clear();
    name_ = name;
  }

  void ScopeNode::AddChild(ScopeNode *node) {
    std::list<ScopeNode *>::iterator I;
    for (I = children_.begin(); I != children_.end(); I++) {
      if (*I == node)
        return;
    }
    children_.push_back(node);
  }

  void ScopeNode::SetParent(ScopeNode *node) {
    parent_ = node;
  }

  void ScopeNode::PropWithEnv() {
    if (parent_ && !(parent_->IsMain())) {
      parent_->SetWithEnv(true);
      parent_->PropWithEnv();
    }
  }

  bool ScopeNode::SetChild(ScopeNode *child) {
    if (!child)
      return false;

    AddChild(child);
    child->SetParent(this);
    return true;
  }

  void ScopeNode::SetFunc(JSMIRFunction *func) {
    func_ = func;
    if (parent_)
      parent_->SetChild(this);
    std::list<ScopeNode *>::iterator I;
    for (I = children_.begin(); I != children_.end(); I++) {
      (*I)->SetParent(this);
    }

    // set scope in function
    func->scope = this;
  }

  void ScopeNode::Dump() {
    DEBUGPRINT2pure(name_);
    DEBUGPRINT2pure(this);
    DEBUGPRINT2pure(func_);
    DEBUGPRINT2pure(isLeaf_);
    DEBUGPRINT2pure(isTopLevel_);
    DEBUGPRINT2pure(useAliased_);
    DEBUGPRINT2pure(withEnv_);
    DEBUGPRINT2pure(parent_);
    std::list<ScopeNode *>::iterator I;
    ScopeNode *child;
    for (I = children_.begin(); I != children_.end(); I++) {
      child = *I;
      DEBUGPRINT2pure(child);
    }
    DEBUGPRINT0;
  }

ScopeNode *Scope::GetOrCreateSN(char *name) {
  std::list<std::pair<char *, ScopeNode *>>::iterator I;
  for(I = scopeChain.begin(); I != scopeChain.end(); I++) {
    if(strcmp(name, (*I).first) == 0) {
      return (*I).second;
    }
  }

  // create new one
  ScopeNode *sn = MP_NEW(mp_, ScopeNode(name));
  std::pair<char *, ScopeNode *> P(name, sn);
  scopeChain.push_back(P);

  return sn;
}

ScopeNode *Scope::GetOrCreateSN(JSMIRFunction *func) {
  char *name = func->_name.c_str();
  return GetOrCreateSN(name);
}

void Scope::SetSNParent(char *name, char *parent) {
  ScopeNode *sn = GetOrCreateSN(name);
  ScopeNode *snp = GetOrCreateSN(parent);
  sn->SetParent(snp);
  snp->AddChild(sn);
  if (snp->IsMain())
    sn->SetTopLevel();
}

void Scope::SetSNLeaf(char *name) {
  ScopeNode *sn = GetOrCreateSN(name);
  sn->SetLeaf();
}

void Scope::SetSNClosure(char *name) {
  ScopeNode *sn = GetOrCreateSN(name);
  if (!(sn->IsTopLevel())) {
    sn->SetUseAliased();
    sn->SetWithEnv(true);
  }
}

void Scope::AddSNChild(char *name, char *child) {
  ScopeNode *sn = GetOrCreateSN(name);
  ScopeNode *snc = GetOrCreateSN(child);
  sn->AddChild(snc);
  snc->SetParent(sn);
}

void Scope::Init() {
  // set up anonymous function to anon_func_no_ mapping
  typedef std::pair<JSFunction *, std::vector<JSAtom *>> funcVarVecPair;
  std::vector<funcVarVecPair> formals = jsscript_->funcFormals;
  char *name;
  char *funcname;
  std::vector<funcVarVecPair>::iterator I;
  std::vector<char *> nameVec;
  JSFunction *jsfun;
  for (I=formals.begin(); I!=formals.end(); I++) {
    jsfun = (*I).first;

    if (!jsfun->name()) {
      DEBUGPRINT3(jsfun);
      SetAnonyidx(jsfun, anon_func_no_);
      anon_func_no_ ++;
    }
  }

  return;
}

bool Scope::Build(JSScript *script) {
  jsbytecode *start = script->code();
  jsbytecode *end = script->codeEnd();

  bool ret = BuildSection(script, start, end);

  return ret;
}


bool Scope::BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend) {
  jsbytecode *pc = pcstart;
  JSFunction *jsfun;

  // use OPT_DUMPJSOPONLY to only dump JSOP code
  if (js2mplDebug == OPT_DUMPJSOPONLY) {
    while (pc < pcend) {
      JSOp op = JSOp(*pc);
      JSScript *scr;

      switch (op) {
        case JSOP_DEFFUN: /*127, 5, 0, 0*/
        case JSOP_LAMBDA: /*130, 5, 0, 1*/  {
          JSFunction *jsfun = script->getFunction(GET_UINT32_INDEX(pc));
          scr = jsfun->nonLazyScript();
          Build(scr);
        }
      }
      std::cout << Util::getOpcodeName[op] << std::endl;
      pc = js::GetNextPc(pc);
    }

  } else {

    if (script == jsscript_) {
      funcstack1_.push("main");
      if (js2mplDebug>2) std::cout << "main {" << std::endl;
      ScopeNode * sn = GetOrCreateSN("main");
      sn->SetTopLevel();
    }

    char *name;
    char *parent;
    while (pc < pcend) {
      JSOp op = JSOp(*pc);
      unsigned lineNo = js::PCToLineNumber(script, pc);
      JSScript *scr;

      Util::SetIndent(2);
      DEBUGPRINTnn(lineNo, Util::getOpcodeName[op]);
      DEBUGPRINT0;
      Util::SetIndent(4);

      switch (op) {
        case JSOP_DEFFUN: /*127, 5, 0, 0*/
        case JSOP_LAMBDA: /*130, 5, 0, 1*/
          {
            jsfun = script->getFunction(GET_UINT32_INDEX(pc));
            DEBUGPRINT3(jsfun);
            scr = jsfun->nonLazyScript();
            JSAtom *atom = jsfun->displayAtom();
            if (atom && !jsfun->hasGuessedAtom()) {
              name = Util::GetString(atom, mp_, ctx_);
            } else {
              name = (char*)Util::GetSequentialName0("anonymous_func_", GetAnonyidx(jsfun), mp_);
              std::pair<jsbytecode *, char *> P(pc, name);
              bytecodeAnonyFunc.push_back(P);
            }
            funcNames_.push_back(name);
            std::pair<JSScript *, char *> P(scr, name);
            scriptstack_.push(P);

            parent = funcstack1_.top();
            SetSNParent(name, parent);
            break;
          }
        case JSOP_GETALIASEDVAR: /*136, 5, 0, 1*/
        case JSOP_SETALIASEDVAR: /*137, 5, 1, 1*/
          {
            name = funcstack1_.top();
            SetSNClosure(name);
            break;
          }
        case JSOP_RETRVAL: /*153, 1, 0, 0*/
          {
            name = funcstack1_.top();
            if (js2mplDebug>2) std::cout << "}\n" << std::endl;
            funcstack1_.pop();
            DEBUGPRINT3((scriptstack_.size()));
            while (scriptstack_.size()) {
              JSScript *scr = scriptstack_.top().first;
              name = scriptstack_.top().second;
              scriptstack_.pop();
              funcstack1_.push(name);
              if (js2mplDebug>2) std::cout << name << "{" << std::endl;
              Build(scr);
            }
            break;
          }
      }
      pc = js::GetNextPc(pc);
    }

    PopulateSNInfo();
  }

  return true;
}

char *Scope::GetAnonyFunctionName(jsbytecode *pc) {
  std::list<std::pair<jsbytecode *, char *>>::iterator I;
  jsbytecode *bytecode;
  for(I = bytecodeAnonyFunc.begin(); I != bytecodeAnonyFunc.end(); I++) {
    bytecode = (*I).first;
    if (bytecode == pc)
     return (*I).second;
  }
  return NULL;
}

bool Scope::IsFunction(char *name) {
  std::vector<char *>::iterator I;
  for(I = funcNames_.begin(); I != funcNames_.end(); I++) {
    if(strcmp(*I, name) == 0) {
      return true;
    }
  }
  return false;
}

void Scope::DumpScopeChain() {
  std::list<std::pair<char *, ScopeNode *>>::iterator I;
  ScopeNode *sn;
  for(I = scopeChain.begin(); I != scopeChain.end(); I++) {
    sn = (*I).second;
    sn->Dump();
  }
}

void Scope::PopulateSNInfo() {
  std::list<std::pair<char *, ScopeNode *>>::iterator I;
  ScopeNode *sn;
  for(I = scopeChain.begin(); I != scopeChain.end(); I++) {
    sn = (*I).second;
    // set leaf
    if (sn->GetChildren().size() == 0) {
      sn->SetLeaf();
      sn->SetWithEnv(false);
    }
  }

  for(I = scopeChain.begin(); I != scopeChain.end(); I++) {
    sn = (*I).second;
    // propagate closure
    if (sn->IsWithEnv() || sn->UseAliased())
      sn->PropWithEnv();
  }
}

}  // namespace mapleir
