/// Copyright [year] <Copyright Owner>
#include <string>
#include "js2mpl-vm/include/jsfunction.h"

namespace mapleir {

void JSMIRFunction::AddToAliasList(stridx_t elem) {
  std::list<stridx_t>::iterator I;
  for (I = alias_vars.begin(); I != alias_vars.end(); I++) {
    if (*I == elem)
      return;
  }
  alias_vars.push_back(elem);
}

// starting from position after the fields in env_type
// parentenv js_arguments length env_argument
#define ALIAS_VAR_START_POSITION 5

int JSMIRFunction::GetAliasPosition(stridx_t elem) {
  std::list<stridx_t>::iterator I;
  int pos = ALIAS_VAR_START_POSITION;
  for (I = alias_vars.begin(); I != alias_vars.end(); I++) {
    if (*I == elem)
      return pos;
    pos++;
  }

  return -1;
}


}  // namespace mapleir
