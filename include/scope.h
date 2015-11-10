/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_SCOPE_H_
#define JS2MPL_INCLUDE_SCOPE_H_
#include <assert.h>
#include <vector>
#include "js/src/jsscript.h"
#include "js/src/jsopcode.h"
#include "js/src/jsfun.h"
#include "js/src/jsatom.h"
#include "mapleir/include/mirnodes.h"
#include "../include/jsfunction.h"
#include "../include/util.h"

namespace mapleir {
class ScopeNode {
  private:
    ScopeNode *root_;
    BaseNode *node_;
    JSMIRFunction *func_;
    char *name_;
    ScopeNode *parent_;
    std::list<ScopeNode *> children_;
    bool isLeaf_;
    bool isTopLevel_;
    bool useAliased_;
    bool withEnv_;
    bool flag_;

  public:
    ScopeNode(BaseNode *node);
    ScopeNode(JSMIRFunction *func);
    ScopeNode(char *name);

    void clear();
    BaseNode * GetNode() { return node_; }
    JSMIRFunction * GetFunc() { return func_; }
    JSMIRFunction * GetParentFunc() { return parent_->func_; }
    void SetFunc(JSMIRFunction *func);
    void SetName(char *name) { name_ = name; }
    char * GetName() { return name_; }
    ScopeNode * GetParent() { return parent_; }
    void SetParent(ScopeNode *node);
    std::list<ScopeNode *> GetChildren() { return children_; }
    void AddChild(ScopeNode *node);
    bool SetChild(ScopeNode *node);
    void SetUseAliased() { useAliased_ = true; }
    bool UseAliased() { return useAliased_; }
    void SetWithEnv(bool b) { withEnv_ = b; }
    bool IsWithEnv() { return withEnv_; }
    void SetLeaf() { isLeaf_ = true; }
    bool IsLeaf() { return isLeaf_; }
    void SetTopLevel() { isTopLevel_ = true; }
    bool IsTopLevel() { return isTopLevel_; }
    void SetFlag() { flag_ = true; }
    bool GetFlag() { return flag_; }
    bool IsMain() { return !strcmp(name_, "main"); }

    void Dump();
    void PropWithEnv();
};

typedef std::list<ScopeNode *> ScopeNodeList;

class Scope {
  private:
    JSContext *ctx_;
    JSScript *jsscript_;
    MemPool *mp_;
    std::vector<char *> funcNames_;
    std::stack<char *> funcstack1_;
    std::stack<std::pair<JSScript *, char *>> scriptstack_;

    uint32_t anon_func_no_;

    std::map<JSFunction *, unsigned> funcToAnonyidx_;

  public:
    Scope(JSContext *context, JSScript *script, mapleir::MIRModule *module) : 
      mp_(module->mp_),
      anon_func_no_(0),
      ctx_(context),
      jsscript_(script) {}

    void Init();
    bool Build(JSScript *script);
    bool BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend);

    unsigned GetAnonyidx(JSFunction *jsfun) { return funcToAnonyidx_[jsfun]; }
    void SetAnonyidx(JSFunction *jsfun, unsigned idx) { funcToAnonyidx_[jsfun] = idx; }

    std::list<std::pair<char *, ScopeNode *>> scopeChain;
    std::list<std::pair<jsbytecode *, char *>> bytecodeAnonyFunc;

    ScopeNode *GetOrCreateSN(JSMIRFunction *func);
    ScopeNode *GetOrCreateSN(char *name);
    void SetSNParent(char *name, char *parent);
    void SetSNLeaf(char *name);
    void SetSNClosure(char *name);
    void GetSNFlag(char *name);
    void AddSNChild(char *name, char *child);

    void PopulateSNInfo();

    void DumpScopeChain();

    char *GetAnonyFunctionName(jsbytecode *pc);
    bool IsFunction(char *name);
};

}  // namespace mapleir
#endif  // JS2MPL_INCLUDE_SCOPE_H_


