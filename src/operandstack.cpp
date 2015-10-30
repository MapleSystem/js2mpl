/// Copyright [year] <Copyright Owner>
#include "js2mpl/include/operandstack.h"
#include "js2mpl/include/compiler.h"

namespace mapleir {

// for each stack item that may contain the variable var, evaluate and store 
// the result in a new temp and replace the stack items by that temp
void OperandStack::ReplaceStackItemsWithTemps(JSCompiler *compiler,
                                              MIRSymbol *var) {
  for (unsigned i = 0; i < current_depth_; i++) {
    BaseNode *cur = (BaseNode *) stack_[i];
    if (cur == NULL)
      continue;
    if (cur->FindSymbol(var))
      stack_[i] = (void *) compiler->NodeFromSavingInATemp(cur);
  }
}

}  // namespace mapleir
