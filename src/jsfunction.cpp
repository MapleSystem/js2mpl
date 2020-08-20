/// Copyright [year] <Copyright Owner>
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
