/*
 * Copyright (C) [2021] Futurewei Technologies, Inc. All rights reserved.
 *
 * OpenArkCompiler is licensed under the Mulan Permissive Software License v2.
 * You can use this software according to the terms and conditions of the MulanPSL - 2.0.
 * You may obtain a copy of MulanPSL - 2.0 at:
 *
 *   https://opensource.org/licenses/MulanPSL-2.0
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the MulanPSL - 2.0 for more details.
 */
#include "../include/operandstack.h"
#include "../include/compiler.h"

namespace maple {

// for each stack item that may contain the variable var, evaluate and store
// the result in a new temp and replace the stack items by that temp
void OperandStack::ReplaceStackItemsWithTemps(JSCompiler *compiler, MIRSymbol *var) {
  for (unsigned i = 0; i < current_depth_; i++) {
    BaseNode *cur = (BaseNode *)stack_[i];
    if (cur == NULL) {
      continue;
    }
    if (cur->HasSymbol(compiler->mirModule, var)) {
      stack_[i] = (void *)compiler->NodeFromSavingInATemp(cur);
    }
  }
}

bool OperandStack::HasTheSameGetThisProp(BaseNode *cur, BaseNode *destNode) {
  if (cur->op == OP_intrinsicop) {
    IntrinsicopNode *intrinNode = static_cast<IntrinsicopNode *>(cur);
    if (intrinNode->intrinsic == INTRN_JSOP_GET_THIS_PROP_BY_NAME &&
      intrinNode->nOpnd[0] == destNode) {
      return true;
    }
  }
  if (UnaryNode *unNode = dynamic_cast<UnaryNode *>(cur)) {
    return HasTheSameGetThisProp(unNode->uOpnd, destNode);
  } else if (BinaryNode *bNode = dynamic_cast<BinaryNode *>(cur)) {
    return HasTheSameGetThisProp(bNode->bOpnd[0], destNode) ||
           HasTheSameGetThisProp(bNode->bOpnd[1], destNode);
  } else if (NaryNode * nNode = dynamic_cast<NaryNode *>(cur)) {
    for (uint32_t i = 0; i < nNode->nOpnd.size(); i++) {
      if (HasTheSameGetThisProp(nNode->nOpnd[i], destNode))
        return true;
    }
  }
  return false;
}

void OperandStack::ReplaceStackItemsWithThisOpTemps(JSCompiler *compiler, JSString *jsstr) {
  BaseNode *destNode = compiler->GetCompileOpString(jsstr);
  if (!destNode)
    return;
  for (unsigned i = 0; i < current_depth_; i++) {
    BaseNode *cur = (BaseNode *)stack_[i];
    if (cur == NULL) {
      continue;
    }
    if (HasTheSameGetThisProp(cur, destNode)) {
      stack_[i] = (void *)compiler->NodeFromSavingInATemp(cur);
    }
    /*
    if (cur->op == OP_intrinsicop) {
      IntrinsicopNode *intrinNode = static_cast<IntrinsicopNode *>(cur);
      if (intrinNode->intrinsic == INTRN_JSOP_GET_THIS_PROP_BY_NAME) {
        BaseNode *nmNode = compiler->CompileOpString(jsstr);
        if(nmNode == intrinNode->nOpnd[0]) {
          stack_[i] = (void *)compiler->NodeFromSavingInATemp(cur);
        }
      }
    }
    */
    // if (cur->HasSymbol(compiler->mirModule, var)) {
    //  stack_[i] = (void *)compiler->NodeFromSavingInATemp(cur);
    // }
  }
}

}  // namespace maple
