/// Copyright [year] <Copyright Owner>
#include "../include/scope.h"

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
    list<ScopeNode *>::iterator I;
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
    list<ScopeNode *>::iterator I;
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
    list<ScopeNode *>::iterator I;
    ScopeNode *child;
    for (I = children_.begin(); I != children_.end(); I++) {
      child = *I;
      DEBUGPRINT2pure(child);
    }
    DEBUGPRINT0;
  }

ScopeNode *Scope::GetOrCreateSN(char *name) {
  list<pair<char *, ScopeNode *>>::iterator I;
  for(I = scopeChain.begin(); I != scopeChain.end(); I++) {
    if(strcmp(name, (*I).first) == 0) {
      return (*I).second;
    }
  }

  // create new one
  ScopeNode *sn = MP_NEW(mp_, ScopeNode(name));
  pair<char *, ScopeNode *> P(name, sn);
  scopeChain.push_back(P);

  return sn;
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
  typedef pair<JSFunction *, vector<JSAtom *>> funcVarVecPair;
  vector<funcVarVecPair> formals = jsscript_->funcFormals;
  char *name;
  char *funcname;
  vector<funcVarVecPair>::iterator I;
  vector<char *> nameVec;
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

EHstruct *Scope::GetEHstruct(jsbytecode *trypc, jsbytecode *catchpc,
                             jsbytecode *finallypc, jsbytecode *endtrypc) {
  vector<EHstruct *>::iterator I;
  for(I = EHstructvec_.begin(); I != EHstructvec_.end(); I++) {
    if((trypc && (*I)->trypc == trypc) ||
       (catchpc && (*I)->catchpc == catchpc) ||
       (finallypc && (*I)->finallypc == finallypc) ||
       (endtrypc && (*I)->endtrypc == endtrypc)) {
      return *I;
    }
  }
  return NULL;
}

bool Scope::IsInEHrange(jsbytecode *pc) {
  vector<EHstruct *>::iterator I;
  for(I = EHstructvec_.begin(); I != EHstructvec_.end(); I++) {
    if (pc >= (*I)->trypc && pc <= (*I)->endtrypc)
      return true;
  }
  return false;
}

void Scope::DumpEHstruct() {
  vector<EHstruct *>::iterator I;
  for(I = EHstructvec_.begin(); I != EHstructvec_.end(); I++) {
    printf("         EHstruct { try=0x%x, catch=0x%x, finally=0x%x, endtry=0x%x }\n",
           (*I)->trypc, (*I)->catchpc, (*I)->finallypc, (*I)->endtrypc);
  }
}

bool Scope::Build(JSScript *script) {
  jsbytecode *start = script->code();
  jsbytecode *end = script->codeEnd();

  bool ret = BuildSection(script, start, end);

  if (js2mplDebug>1) cout << "GetDepth() = " << GetDepth() << endl;
  return ret;
}


bool Scope::BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend) {
  jsbytecode *pc = pcstart;
  JSFunction *jsfun;

  // dump JSOP code only
  if (jsbuilder_->JSOPOnly()) {
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
      cout << Util::getOpcodeName[op] << endl;
      pc = js::GetNextPc(pc);
    }

  } else {

    if (script == jsscript_) {
      funcstack_.push("main");
      if (js2mplDebug>0) cout << "main {" << endl;
      ScopeNode * sn = GetOrCreateSN("main");
      sn->SetTopLevel();
    }

    char *name;
    char *parent;
    JSOp lastOp;
    JSScript *scr;

    while (pc < pcend) {
      JSOp op = JSOp(*pc);
      unsigned lineNo = js::PCToLineNumber(script, pc);

      if (js2mplDebug>0) printf("  %4d %-25s pc = 0x%x\n", lineNo, Util::getOpcodeName[op], pc);
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
              pair<jsbytecode *, char *> P(pc, name);
              bytecodeAnonyFunc.push_back(P);
            }
            DEBUGPRINT3(name);
            funcNames_.push_back(name);
            SetJSFunc(name, jsfun);
            pair<JSScript *, char *> P(scr, name);
            scriptstack_.push(P);

            parent = funcstack_.top();
            SetSNParent(name, parent);
            break;
          }
        case JSOP_GETALIASEDVAR: /*136, 5, 0, 1*/
        case JSOP_SETALIASEDVAR: /*137, 5, 11*/
          {
            name = funcstack_.top();
            SetSNClosure(name);
            break;
          }
      }

      // collecting EH info
      switch (op) {
        case JSOP_TRY: {
          JSTryNote *tn = script->trynotes()->vector;
          JSTryNote *tnlimit = tn + script->trynotes()->length;
          for (; tn < tnlimit; tn++) {
            if ((tn->start + script->mainOffset()) == (pc - script->code() + 1)) {
              jsbytecode *trythrow = pc + 1 + tn->length;
              trystack_.push(pc);

              // use the goto before trythrow to find the end of try combo
              jsbytecode *jumppc = trythrow - js_CodeSpec[JSOP_GOTO].length;
              assert(JSOp(*jumppc) == JSOP_GOTO);
              jsbytecode *aftertrypc = jumppc + GET_JUMP_OFFSET(jumppc);
              endtrystack_.push(aftertrypc);

              EHstruct *combo = (EHstruct *)malloc(sizeof(EHstruct));
              combo->trypc = pc;
              combo->catchpc = trythrow;
              combo->finallypc = 0;
              combo->endtrypc = aftertrypc;
              EHstructvec_.push_back(combo);
              break;
            }
          }
          break;
        }
        case JSOP_FINALLY: {
          EHstruct *combo = GetEHstruct(trystack_.top(), 0, 0, 0);
          assert(combo);
          combo->finallypc = pc;
          // check if no catch (catch == finally)
          if (combo->catchpc == pc) {
            combo->catchpc = 0;
          }
          break;
        }
      }

      // end of current try combo
      if (endtrystack_.size() && pc == endtrystack_.top()) {
        EHstruct *combo = GetEHstruct(trystack_.top(), 0, 0, 0);
        assert(combo);
        if (js2mplDebug>2) DumpEHstruct();

        trystack_.pop();
        endtrystack_.pop();
      }

      // calculate the expected stack size at the end
      {
        // get def/use for each op from mozjs js/src/vm/Opcodes.h
        int use = 0;
        int def = 0;
        switch (op) {
  #define JSOPDEPTH(op,val,name,token,length,nuses,ndefs,format)  case op: def=ndefs; use=nuses; break;
    FOR_EACH_OPCODE(JSOPDEPTH)
  #undef JSOPDEPTH
        }

        // handles dynamic use count (-1)
        uint32_t use0 = 0;
        switch (op) {
          case JSOP_POPN:
            use0 = GET_UINT16(pc);
            use0 = 0;   // adjustment
            break;
          case JSOP_CALL:
          case JSOP_FUNAPPLY:
          case JSOP_NEW:
          case JSOP_FUNCALL:
          case JSOP_EVAL:
            use0 = GET_ARGC(pc);
            use0 += 2;   // adjustment
            break;
        }

        use = (use < 0) ? use0 : use;
        int inc = def - use;

        if (js2mplDebug>3) cout << "line : " << lineNo << "  " << Util::getOpcodeName[op]
                                     << "  stackDepth: " << stackDepth
                                     << " == (u" << use << ", d" << def << ")==>"
                                     << stackDepth+inc << endl;
        stackDepth += inc;
      }

      lastOp = op;
      pc = js::GetNextPc(pc);
    }

    if (lastOp == JSOP_RETRVAL) {
      name = funcstack_.top();
      if (js2mplDebug>0) cout << "}\n" << endl;
      funcstack_.pop();
      DEBUGPRINT3((scriptstack_.size()));
      while (scriptstack_.size()) {
        JSScript *scr = scriptstack_.top().first;
        name = scriptstack_.top().second;
        scriptstack_.pop();
        funcstack_.push(name);
        if (js2mplDebug>0) cout << name << " {" << endl;
          Build(scr);
      }
    }

    PopulateSNInfo();
  }

  return true;
}

char *Scope::GetAnonyFunctionName(jsbytecode *pc) {
  list<pair<jsbytecode *, char *>>::iterator I;
  jsbytecode *bytecode;
  for(I = bytecodeAnonyFunc.begin(); I != bytecodeAnonyFunc.end(); I++) {
    bytecode = (*I).first;
    if (bytecode == pc)
     return (*I).second;
  }
  return NULL;
}

bool Scope::IsFunction(char *name) {
  vector<char *>::iterator I;
  for(I = funcNames_.begin(); I != funcNames_.end(); I++) {
    if(strcmp(*I, name) == 0) {
      return true;
    }
  }
  return false;
}

void Scope::DumpScopeChain() {
  list<pair<char *, ScopeNode *>>::iterator I;
  ScopeNode *sn;
  for(I = scopeChain.begin(); I != scopeChain.end(); I++) {
    sn = (*I).second;
    sn->Dump();
  }
}

void Scope::PopulateSNInfo() {
  list<pair<char *, ScopeNode *>>::iterator I;
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

JSFunction *Scope::GetJSFunc(char *name) {
  vector<pair<char *, JSFunction *>>::iterator I;
  for(I = nameJSfunc_.begin(); I != nameJSfunc_.end(); I++) {
    if(strcmp(name, (*I).first) == 0) {
      return (*I).second;
    }
  }
  return NULL;
}

void Scope::SetJSFunc(char *name, JSFunction *func) {
  if (GetJSFunc(name))
    return;
  pair<char *, JSFunction *> P(name, func);
  nameJSfunc_.push_back(P);
}

}  // namespace mapleir
