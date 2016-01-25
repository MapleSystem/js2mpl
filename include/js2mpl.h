/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_JS2MPL_H_
#define JS2MPL_INCLUDE_JS2MPL_H_
#include <string.h>
using namespace std;
namespace mapleir {
  class MIRModule;
  struct MirJsContext {
    bool isplugin_;
    const string &wrapper_name_;
    MirJsContext(bool isplugin, const string &name):
      isplugin_(isplugin), wrapper_name_(name) {}
  };
  bool js2mpldriver(const char *, MIRModule *, MirJsContext &);
  
  
}

#endif  // JS2MPL_INCLUDE_JS2MPL_H_
