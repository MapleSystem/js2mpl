/// Copyright [year] <Copyright Owner>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include "js/src/jsapi.h"
#include "js/src/jscntxt.h"
#include "../include/js2mpl.h"
#include "../include/compiler.h"

using namespace mapleir;
using namespace std;

GlobalTables mapleir::globaltable;

// extract base name and plugin name from input js file name with path
static void ProcessSrcInfo(string infile, string &basename,  string &pluginname) {
  unsigned lastdot = infile.find_last_of(".");
  unsigned lastslash = infile.find_last_of("/");
  string dir = infile.substr(0, lastslash);
  basename = infile.substr(lastslash+1, lastdot-lastslash-1);

  // replace - . in file name to _
  replace(basename.begin(), basename.end(), '-', '_');
  replace(basename.begin(), basename.end(), '.', '_');

  // for add.js, the plugin file is add.jsp
  pluginname = PLUGINPREFIX + basename;

  return;
}

// create a new src file with a plugin wrapper
static void GenJSFile(string infile, bool with_main, string &outfile) {
  string basename;
  string pluginname;
  ProcessSrcInfo(infile, basename, pluginname); 

  unsigned lastdot = infile.find_last_of(".");
  unsigned lastslash = infile.find_last_of("/");
  string dir = infile.substr(0, lastslash);

  outfile = dir + "/" +  PLUGINPREFIX + basename + ".js";

  ifstream src (infile.c_str());
  ofstream des (outfile.c_str());
  if (src.is_open() && des.is_open()) {
    // add wrapper around the whole file
    des << "function " << pluginname << "() {\n";

    string line;
    while (getline(src, line)) {
      des << line << '\n';
    }
    des << "}\n";

    // for testing purpose
    if (with_main)
      des << pluginname.c_str() << "();\n";

    src.close();
    des.close();
  } else {
    cout << "unable to open file";
    exit(1);
  }
  return;
}

static void help() {
  fprintf(stderr, "=============================================\n");
  fprintf(stderr, " usage: js2mpl javascript [ options ]\n");
  fprintf(stderr, " options:\n");
  fprintf(stderr, "   -d=n    : debug print level n=1,2,3,...\n");
  fprintf(stderr, "   -plugin : generate mpl file as plugin.\n");
  fprintf(stderr, "   -main   : generate mpl file with main.\n");
  fprintf(stderr, "   -nomain : generate mpl file without main.\n");
  fprintf(stderr, "   -jsop   : dump JSOP only.\n");
  fprintf(stderr, "   -help   : display this usage info.\n");
  fprintf(stderr, "=============================================\n");
}

int main(int argc, const char *argv[]) {

  if (!strcmp(argv[1], "-help")) {
    help();
    exit(1);
  }
  if (argc < 2) {
    fprintf(stderr, "usage: js2mpl javaScript [-d=n | -plugin]\n");
    exit(1);
  }
  bool isplugin = false;
  bool with_main = true;
  bool jsop_only = false;
  bool simp_call = true;

  if (argc >=3) {
    for (int i = 2; i < argc; i++) {
      if (!strncmp(argv[i], "-d=", 3)) {
        int value = atoi(argv[i]+3);
        js2mplDebug = value;
      } else if (!strcmp(argv[i], "-plugin")) {
        isplugin = true;
      } else if (!strcmp(argv[i], "-main")) {
        with_main = true;
      } else if (!strcmp(argv[i], "-nomain")) {
        with_main = false;
      } else if (!strcmp(argv[i], "-jsop")) {
        jsop_only = true;
      } else if (!strcmp(argv[i], "-nosimpcall")) {
        simp_call = false;
      } else if (!strcmp(argv[i], "-help")) {
        help();
      } else {
        char *end;
        long value = strtol(argv[i], &end,10);
        if (end == argv[i] ||*end !='\0' || errno == ERANGE) {
          help();
          exit(1);
        } else
          js2mplDebug = value;
      }
    }
  }

  const char *fn = argv[1];
  string file_name(fn);

  string name;
  string pluginname;
  ProcessSrcInfo(file_name, name, pluginname);

  if (isplugin) {
    name = pluginname;
  }

  JSMIRContext jsmirctx(isplugin, name, with_main, jsop_only, simp_call);

  MIRModule themodule(fn);
  themodule.srclang_ = mapleir::SrcLangJS;
  if (!mapleir::js2mpldriver(fn, &themodule, jsmirctx)) {
    exit(1);
  }

  // set entryfuncname_ in MIRModule
  if (! isplugin)
    themodule.entryfuncname_ = "main";
  else {  // entryfunc_ is the last function generated
    themodule.entryfuncname_ = globaltable.GetSymbolFromStidx(themodule._function_list.back()->stidx.Idx())->GetName();
  }
  // set numfuncs_ in MIRModule
  themodule.num_funcs = themodule._function_list.size();

  if (js2mplDebug > 0)
    themodule.Dump();

  themodule.flavor_ = mapleir::FEproduced;
  themodule.OutputAsciiMpl("");
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

  string line;
  ifstream errStream("/tmp/error.log");
  if (errStream.is_open()) {
    while ( getline (errStream,line) ) {
      cout << line << '\n';
    }
    errStream.close();
  } else {
    cout << "Unable to open file";
  }
}

bool js2mpldriver(const char *fn, mapleir::MIRModule *module, JSMIRContext &jsmirctx) {
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
    // Set Up JSMIRBuilder
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up JSMIRBuilder <===\n");
    mapleir::JSMIRBuilder jsbuilder(module, jsmirctx);
    jsbuilder.Init();

    mapleir::OperandStack *opstack = module->mp_->New<mapleir::OperandStack>(50);

    ///////////////////////////////////////////////
    // Pass To Set Up Scope Chain
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up Scope Chain <====\n");
    mapleir::Scope scope(cx, script, module, &jsbuilder);
    scope.Init();
    scope.Build(script);

    if (jsbuilder.JSOPOnly())
      goto finish;

    if (js2mplDebug>2) {
      cout << "========== After Scope Chain  ====" << endl;
      scope.DumpScopeChain();
      cout << "==================================" << endl;
    }

    ///////////////////////////////////////////////
    // Pass To Set Up Exception Handling
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up Exception Handling <====\n");
    mapleir::EH eh(cx, script, module, &jsbuilder, &scope);
    eh.Build(script);

    ///////////////////////////////////////////////
    // Pass To Set Up Closure Environment
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Set Up Closure Env <====\n");
    mapleir::JSClosure closure(fn, cx, script, module, &scope, &jsbuilder, opstack);
    closure.Init();
    closure.Build(script);

    if (js2mplDebug>2) {
      cout << "==== After Closure Environment ===" << endl;
      scope.DumpScopeChain();
      cout << "==================================" << endl;
    }

    ///////////////////////////////////////////////
    // Pass To Build MapleIR.
    ///////////////////////////////////////////////
    DEBUGPRINTs("\n\n =====> Pass To Build MapleIR <=========\n");
    mapleir::JSCompiler compiler(fn, cx, script, module, &jsbuilder, &scope, &eh, &closure, opstack);

    compiler.Init();

    // first pass collect info, including function hirachy and closure setting
    compiler.CompileScript(script);

    compiler.Finish();

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
