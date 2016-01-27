/// Copyright [year] <Copyright Owner>
#ifndef JS2MPL_INCLUDE_JS2MPL_H_
#define JS2MPL_INCLUDE_JS2MPL_H_
#include <string.h>
using namespace std;
namespace mapleir {
  class MIRModule;
  struct JSMIRContext {
    bool isplugin_;
    const string &wrapper_name_;
    JSMIRContext(bool isplugin, const string &name):
      isplugin_(isplugin), wrapper_name_(name) {}
  };
  bool js2mpldriver(const char *, MIRModule *, JSMIRContext &);
  
  
}

#endif  // JS2MPL_INCLUDE_JS2MPL_H_
