/*
 * Copyright (C) [2021] Futurewei Technologies, Inc. All rights reserved.
 *
 * OpenArkCompiler is licensed under the Mulan Permissive Software License v2.
 * You can use this software according to the terms and conditions of the MulanPSL - 2.0.
 * You may obtain a copy of MulanPSL - 2.0 at:
 *
 *   https://opensource.org/licenses/MulanPSL-2.0
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR
 * FIT FOR A PARTICULAR PURPOSE.
 * See the MulanPSL - 2.0 for more details.
 */
#ifndef JSVALUE_H
#define JSVALUE_H
#define JSFUNCPROP_STRICT ((uint8_t)0x01)
#define JSFUNCPROP_NATIVE ((uint8_t)0x02)
#define JSFUNCPROP_USERFUNC ((uint8_t)0x04)
namespace maple {
enum __jstype {
  JSTYPE_NONE = 0,
  JSTYPE_NULL,
  JSTYPE_BOOLEAN,
  JSTYPE_STRING,
  JSTYPE_NUMBER,
  JSTYPE_OBJECT,
  JSTYPE_ENV,
  JSTYPE_UNKNOWN,
  JSTYPE_UNDEFINED,
  JSTYPE_DOUBLE,
  JSTYPE_NAN,
  JSTYPE_INFINITY,
  // the following tag generated for maple engine only
  JSTYPE_SPBASE,
  JSTYPE_FPBASE,
  JSTYPE_GPBASE,
  JSTYPE_FUNCTION,
};

// Bit is set for 16-bit code units
#define JSSTRING_UNICODE ((uint8_t)0x01)
// Bit is set for non-const code units
#define JSSTRING_GEN ((uint8_t)0x02)
// Bit is set for large strs whose length > 255
#define JSSTRING_LARGE ((uint8_t)0x04)

enum js_builtin_id {  // must in accordance with js_value.h:js_builtin_id in the runtime (maple_engine/include/jsvalue.h)
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
  JS_BUILTIN_EXPORTS,
  JS_BUILTIN_MODULE,
  JS_BUILTIN_MATH,
  JS_BUILTIN_JSON,
  JS_BUILTIN_ERROR_CONSTRUCTOR,
  JS_BUILTIN_ERROR_PROTOTYPE,
  JS_BUILTIN_EVALERROR_CONSTRUCTOR,
  JS_BUILTIN_EVALERROR_PROTOTYPE,
  JS_BUILTIN_RANGEERROR_CONSTRUCTOR,
  JS_BUILTIN_RANGEERROR_PROTOTYPE,
  JS_BUILTIN_REFERENCEERROR_OBJECT,
  JS_BUILTIN_REFERENCEERROR_PROTOTYPE,
  JS_BUILTIN_SYNTAXERROR_CONSTRUCTOR,
  JS_BUILTIN_SYNTAXERROR_PROTOTYPE,
  JS_BUILTIN_TYPEERROR_CONSTRUCTOR,
  JS_BUILTIN_TYPEERROR_PROTOTYPE,
  JS_BUILTIN_URIERROR_CONSTRUCTOR,
  JS_BUILTIN_URIERROR_PROTOTYPE,
  JS_BUILTIN_DATE,
  JS_BUILTIN_DATE_PROTOTYPE,
  JS_BUILTIN_ISNAN,
  JS_BUILTIN_REGEXP,
  JS_BUILTIN_REGEXPPROTOTYPE,
  JS_BUILTIN_NAN,
  JS_BUILTIN_INFINITY,
  JS_BUILTIN_UNDEFINED,
  JS_BUILTIN_PARSEINT,
  JS_BUILTIN_DECODEURI,
  JS_BUILTIN_DECODEURICOMPONENT,
  JS_BUILTIN_PARSEFLOAT,
  JS_BUILTIN_ISFINITE,
  JS_BUILTIN_ENCODEURI,
  JS_BUILTIN_ENCODEURICOMPONENT,
  JS_BUILTIN_EVAL,
  JS_BUILTIN_INTL,
  JS_BUILTIN_INTL_COLLATOR_CONSTRUCTOR,
  JS_BUILTIN_INTL_COLLATOR_PROTOTYPE,
  JS_BUILTIN_INTL_NUMBERFORMAT_CONSTRUCTOR,
  JS_BUILTIN_INTL_NUMBERFORMAT_PROTOTYPE,
  JS_BUILTIN_INTL_DATETIMEFORMAT_CONSTRUCTOR,
  JS_BUILTIN_INTL_DATETIMEFORMAT_PROTOTYPE,
  JS_BUILTIN_CONSOLE,
  JS_BUILTIN_ARRAYBUFFER,
  JS_BUILTIN_ARRAYBUFFER_PROTOTYPE,
  JS_BUILTIN_DATAVIEW,
  JS_BUILTIN_DATAVIEW_PROTOTYPE,
  JS_BUILTIN_COUNT,
};
}  // namespace maple
#endif
