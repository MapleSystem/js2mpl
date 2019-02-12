/// Copyright [year] <Copyright Owner>
#include "js/src/jsapi.h"
#include "js/src/jsscript.h"
#include "js/src/jsopcode.h"
#include "js/src/jsfun.h"
#include "js/src/jsatom.h"
#include "js/src/jscntxt.h"
#include "js/src/jsatominlines.h"
#include "mapleir/include/mirnodes.h"
#include "../include/jsfunction.h"
#include "../include/util.h"

int js2mplDebug = 0;
int js2mplDebugIndent = 0;

namespace mapleir {

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
        if (c >= 'a' && c <= 'z')
            return true;
        if (c >= 'A' && c <= 'Z')
            return true;
        if (c >= '0' && c <= '9')
            return true;
        if (c == '_' || c == '$')
            return true;
        return false;
    }

    char *Util::GetString(JSAtom *atom, MemPool *mp, JSContext *ctx) {
        if (!atom)
            return NULL;

        js::ExclusiveContext *ect = (js::ExclusiveContext *) ctx;
        int len = atom->length();
        const jschar *js = atom->getCharsZ(ect);
        char *name = static_cast<char *>(mp->Malloc(len + 1));
        char c;

        bool isGood = true;
        for (int i = 0; i < len; i++) {
            c = js[i];
            if (ValidInName(c))
                name[i] = c;
            else {
                isGood = false;
                DEBUGPRINT2(c);
            }
        }
        name[len] = '\0';

        DEBUGPRINT2(name);

        if (!isGood)
            return NULL;

        return name;
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

    char *Util::GetNameWithPrefix(const char *orig_name, const char *prefix, MemPool *mp) {
        std::stringstream ss;
        ss << prefix;
        MapleString name(ss.str() + orig_name, mp);
        return name.c_str();
    }

    char *Util::GetNameWithSuffix(const char *orig_name, const char *suffix, MemPool *mp) {
        std::stringstream ss;
        ss << suffix;
        MapleString name(orig_name + ss.str(), mp);
        return name.c_str();
    }

} // namespace mapleir
