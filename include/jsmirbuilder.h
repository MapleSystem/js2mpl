/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_JSMIRBUILDER_H_
#define JS2MPL_INCLUDE_JSMIRBUILDER_H_
#include "mapleir/include/mirbuilder.h"
#include "js2mpl/include/scope.h"
namespace mapleir {

class JSMIRBuilder : public MIRBuilder {
 private:
  JSMIRFunction *main_func_;
  JSMIRFunction *jscurrfunc;
  std::set<char *> global_vars;

 public:
  JSMIRFunction *jsmain_;
  MIRType *jsvalue_type_;
  MIRType *jsvalue_ptr_;

 public:
  explicit JSMIRBuilder(MIRModule *module):MIRBuilder(module) {}

  JSMIRFunction *GetFunction(const char *name);
  JSMIRFunction *GetOrCreateFunction(const char *name,
                                     MIRType *return_type,
                                     ArgVector arguments,
                                     bool isvarg);
  BaseNode *CreateStmtReturn(BaseNode *rval, bool adj_type);
  BaseNode *CreateStmtDassign(MIRSymbol *symbol, uint32_t field_id, BaseNode *src);
  BaseNode *CreateStmtIcall(BaseNode *puptrexp, MapleVector<BaseNode *> args);
  BaseNode *CreateStmtIntrinsicCall1N( MIRIntrinsicId idx,
                                       BaseNode *arg0,
                                       MapleVector<BaseNode *> &args);

  void UpdateFunction(JSMIRFunction *fn, MIRType *return_type, ArgVector arguments);
  void SaveReturnValue(MIRSymbol *var);
  void AddStmtInCurrentFunctionBody(BaseNode *n);

  std::vector<std::pair<const char *, JSMIRFunction *>> name_func_vec_;

  JSMIRFunction * GetNameFunc(const char *name ) {
    std::vector<std::pair<const char *, JSMIRFunction *>>::iterator I;
    for (I = name_func_vec_.begin(); I != name_func_vec_.end(); I++)
      if (strcmp(name, I->first) == 0)
        return I->second;
    return NULL;
  }

  void AddNameFunc(const char *name, JSMIRFunction *func) {
    JSMIRFunction * f = GetNameFunc(name);
    if (f) {
      assert (f==func && "error: function not match!");
    } else {
      std::pair<const char *, JSMIRFunction *> P(name, func);
      name_func_vec_.push_back(P);
    }
  }

  void InitGlobalName() { global_vars.empty(); }
  void InsertGlobalName(char *name) {
    if (IsGlobalName(name))
      return;
    DEBUGPRINTsv2("global_var", name);
    global_vars.insert(name);
  }
  bool IsGlobalName(char *name) {
    std::set<char *>::iterator BI;
    for (BI = global_vars.begin(); BI != global_vars.end(); BI++) {
      if (strcmp(name, *BI) == 0) {
        DEBUGPRINTsv2("global_var", name);
        return true;
      }
    }
    DEBUGPRINTsv2("local_var", name);
    return false;
  }

  // Initializations.
  MIRType *CreateJSValueType();
  JSMIRFunction *CreateJSMain();
  void InitBuiltinMethod();
  void Init();

  bool IsMain(MIRFunction *func) { return func == (MIRFunction *)main_func_; }
  bool SetMain(JSMIRFunction *func) { main_func_ = func; }
  JSMIRFunction *GetCurrentFunction() { (JSMIRFunction *)(MIRBuilder::GetCurrentFunction()); }
};
}   // namespace mapleir
#endif  // JS2MPL_INCLUDE_JSMIRBUILDER_H_
