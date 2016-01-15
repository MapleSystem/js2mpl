/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_JSCLOSURE_H_
#define JS2MPL_INCLUDE_JSCLOSURE_H_
#include <assert.h>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include "mapleir/include/opcodes.h"
#include "mapleir/include/primtypes.h"
#include "mapleir/include/mirtype.h"
#include "mapleir/include/mirconst.h"
#include "mapleir/include/mirstring.h"
#include "mapleir/include/mirsymbol.h"
#include "mapleir/include/mirnodes.h"
#include "mapleir/include/mirmodule.h"
#include "mapleir/include/mirpreg.h"
#include "mapleir/include/printing.h"
#include "../include/jsfunction.h"
#include "../include/jsmirbuilder.h"
#include "../include/scope.h"
#include "../include/operandstack.h"
namespace mapleir {

class JSClosure {
  typedef std::pair<JSMIRFunction *, std::vector<char *>> funcArgPair;

  private:
    const char *filename_;
    JSContext *jscontext_;
    JSScript *jsscript_;
    MIRModule *module_;
    MemPool *mp_;
    Scope *scope_;
    JSMIRBuilder *jsbuilder_;
    OperandStack *opstack_;

    std::stack<std::pair<JSScript *, JSMIRFunction *>> scriptstack_;

  public:
    explicit JSClosure(const char *filename,
                       JSContext *context,
                       JSScript *script,
                       mapleir::MIRModule *module,
                       Scope *scope,
                       JSMIRBuilder *jsbuilder,
                       OperandStack *opstack):
      filename_(filename),
      jscontext_(context),
      jsscript_(script),
      scope_(scope),
      mp_(module->mp_),
      jsbuilder_(jsbuilder),
      opstack_(opstack),
      temp_var_no_(0) {}

    uint32_t temp_var_no_;
    JSScript *currscr_;
    std::stack<JSMIRFunction *> funcstack_;
    JSMIRFunction *jsmain_;
    MIRType *jsvalue_type_;
    MIRType *jsvalue_ptr_;
    std::vector<funcArgPair> funcFormals;
    std::vector<char *> funcMod;
    std::vector<std::pair<char *, JSMIRFunction *>> nameJSMIRfunc_;

    void Init();
    bool Build(JSScript *script);
    bool BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend);

    void CloseFuncBookKeeping();

    MIRType *GetOrCreateEnvType(JSMIRFunction *func);
    void AddAliasToEnvType(MIRType *env_type, char *name, MIRType *T);

    MIRSymbol *GetSymbolFromEnclosingScope(JSMIRFunction *func, const char *name);
    MIRSymbol *GetSymbolFromEnclosingScope(JSMIRFunction *func, stidx_t stidx);

    void ProcessAliasedVar(jsbytecode *pc);
    JSMIRFunction *ProcessFunc(JSFunction *jsfun, char *funcname);
    void ProcessOpLambda(jsbytecode *pc);
    bool ProcessOpDefFun(jsbytecode *pc);
    void AddFuncFormalsToEnvType(JSMIRFunction *func);
    bool IsLocalVar(JSMIRFunction *func, char *name);
    char *GetLocalVar(JSMIRFunction *func, uint32_t local_no);

    bool IsFuncWithEnv(char *name);
    void UpdateFuncMod(char *name);
    bool IsFuncModified(char *name);

    JSMIRFunction *GetJSMIRFunc(char *name);
    void SetJSMIRFunc(char *name, JSMIRFunction *);
};
}   // namespace mapleir
#endif  // JS2MPL_INCLUDE_JSCLOSURE_H_
