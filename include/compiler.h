/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_COMPILER_H_
#define JS2MPL_INCLUDE_COMPILER_H_
#include <list>

#include "mapleir/include/mirbuilder.h"
#include "../include/jsvalue.h"
#include "../include/jsmirbuilder.h"
#include "../include/closure.h"
#include "../include/operandstack.h"
#include "../include/scope.h"
#include "../include/eh.h"
#include "../include/util.h"

namespace mapleir {
class JSCompiler{
 private:
  const char *filename_;
  JSContext *jscontext_;
  JSScript *jsscript_;
  JSMIRBuilder *jsbuilder_;

  MIRType *jsvalue_type_;
  MIRType *jsvalue_ptr_;

  JSMIRFunction *jsmain_;
  OperandStack *opstack_;
  std::stack<JSMIRFunction *> funcstack_;
  std::stack<BlockNode *> bodystack_;
  std::map<jsbytecode *, labidx_t> label_map_;
  std::map<labidx_t, MIRSymbol *> label_tempvar_map_;

  std::map<const jschar *, base_node_t *> jsstring_map_;

  std::stack<std::pair<JSScript *, JSMIRFunction *>> scriptstack_;

  MemPool *mp_;
  mapleir::MIRModule *module_;
  base_node_t *dummyNode;

  uint32_t anon_func_no_;
  uint32_t temp_var_no_;

  Scope *scope_;
  EH *eh_;
  JSClosure *closure_;

  typedef std::pair<JSMIRFunction *, std::vector<char *>> funcArgPair;
  std::vector<funcArgPair> funcFormals;
  std::vector<std::pair<char*, char*>> objFuncMap;

 public:
  explicit JSCompiler(const char *filename, 
                      JSContext *context,
                      JSScript *script, 
                      mapleir::MIRModule *module, 
                      JSMIRBuilder *jsbuilder,
                      Scope *scope,
                      EH *eh,
                      JSClosure *closure,
                      OperandStack *opstack):
      filename_(filename),
      jscontext_(context),
      jsscript_(script),
      module_(module),
      mp_(module->mp_),
      temp_var_no_(1),
      jsbuilder_(jsbuilder),
      scope_(scope),
      eh_(eh),
      closure_(closure),
      opstack_(opstack){}

  void Push(base_node_t *node) { assert(node->op != OP_dassign); opstack_->Push(node); }
  base_node_t *Pop() {
    base_node_t *bn = static_cast <base_node_t *>(opstack_->Pop());
    assert(bn);
    return bn;
  }

  base_node_t *Top() {
    base_node_t *bn = static_cast <base_node_t *>(opstack_->Top());
    assert(bn);
    return bn;
  }
  base_node_t *GetOpAt(uint32_t n) {
    base_node_t *bn = static_cast <base_node_t *>(opstack_->GetOpAt(n));
    assert(bn);
    return bn;
  }

  bool UseSimpleCall(char *name) { 
    return jsbuilder_->UseSimpCall() &&
           name && scope_->IsFunction(name) &&
           !closure_->FuncUseEnv(name) &&
           !closure_->IsFuncModified(name);
  }

  char *GetFuncName(char *objname) {
    std::vector<std::pair<char*, char*>>::iterator I;
    for (I = objFuncMap.begin(); I != objFuncMap.end(); I++) {
      if (!strcmp(objname, (*I).first)) {
        return (*I).second;
      }
    }
    return NULL;
  }

  void Init();
  void EnvInit(JSMIRFunction *func);
  void SetupMainFuncRet(base_node_t *rval);
  void CloseFuncBookKeeping();
  int32_t GetBuiltinMethod(uint32_t argc, bool *need_this);
  MIRSymbol *CreateTempVar(MIRType *);
  MIRSymbol *CreateTempJSValueTypeVar();
  void InitWithUndefined(bool created, MIRSymbol *var);
  uint32_t GetFieldidFromTag(uint32_t tag);
  MIRType *DetermineTypeFromNode(base_node_t *node);
  MIRSymbol *SymbolFromSavingInATemp(base_node_t *expr, bool jsvalue_p);
  AddrofNode *NodeFromSavingInATemp(base_node_t *expr);
  // Compile Functions.
  BaseNode *CompileOpConstValue(uint32_t jsvalue_tag, int32_t payload);
  js_builtin_id EcmaNameToId(char *name);
  int32_t GetBuiltinStringId(const jschar *chars, uint32_t length);
  base_node_t *CompileBuiltinObject(char *name);
  base_node_t *CompileBuiltinMethod(int32_t idx, int arg_num, bool need_this);
  uint32_t FindIntrinsicForOp(JSOp opcode);
  base_node_t *CompileOpBinary(JSOp op, base_node_t *opnd0, base_node_t *opnd1);
  base_node_t *CompileOpUnary(JSOp op, base_node_t *opnd);
  base_node_t *CompileGenericN(int32_t intrin_id,
                            MapleVector<base_node_t *> &arguments,
                            bool is_call, bool retvalOK = false);
  base_node_t *CompileGeneric0(int32_t intrin_id,
                            bool is_call, bool retvalOK = false);
  base_node_t *CompileGeneric1(int32_t intrin_id, base_node_t *arg,
                            bool is_call, bool retvalOK = false);
  base_node_t *CompileGeneric2(int32_t intrin_id, base_node_t *arg0,
                            base_node_t *arg1, bool is_call, bool retvalOK = false);
  base_node_t *CompileGeneric3(int32_t intrin_id, base_node_t *arg0,
                            base_node_t *arg1, base_node_t *arg2, bool is_call, bool retvalOK = false);
  base_node_t *CompileGeneric4(int32_t intrin_id, base_node_t *arg0,
                            base_node_t *arg1, base_node_t *arg2, base_node_t *arg3, bool is_call, bool retvalOK = false);
  base_node_t *CompileOpGetProp(base_node_t *obj, JSString *name);
  base_node_t *CompileOpCallprop(base_node_t *obj, JSAtom *atom);
  base_node_t *CompileOpString(JSString *str);
  base_node_t *CompileOpNewIterator(base_node_t *bn, uint8_t flags);
  base_node_t *CompileOpIterNext(base_node_t *iterator);
  base_node_t *CompileOpMoreIterator(base_node_t *iterator);
  base_node_t *CompileOpGetArg(uint32_t i);
  void CompileOpSetArg(uint32_t i, base_node_t *val);
  base_node_t *CompileOpGetLocal(uint32_t local_no);
  StmtNode *CompileOpSetLocal(uint32_t local_no, base_node_t *src);
  base_node_t *CompileOpNewInit(uint32_t kind);
  BaseNode *CompileOpNewArray(uint32_t length);
  BaseNode *CompileOpLength(base_node_t *array);
  base_node_t *CompileOpGetElem(base_node_t *obj, base_node_t *index);
  bool CompileOpSetElem(base_node_t *obj, base_node_t *index, base_node_t *val);
  bool CompileOpSetProp(base_node_t *obj, JSString *name, base_node_t *val);
  bool CompileOpInitPropGetter(base_node_t *obj, JSString *name, base_node_t *val);
  bool CompileOpInitPropSetter(base_node_t *obj, JSString *name, base_node_t *val);
  bool CompileOpInitElemGetter(base_node_t *obj, base_node_t *index, base_node_t *val);
  bool CompileOpInitElemSetter(base_node_t *obj, base_node_t *index, base_node_t *val);
  int ProcessAliasedVar(JSAtom *atom, MIRType *&env_ptr, base_node_t *&base, int &depth);
  base_node_t *CompileOpGetAliasedVar(JSAtom *atom);
  base_node_t *CompileOpSetAliasedVar(JSAtom *atom, base_node_t *val);
  base_node_t *CompileOpLambda(jsbytecode *pc, JSFunction *jsfun);
  base_node_t *CompileOpBindName(JSAtom *atom);
  base_node_t *CompileOpCall(uint32_t argc);
  base_node_t *CompileOpNew(uint32_t argc);
  base_node_t *CompileOpName(JSAtom *atom, jsbytecode *pc);
  base_node_t *CompileOpIfJump(JSOp op, base_node_t *cond, jsbytecode *pcend);
  
  labidx_t CreateLabel(char *pref = NULL);
  labidx_t GetorCreateLabelofPc(jsbytecode *pc, char *pref = NULL);
  int64_t GetIntValue(jsbytecode *pc);
  SwitchNode *CompileOpCondSwitch(base_node_t *opnd, JSScript *script,
                                jsbytecode *pcstart, jsbytecode *pcend);
  SwitchNode *CompileOpTableSwitch(base_node_t *opnd, int32_t len,
                                 JSScript *script, jsbytecode *pc);
  GotoNode *CompileOpGoto(jsbytecode *pc, jsbytecode *jumptopc, MIRSymbol *tempvar);
  GotoNode *CompileOpGosub(jsbytecode *pc);
  TryNode *CompileOpTry(jsbytecode *pc);
  BaseNode *CompileOpLoopHead(jsbytecode *pc);
  base_node_t *CheckConvertToJSValueType(base_node_t *node);
  base_node_t *CheckConvertToBoolean(base_node_t *node);
  base_node_t *CheckConvertToInt32(base_node_t *node);
  base_node_t *CheckConvertToUInt32(base_node_t *node);
  base_node_t *CheckConvertToRespectiveType(base_node_t *node, MIRType *ty);
  bool CompileOpSetName(JSAtom *atom, base_node_t *val);
  void CompileOpCase(jsbytecode *pc, int offset, base_node_t *rval,
		     base_node_t *lval);
  bool CompileOpDefFun(JSFunction *jsfun);
  bool CompileOpDefVar(JSAtom *atom);
  bool CompileScript(JSScript *script);
  bool CompileScriptBytecodes(JSScript *script, jsbytecode *pcstart,
                              jsbytecode *pcend, jsbytecode **newpc);
  bool MarkLabels(JSScript *s, jsbytecode *pc0, jsbytecode *pc1);
  bool CollectInfo(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend);
  // Finish job.
  void Finish();
};  // class JSCompiler
}  // namespace mapleir
#endif  // JS2MPL_INCLUDE_COMPILER_H_
