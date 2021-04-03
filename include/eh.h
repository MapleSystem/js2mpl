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
#ifndef JS2MPL_INCLUDE_EH_H
#define JS2MPL_INCLUDE_EH_H
#include <assert.h>
#include <vector>
#include "js/src/jsscript.h"
#include "js/src/jsopcode.h"
#include "js/src/jsfun.h"
#include "js/src/jsatom.h"
#include "maple_ir/include/mir_nodes.h"
#include "../include/jsfunction.h"
#include "../include/jsmirbuilder.h"
#include "../include/scope.h"
#include "../include/util.h"

namespace maple {
using namespace std;

struct EHstruct {
  jsbytecode *trypc;
  jsbytecode *catchpc;
  jsbytecode *finallypc;
  jsbytecode *endtrypc;
  LabelIdx label;
};

class EH {
 private:
  JSContext *ctx_;
  JSScript *jsscript_;
  MemPool *mp_;
  JSMIRBuilder *jsbuilder_;
  vector<char *> funcNames_;
  stack<char *> funcstack_;
  stack<pair<JSScript *, char *>> scriptstack_;

  stack<jsbytecode *> trystack_;
  vector<EHstruct *> EHstructvec_;

  Scope *scope_;

 public:
  EH(JSContext *context, JSScript *script, maple::MIRModule *module, JSMIRBuilder *jsbuilder, Scope *scope)
    : mp_(module->memPool), ctx_(context), jsbuilder_(jsbuilder), scope_(scope), jsscript_(script) {}

  bool Build(JSScript *script);
  bool BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend);

  // return the innermost EHstruct containing the pc if any
  EHstruct *GetEHstruct(jsbytecode *pc);
  // passing 0 to skip search fields
  EHstruct *GetEHstruct(jsbytecode *tryop, jsbytecode *catchop, jsbytecode *finallyop, jsbytecode *endtryop);

  void SetEHLabel(EHstruct *eh, LabelIdx lab) {
    eh->label = lab;
  }

  void DumpEHstruct(EHstruct *);
  void DumpEHstructVec();
  bool IsInEHrange(jsbytecode *pc) {
    return GetEHstruct(pc) != NULL;
  }
};

}  // namespace maple
#endif  // JS2MPL_INCLUDE_EH_H
