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
#include <string>
#include "../include/jsfunction.h"

namespace maple {

void JSMIRFunction::AddToAliasList(GStrIdx elem) {
  std::list<GStrIdx>::iterator i;
  for (i = alias_vars.begin(); i != alias_vars.end(); i++) {
    if (*i == elem) {
      return;
    }
  }
  alias_vars.push_back(elem);
}

// starting from position after the fields in env_type
// parentenv js_arguments length env_argument
#define ALIAS_VAR_START_POSITION 5

int JSMIRFunction::GetAliasPosition(GStrIdx elem) {
  std::list<GStrIdx>::iterator i;
  int pos = ALIAS_VAR_START_POSITION;
  for (i = alias_vars.begin(); i != alias_vars.end(); i++) {
    if (*i == elem) {
      return pos;
    }
    pos++;
  }

  return -1;
}

}  // namespace maple
