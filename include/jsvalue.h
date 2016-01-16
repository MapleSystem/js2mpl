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
#define JSVALTYPEELEMHOLE          ((uint8_t)0x09)
#define JSVALTYPEOBJECT            ((uint8_t)0x10)
#define JSVALTYPEUNKNOWN           ((uint8_t)0x20)

#define JSVALTAGCLEAR              ((uint32_t)(0xFFFFFF80))
#define JSVALTAGINT32              ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEINT32))
#define JSVALTAGUNDEFINED          ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEUNDEFINED))
#define JSVALTAGSTRING             ((uint32_t)(JSVALTAGCLEAR | JSVALTYPESTRING))
#define JSVALTAGBOOLEAN            ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEBOOLEAN))
#define JSVALTAGMAGIC              ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEMAGIC))
#define JSVALTAGNULL               ((uint32_t)(JSVALTAGCLEAR | JSVALTYPENULL))
#define JSVALTAGOBJECT             ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEOBJECT))
#define JSVALTAGELEMHOLE           ((uint32_t)(JSVALTAGCLEAR | JSVALTYPEELEMHOLE))

typedef enum {
  // Include sequences of 8-bit code units.
  JSSTRING_ASCII = 0,
  // Include sequences of 16-bit code units.
  JSSTRING_UNICODE,
  // Inclue sequences of 8-bit code units of generated.
  JSSTRING_ASCII_GEN,
  // Inclue sequences of 16-bit code units of generated.
  JSSTRING_UNICODE_GEN,
}__jsstring_class;

typedef enum {
  JS_BUILTIN_GLOBAL = 0,
  JS_BUILTIN_OBJECT,
  JS_BUILTIN_OBJECT_PROTOTYPE,
  JS_BUILTIN_FUNCTION,
  JS_BUILTIN_FUNCTION_PROTOTYPE,
  JS_BUILTIN_ARRAY,
  JS_BUILTIN_ARRAY_PROTOTYPE,
  JS_BUILTIN_STRING,
  JS_BUILTIN_STRING_PROTOTYPE,
  JS_BUILTIN_BOOLEAN,
  JS_BUILTIN_BOOLEAN_PROTOTYPE,
  JS_BUILTIN_NUMBER,
  JS_BUILTIN_NUMBER_PROTOTYPE,
  JS_BUILTIN_COUNT
} js_builtin_id;
}
#endif
