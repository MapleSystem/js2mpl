#ifndef JSVALUE_H
#define JSVALUE_H

namespace mapleir {
#define JSVALTYPEDOUBLE            ((uint8_t)0x00)
#define JSVALTYPEINT32             ((uint8_t)0x01)
#define JSVALTYPEUNDEFINED         ((uint8_t)0x02)
#define JSVALTYPEBOOLEAN           ((uint8_t)0x03)
#define JSVALTYPEMAGIC             ((uint8_t)0x04)
#define JSVALTYPESTRING            ((uint8_t)0x05)
#define JSVALTYPENULL              ((uint8_t)0x06)
#define JSVALTYPEOBJECT            ((uint8_t)0x10)
#define JSVALTYPEFUNCTION          ((uint8_t)0x11)
#define JSVALTYPECLOSURE           ((uint8_t)0x12)
#define JSVALTYPEAYYAY             ((uint8_t)0x13)
#define JSVALTYPEELEMHOLE          ((uint8_t)0x14)
#define JSVALTYPEUNKNOWN           ((uint8_t)0x20)

#define JSVALTAGCLEAR              ((uint32_t)(0xFFFFFF80))
#define JSVALTAGINT32              ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEINT32))
#define JSVALTAGUNDEFINED          ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEUNDEFINED))
#define JSVALTAGSTRING             ((uint32_t)(JSVALTAGCLEAR | JSVALTYPESTRING))
#define JSVALTAGBOOLEAN            ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEBOOLEAN))
#define JSVALTAGMAGIC              ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEMAGIC))
#define JSVALTAGNULL               ((uint32_t)(JSVALTAGCLEAR | JSVALTYPENULL))
#define JSVALTAGOBJECT             ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEOBJECT))
#define JSVALTAGFUNCTION           ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEFUNCTION))
#define JSVALTAGCLOSURE            ((uint32_t)(JSVALTAGCLEAR | JSVALTYPECLOSURE))
#define JSVALTAGARRAY              ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEARRAY))
#define JSVALTAGELEMHOLE           ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEELEMHOLE))
}
#endif
