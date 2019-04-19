/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_JSFUNCTION_H_
#define JS2MPL_INCLUDE_JSFUNCTION_H_
#include "mapleir/include/mirfunction.h"
namespace maple {

class ScopeNode;
class JSClosure;

class JSMIRFunction : public MIRFunction {
 public:
  JSMIRFunction(MIRModule *mod, stidx_t stidx) : MIRFunction(mod, stidx) {}

 public:
  bool with_env_arg;
  bool env_setup;

  int argc;

  std::list<gstridx_t> alias_vars;
  ScopeNode *scope;

  MIRType *envtype;
  MIRType *envptr;
  MIRType *penvtype;  // parent env type
  MIRType *penvptr;   // parent env pointer type

  JSMIRFunction *parent;

 public:
  void Init() {
    with_env_arg = false;
    env_setup = false;
    alias_vars.empty();
    scope = NULL;
    envtype = NULL;
    envptr = NULL;
    penvtype = NULL;
    penvptr = NULL;
    parent = NULL;
  }

  void initAliasList() {
    alias_vars.empty();
  }

  void AddToAliasList(gstridx_t);
  int GetAliasPosition(gstridx_t);
};
}  // namespace maple
#endif  // JS2MPL_INCLUDE_JSFUNCTION_H_
