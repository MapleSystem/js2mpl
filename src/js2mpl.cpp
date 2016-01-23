/// Copyright [year] <Copyright Owner>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "js/src/jsapi.h"
#include "js/src/jscntxt.h"
#include "../include/js2mpl.h"
#include "../include/compiler.h"

maplemp::MemPoolCtrler Mpc;
using namespace mapleir;
mapleir::MIRModule mapleir::themodule(Mpc);
int main(int argc, const char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: js2mpl javaScript");
    exit(1);
  }
  bool  isplugin = false;
  if (argc >=3) {
    for (int i = 2; i < argc; i++) {
      if (!strcmp(argv[i], "-plugin"))
        isplugin = true;
      else {
        char *end;
        long value = strtol(argv[i], &end,10);
        if (end == argv[i] ||*end !='\0' || errno == ERANGE) {
          fprintf(stderr, "usage: js2mpl javascript [debug id | -plugin]");
          exit(1);
        } else
          js2mplDebug = value;
      }
    }
  }
  const char *fn = argv[1];
  std::string file_name(fn);
  unsigned lastdot = file_name.find_last_of(".");
  std::string prefixfile_name = file_name.substr(0, lastdot);
  MirJsContext mirjscontx(isplugin, prefixfile_name);
  
  if (!mapleir::js2mpldriver(fn, &mapleir::themodule, mirjscontx)) {
    exit(1);
  }
  // use OPT_DUMPJSOPONLY to only dump JSOP code
  if (js2mplDebug == OPT_DUMPJSOPONLY)
    return 0;

  if (js2mplDebug > 0)
    mapleir::themodule.dump();

  // form output file name
  
  
  std::string out_file_name;
  if (lastdot == std::string::npos)
    out_file_name = file_name.append(".mpl");
  else out_file_name = file_name.substr(0, lastdot).append(".mpl");
  std::ofstream mplfile;
  mplfile.open(out_file_name.c_str(), std::ios::trunc);
  // save and then change cout's buffer to that of mplfile
  std::streambuf *backup = std::cout.rdbuf();
  std::cout.rdbuf(mplfile.rdbuf());
  mapleir::themodule.flavor_ = mapleir::FEproduced;
  mapleir::themodule.dump();  // write out generated Maple IR
  std::cout.rdbuf(backup);  // restore cout's buffer
  mplfile.close();
  return 0;
}

namespace mapleir {
// The class of the global object.
static JSClass global_class = {
    "global",
    JSCLASS_GLOBAL_FLAGS,
    JS_PropertyStub,
    JS_DeletePropertyStub,
    JS_PropertyStub,
    JS_StrictPropertyStub,
    JS_EnumerateStub,
    JS_ResolveStub,
    JS_ConvertStub
};

static inline void
SkipUTF8BOM(FILE *file) {
  int32_t ch1 = fgetc(file);
  int32_t ch2 = fgetc(file);
  int32_t ch3 = fgetc(file);
  // Skip the BOM
  if (ch1 == 0xEF && ch2 == 0xBB && ch3 == 0xBF)
    return;
  // No BOM - revert
  if (ch3 != EOF)
    ungetc(ch3, file);
  if (ch2 != EOF)
    ungetc(ch2, file);
  if (ch1 != EOF)
    ungetc(ch1, file);
}

static void myErrReproter(JSContext *cx, const char *message, JSErrorReport *report) {
  bool reportWarnings = false;
  FILE *errFile = fopen ("/tmp/error.log", "w");
  js::PrintError(cx, errFile, message, report, reportWarnings);
  fclose(errFile);

  std::string line;
  std::ifstream errStream("/tmp/error.log");
  if (errStream.is_open()) {
    while ( getline (errStream,line) ) {
      std::cout << line << '\n';
    }
    errStream.close();
  } else {
    std::cout << "Unable to open file";
  }
}

bool js2mpldriver(const char *fn, mapleir::MIRModule *module, MirJsContext &mirjscontx) {
  FILE *file = fopen(fn, "r");
  if (!file) {
    fprintf(stderr, "error input file.");
    exit(1);
  }
  SkipUTF8BOM(file);

  // Initialize JS related things.
  JS_Init();
  JSRuntime *rt = JS_NewRuntime(8L * 1024 * 1024, JS_USE_HELPER_THREADS);
  if (!rt)
    exit(1);
  JSContext *cx = JS_NewContext(rt, 8192);
  if (!cx)
    exit(1);
  JS_SetErrorReporter(cx, myErrReproter);

  // Make sure call the destructor of js objects(global/ac/script..)
  // before JS_DestroyContext.
  {
    JS::RootedObject global(cx, JS_NewGlobalObject(cx,
                                                   &global_class,
                                                   nullptr,
                                                   JS::FireOnNewGlobalHook));
    if (!global)
      exit(1);
    JSAutoCompartment ac(cx, global);
    JS_InitStandardClasses(cx, global);
    // Compile the script.
    JS::CompileOptions opts(cx);
    opts.setFileAndLine(fn, 1);

    // set more options
    opts.setVersion(JSVERSION_LATEST);
    // opts.setCompileAndGo(true);

    JS::RootedScript script(cx);
    script = JS::Compile(cx, global, opts, file);
    if (!script)
      exit(1);

    ///////////////////////////////////////////////
    // Pass To Set Up Scope Chain
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up Scope Chain <====\n");
    mapleir::Scope *scope = MP_NEW(module->mp_, mapleir::Scope(cx, script, module));
    scope->Init();
    scope->Build(script);

    if (js2mplDebug == OPT_DUMPJSOPONLY)
      goto finish;

    if (js2mplDebug>2) {
      std::cout << "========== After Scope Chain  ====" << std::endl;
      scope->DumpScopeChain();
      std::cout << "==================================" << std::endl;
    }

    ///////////////////////////////////////////////
    // Set Up JSMIRBuilder
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up JSMIRBuilder <===\n");
    mapleir::JSMIRBuilder *jsbuilder = MP_NEW(module->mp_, mapleir::JSMIRBuilder(module, mirjscontx));
    jsbuilder->Init();

    mapleir::OperandStack *opstack = MP_NEW(module->mp_, mapleir::OperandStack(50));

    ///////////////////////////////////////////////
    // Pass To Set Up Closure Environment
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up Closure Env <====\n");
    mapleir::JSClosure *closure = MP_NEW(module->mp_,
        mapleir::JSClosure(fn, cx, script, module, scope, jsbuilder, opstack));
    closure->Init();
    closure->Build(script);

    if (js2mplDebug>2) {
      std::cout << "==== After Closure Environment ===" << std::endl;
      scope->DumpScopeChain();
      std::cout << "==================================" << std::endl;
    }

    ///////////////////////////////////////////////
    // Pass To Build MapleIR.
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Build MapleIR <=========\n");
    mapleir::JSCompiler *compiler = MP_NEW(module->mp_,
        mapleir::JSCompiler(fn, cx, script, module, scope, jsbuilder, closure, opstack));

    compiler->Init();

    // first pass collect info, including function hirachy and closure setting
    compiler->CompileScript(script);

    compiler->Finish();

#if 0
    // Execute the script any times.
    JS::RootedValue rval(cx);
    int execute_time = 1;
    for (int i = 0; i < execute_time; i++) {
      bool ok = JS_ExecuteScript(cx, global, script, &rval);
      if (!ok)
        exit(1);
    }
#endif
  }

finish:
  // Finish.
  JS_DestroyContext(cx);
  JS_DestroyRuntime(rt);
  JS_ShutDown();
  return true;
}

}  // namespace mapleir
