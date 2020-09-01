/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.4.3-1.js
   ECMA Section:       15.7.4.3.1 Number.prototype.valueOf()
   Description:
   Returns this number value.

   The valueOf function is not generic; it generates a runtime error if its
   this value is not a Number object. Therefore it cannot be transferred to
   other kinds of objects for use as a method.

   Author:             christine@netscape.com
   Date:               16 september 1997
*/
var SECTION = "15.7.4.3-1";
var VERSION = "ECMA_1";

print( SECTION + " Number.prototype.valueOf()");

//  the following two line causes navigator to crash -- cmb 9/16/97
// CHECK#1
if (Number.prototype.valueOf() !== 0){
  $ERROR('#1: Number.prototype.valueOf() !== 0. Actual: ' + Number.prototype.valueOf());
}

// CHECK#2
if ((new Number(1)).valueOf() !== 1){
  $ERROR('#2: (new Number(1)).valueOf() !== 1. Actual: ' + (new Number(1)).valueOf());
}

// CHECK#3
if ((new Number(-1)).valueOf() !== -1){
  $ERROR('#3: (new Number(-1)).valueOf() !== -1. Actual: ' + (new Number(-1)).valueOf());
}

// CHECK#4
if ((new Number(0)).valueOf() !== 0){
  $ERROR('#4: (new Number(0)).valueOf() !== 0. Actual: ' + (new Number(0)).valueOf());
}

// CHECK#5
if ((new Number(Number.POSITIVE_INFINITY)).valueOf() !== Number.POSITIVE_INFINITY){
  $ERROR('#5: (new Number(Number.POSITIVE_INFINITY)).valueOf() !== Number.POSITIVE_INFINITY. Actual: ' + (new Number(Number.POSITIVE_INFINITY)).valueOf());
}

// CHECK#6
if (String((new Number(Number.NaN)).valueOf()) !== String(Number.NaN)){
  $ERROR('#6: (new Number(Number.NaN)).valueOf() !== Number.NaN. Actual: ' + (new Number(Number.NaN)).valueOf());
}

// CHECK#7
if ((new Number()).valueOf() !== 0){
  $ERROR('#7: (new Number()).valueOf() !== 0. Actual: ' + (new Number()).valueOf());
}