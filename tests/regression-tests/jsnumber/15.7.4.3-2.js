/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.4.3-2.js
   ECMA Section:       15.7.4.3.1 Number.prototype.valueOf()
   Description:
   Returns this number value.

   The valueOf function is not generic; it generates a runtime error if its
   this value is not a Number object. Therefore it cannot be transferred to
   other kinds of objects for use as a method.

   Author:             christine@netscape.com
   Date:               16 september 1997
*/
var SECTION = "15.7.4.3-2";
var VERSION = "ECMA_1";

print( SECTION + " Number.prototype.valueOf()");

// CHECK#1
v = Number.prototype.valueOf;
num = 3;
//num.valueOf = v;//: ./include/jsvalueinline.h:105: __jsval_to_object: Assertion `__is_object(data)' failed.
if (num.valueOf()!== 3){
  $ERROR('#1: v = Number.prototype.valueOf; num = 3; num.valueOf = v; num.valueOf() !== 3. Actual: ' + num.valueOf());
}