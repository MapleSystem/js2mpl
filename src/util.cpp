/// Copyright [year] <Copyright Owner>
#include "js/src/jsapi.h"
#include "js/src/jsscript.h"
#include "js/src/jsopcode.h"
#include "js/src/jsfun.h"
#include "js/src/jsatom.h"
#include "js/src/jscntxt.h"
#include "js/src/jsatominlines.h"
#include "maple_ir/include/mir_nodes.h"
#include "../include/jsfunction.h"
#include "../include/util.h"

int js2mplDebug = 0;
int js2mplDebugIndent = 0;

namespace maple {

void Util::AdjIndent(int n) {
  js2mplDebugIndent += n;
}

void Util::SetIndent(int n) {
  js2mplDebugIndent = n;
}

const char *Util::getOpcodeName[228] = {
#include "opcname.def"
};

static bool ValidInName(char c) {
  if (c >= 'a' && c <= 'z') {
    return true;
  }
  if (c >= 'A' && c <= 'Z') {
    return true;
  }
  if (c >= '0' && c <= '9') {
    return true;
  }
  if (c == '_' || c == '$') {
    return true;
  }
  return false;
}

char *Util::GetString(JSAtom *atom, MemPool *mp, JSContext *ctx) {
  if (!atom) {
    return NULL;
  }

  js::ExclusiveContext *ect = (js::ExclusiveContext *)ctx;
  int len = atom->length();
  const jschar *js = atom->getCharsZ(ect);
  char *name = static_cast<char *>(mp->Malloc(len + 1));
  char c;

  bool isGood = true;
  for (int i = 0; i < len; i++) {
    c = js[i];
    if (ValidInName(c)) {
      name[i] = c;
    } else {
      isGood = false;
      DEBUGPRINT2(c);
    }
  }
  name[len] = '\0';

  DEBUGPRINT2(name);

  if (!isGood) {
    return NULL;
  }

  return name;
}

char *Util::GetSequentialName0WithLineNo(const char *prefix, uint32_t num, MemPool *mp, unsigned lineNo) {
  std::stringstream ss;

  if (lineNo) { // anonym lambda
    ss << num;
    ss << "_at_line_" << lineNo;
  } else {      // named lambda
    ss << "_" << num << "_";
  }
  MapleString name(prefix + ss.str(), mp);
  DEBUGPRINT2(name.c_str());
  return name.c_str();
}

char *Util::GetSequentialName0(const char *prefix, uint32_t num, MemPool *mp) {
  std::stringstream ss;
  ss << num;

  MapleString name(prefix + ss.str(), mp);
  DEBUGPRINT2(name.c_str());
  return name.c_str();
}

char *Util::GetSequentialName(const char *prefix, uint32_t &num, MemPool *mp) {
  char *name = GetSequentialName0(prefix, num, mp);
  num++;
  return name;
}

char *Util::GetNameWithPrefix(const char *origName, const char *prefix, MemPool *mp) {
  std::stringstream ss;
  ss << prefix;
  MapleString name(ss.str() + origName, mp);
  return name.c_str();
}

char *Util::GetNameWithSuffix(const char *origName, const char *suffix, MemPool *mp) {
  std::stringstream ss;
  ss << suffix;
  MapleString name(origName + ss.str(), mp);
  return name.c_str();
}

char *Util::GetNameWithScopeSuffix(const char *origName, uint32_t scope, MemPool *mp) {
  std::stringstream ss;
  ss << "__" << std::hex << scope;
  MapleString name(origName + ss.str(), mp);
  return name.c_str();
}

void Util::GetSrcFile(const char *fileName, std::vector<char *>&srcLines) {
  FILE *f = fopen(fileName, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *buf = (char *)malloc(fsize + 1);
  fread(buf, fsize, 1, f);
  fclose(f);
  buf[fsize] = 0;

  srcLines.push_back(buf);
  for (int i=0; i<fsize; ++i) {
    if (buf[i] == 0xa) {
      buf[i] = 0;
      if (i+1 < fsize) {
        srcLines.push_back(&buf[i+1]);
      }
    }
  }
}


}  // namespace maple
