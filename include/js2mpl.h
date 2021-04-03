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
#ifndef JS2MPL_INCLUDE_JS2MPL_H_
#define JS2MPL_INCLUDE_JS2MPL_H_
#include <string.h>
using namespace std;
namespace maple {
class MIRModule;
struct JSMIRContext {
  bool isplugin_;
  const string &wrapper_name_;
  bool with_main_;  // whether generate main()
  bool simp_call_;  // whether use simple call if possible

  bool jsop_only_;  // dump jsop only

  JSMIRContext(bool isplugin, const string &name, bool with_main, bool jsop_only, bool simp_call)
    : isplugin_(isplugin), wrapper_name_(name), with_main_(with_main), jsop_only_(jsop_only), simp_call_(simp_call) {}
};
bool js2mpldriver(const char *, MIRModule *, JSMIRContext &);
}  // namespace maple

#endif  // JS2MPL_INCLUDE_JS2MPL_H_
