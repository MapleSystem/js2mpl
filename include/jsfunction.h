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
#ifndef JS2MPL_INCLUDE_JSFUNCTION_H_
#define JS2MPL_INCLUDE_JSFUNCTION_H_
#include "maple_ir/include/mir_function.h"
namespace maple {

class ScopeNode;
class JSClosure;

class JSMIRFunction : public MIRFunction {
 public:
  JSMIRFunction(MIRModule *mod, StIdx stidx) : MIRFunction(mod, stidx) {}

 public:
  bool with_env_arg;
  bool env_setup;
  bool dup_name;
  bool is_lambda;
  int argc;

  std::list<GStrIdx> alias_vars;
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
    dup_name = false;
    is_lambda = false;
    alias_vars.empty();
    scope = NULL;
    envtype = NULL;
    envptr = NULL;
    penvtype = NULL;
    penvptr = NULL;
    parent = NULL;
  }

  bool isLambda() {
    return is_lambda;
  }

  void initAliasList() {
    alias_vars.empty();
  }

  void AddToAliasList(GStrIdx);
  int GetAliasPosition(GStrIdx);
};
}  // namespace maple
#endif  // JS2MPL_INCLUDE_JSFUNCTION_H_
