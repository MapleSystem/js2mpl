/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          call-1.js
   Section:            Function.prototype.call
   Description:


   Author:             christine@netscape.com
   Date:               12 november 1997
*/
var SECTION = "call-1";
var VERSION = "ECMA_2";
var TITLE   = "Function.prototype.call";
print( SECTION + " "+ TITLE);

function ConvertToString(obj) {
  return obj +"";
}

// CHECK#1
if (ConvertToString.call(Boolean, Boolean.prototype) !== "false"){
  $ERROR('#1: ConvertToString.call(Boolean, Boolean.prototype) !== false. Actual: ' + ConvertToString.call(Boolean, Boolean.prototype));
}

// CHECK#2
if (ConvertToString.call(Boolean, Boolean.prototype.valueOf()) !== "false"){
  $ERROR('#2: ConvertToString.call(Boolean, Boolean.prototype.valueOf()) !== false. Actual: ' + ConvertToString.call(Boolean, Boolean.prototype.valueOf()));
}
