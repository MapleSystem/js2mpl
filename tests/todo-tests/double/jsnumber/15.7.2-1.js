/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.2.js
   ECMA Section:       15.7.2 The Number Constructor
   15.7.2.1
   15.7.2.2

   Description:        15.7.2 When Number is called as part of a new
   expression, it is a constructor:  it initializes
   the newly created object.

   15.7.2.1 The [[Prototype]] property of the newly
   constructed object is set to othe original Number
   prototype object, the one that is the initial value
   of Number.prototype(0).  The [[Class]] property is
   set to "Number".  The [[Value]] property of the
   newly constructed object is set to ToNumber(value)

   15.7.2.2 new Number().  same as in 15.7.2.1, except
   the [[Value]] property is set to +0.

   need to add more test cases.  see the testcases for
   TypeConversion ToNumber.

   Author:             christine@netscape.com
   Date:               29 september 1997
*/

var SECTION = "15.7.2";
var VERSION = "ECMA_1";
var TITLE   = "The Number Constructor";

print( SECTION + " "+ TITLE);

//  To verify that the object's prototype is the Number.prototype, check to see if the object's
//  constructor property is the same as Number.prototype.constructor.
// CHECK#1
if ((new Number()).constructor !== Number.prototype.constructor){
  $ERROR('#1: (new Number()).constructor !== Number.prototype.constructor. Actual: ' + (new Number()).constructor);
}
if (typeof (new Number()) !== "object"){
  $ERROR('#1: typeof (new Number()) !== "object". Actual: ' + typeof (new Number()));
}
if ((new Number()).valueOf() !== 0){
  $ERROR('#1: (new Number()).valueOf() !== 0. Actual: ' + (new Number()).valueOf());
}


// CHECK#2
if ((new Number(0)).constructor !== Number.prototype.constructor){
  $ERROR('#2: (new Number(0)).constructor !== Number.prototype.constructor. Actual: ' + (new Number(0)).constructor);
}
if (typeof (new Number(0)) !== "object"){
  $ERROR('#2: typeof (new Number(0)) !== "object". Actual: ' + typeof (new Number(0)));
}
if ((new Number(0)).valueOf() !== 0){
  $ERROR('#2: (new Number(0)).valueOf() !== 0. Actual: ' + (new Number(0)).valueOf());
}


// CHECK#3
if ((new Number(1)).constructor !== Number.prototype.constructor){
  $ERROR('#3: (new Number(1)).constructor !== Number.prototype.constructor. Actual: ' + (new Number(1)).constructor);
}
if (typeof (new Number(1)) !== "object"){
  $ERROR('#3: typeof (new Number(1)) !== "object". Actual: ' + typeof (new Number(1)));
}
if ((new Number(1)).valueOf() !== 1){
  $ERROR('#3: (new Number(1)).valueOf() !== 1. Actual: ' + (new Number(1)).valueOf());
}


// CHECK#4
if ((new Number(-1)).constructor !== Number.prototype.constructor){
  $ERROR('#4: (new Number(-1)).constructor !== Number.prototype.constructor. Actual: ' + (new Number(-1)).constructor);
}
if (typeof (new Number(-1)) !== "object"){
  $ERROR('#4: typeof (new Number(-1)) !== "object". Actual: ' + typeof (new Number(-1)));
}
if ((new Number(-1)).valueOf() !== -1){
  $ERROR('#4: (new Number(-1)).valueOf() !== -1. Actual: ' + (new Number(-1)).valueOf());
}


// CHECK#5
if ((new Number(Number.NaN)).constructor !== Number.prototype.constructor){
  $ERROR('#5: (new Number(Number.NaN)).constructor !== Number.prototype.constructor. Actual: ' + (new Number(Number.NaN)).constructor);
}
if (typeof (new Number(Number.NaN)) !== "object"){
  $ERROR('#5: typeof (new Number(Number.NaN)) !== "object". Actual: ' + typeof (new Number(Number.NaN)));
}
if (String((new Number(Number.NaN)).valueOf()) !== 'NaN'){
  $ERROR('#5: (new Number(Number.NaN)).valueOf() !== Number.NaN. Actual: ' + (new Number(Number.NaN)).valueOf());
}