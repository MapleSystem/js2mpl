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
#ifndef JS2MPL_INCLUDE_SCOPE_H_
#define JS2MPL_INCLUDE_SCOPE_H_
#include <assert.h>
#include <vector>
#include "js/src/jsscript.h"
#include "js/src/jsopcode.h"
#include "js/src/jsfun.h"
#include "js/src/jsatom.h"
#include "maple_ir/include/mir_nodes.h"
#include "../include/jsfunction.h"
#include "../include/jsmirbuilder.h"
#include "../include/util.h"

namespace maple {
using namespace std;

class ScopeNode {
 private:
  ScopeNode *root_;
  BaseNode *node_;
  JSMIRFunction *func_;
  char *name_;
  ScopeNode *parent_;
  list<ScopeNode *> children_;
  bool isLeaf_;
  bool isTopLevel_;
  bool useAliased_;
  bool withEnv_;
  bool flag_;
  uint32_t id_;

 public:
  ScopeNode(BaseNode *node);
  ScopeNode(JSMIRFunction *func);
  ScopeNode(char *name);

  void clear();
  BaseNode *GetNode() {
    return node_;
  }

  JSMIRFunction *GetFunc() {
    return func_;
  }

  JSMIRFunction *GetParentFunc() {
    return parent_->func_;
  }

  void SetFunc(JSMIRFunction *func);
  void SetName(char *name) {
    name_ = name;
  }

  char *GetName() {
    return name_;
  }

  ScopeNode *GetParent() {
    return parent_;
  }

  void SetParent(ScopeNode *node);
  list<ScopeNode *> GetChildren() {
    return children_;
  }

  void AddChild(ScopeNode *node);
  bool SetChild(ScopeNode *node);
  void SetUseAliased() {
    useAliased_ = true;
  }

  bool UseAliased() {
    return useAliased_;
  }

  void SetWithEnv(bool b) {
    withEnv_ = b;
  }

  bool IsWithEnv() {
    return withEnv_;
  }

  void SetLeaf() {
    isLeaf_ = true;
  }

  bool IsLeaf() {
    return isLeaf_;
  }

  void SetTopLevel() {
    isTopLevel_ = true;
  }

  bool IsTopLevel() {
    return isTopLevel_;
  }

  void SetFlag() {
    flag_ = true;
  }

  bool GetFlag() {
    return flag_;
  }

  bool IsMain() {
    return !strcmp(name_, "__jsmain");
  }

  uint32_t GetId() {
    return id_;
  }

  void SetId(uint32_t id) {
    id_ = id;
  }

  void Dump();
  void PropWithEnv();
};

typedef list<ScopeNode *> ScopeNodeList;

class Scope {
 private:
  JSContext *ctx_;
  JSScript *jsscript_;
  MemPool *mp_;
  JSMIRBuilder *jsbuilder_;
  vector<char *> funcNames_;
  stack<char *> funcstack_;
  stack<pair<JSScript *, char *>> scriptstack_;

  uint32_t anon_func_no_;
  uint32_t nextAvailId_;
  int stackDepth;

  map<JSFunction *, unsigned> funcToAnonyidx_;
  map<unsigned, unsigned> anonyidxToLineNum_;
  vector<pair<char *, JSFunction *>> nameJSfunc_;
;
 public:
  Scope(JSContext *context, JSScript *script, maple::MIRModule *module, JSMIRBuilder *jsbuilder)
    : mp_(module->memPool), anon_func_no_(0), stackDepth(0), ctx_(context), jsbuilder_(jsbuilder), jsscript_(script), nextAvailId_(0) {}

  void Init();
  bool Build(JSScript *script);
  bool BuildSection(JSScript *script, jsbytecode *pcstart, jsbytecode *pcend);

  uint32_t AllocNextAvailScopeId() {
    return nextAvailId_++;
  }

  unsigned GetAnonyidx(JSFunction *jsfun) {
    return funcToAnonyidx_[jsfun];
  }

  unsigned GetOrCreateAnonyidx(JSFunction *jsfun) {
    if (funcToAnonyidx_[jsfun]) {
      return funcToAnonyidx_[jsfun];
    } else {
      funcToAnonyidx_[jsfun] = ++anon_func_no_;
      return funcToAnonyidx_[jsfun];
    }
  }

  void SetAnonyidx(JSFunction *jsfun, unsigned idx) {
    funcToAnonyidx_[jsfun] = idx;
  }

  unsigned GetAnonyidxLineNum(unsigned anonyidx) {
    return anonyidxToLineNum_[anonyidx];
  }

  void SetAnonyidxLineNum(unsigned anonyidx, unsigned lineNum) {
    anonyidxToLineNum_[anonyidx] = lineNum;
  }

  list<pair<char *, ScopeNode *>> scopeChain;
  list<pair<jsbytecode *, char *>> bytecodeAnonyFunc;

  ScopeNode *GetOrCreateSN(char *name);
  void SetSNParent(char *name, char *parent);
  void SetSNLeaf(char *name);
  void SetSNClosure(char *name);
  void GetSNFlag(char *name);
  void AddSNChild(char *name, char *child);

  char *GetJSFuncName(JSFunction *func);
  JSFunction *GetJSFunc(char *name);
  void SetJSFunc(char *name, JSFunction *func);

  void PopulateSNInfo();

  void DumpScopeChain();

  char *GetAnonyFunctionName(jsbytecode *pc);
  bool IsFunction(char *name);
  int GetDepth() {
    return stackDepth;
  }

  void DecDepth() {
    stackDepth--;
  }

  void GetJSOPOperand(JSOp op, JSScript *script, jsbytecode *pc, std::string &str);
};

}  // namespace maple
#endif  // JS2MPL_INCLUDE_SCOPE_H_
