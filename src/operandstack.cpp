/// Copyright [year] <Copyright Owner>
#include "../include/operandstack.h"
#include "../include/compiler.h"

namespace mapleir {

// for each stack item that may contain the variable var, evaluate and store 
// the result in a new temp and replace the stack items by that temp
void OperandStack::ReplaceStackItemsWithTemps(JSCompiler *compiler,
                                              MIRSymbol *var) {
  for (unsigned i = 0; i < current_depth_; i++) {
    BaseNode *cur = (BaseNode *)stack_[i];
    if (cur == NULL)
      continue;
    if (GenericHasSymbol(compiler->module_, cur, var))
      stack_[i] = (void *) compiler->NodeFromSavingInATemp(cur);
  }
}

}  // namespace mapleir
