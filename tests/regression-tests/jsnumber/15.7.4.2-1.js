/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.4.2.js
   ECMA Section:       15.7.4.2.1 Number.prototype.toString()
   Description:
   If the radix is the number 10 or not supplied, then this number value is
   given as an argument to the ToString operator; the resulting string value
   is returned.

   If the radix is supplied and is an integer from 2 to 36, but not 10, the
   result is a string, the choice of which is implementation dependent.

   The toString function is not generic; it generates a runtime error if its
   this value is not a Number object. Therefore it cannot be transferred to
   other kinds of objects for use as a method.

   Author:             christine@netscape.com
   Date:               16 september 1997
*/
var SECTION = "15.7.4.2-1";
var VERSION = "ECMA_1";

print( SECTION + " "+ " Number.prototype.toString()");

// CHECK#1
if (Number.prototype.toString() !== "0"){
  $ERROR('#1: Number.prototype.toString() !== "0". Actual: ' + Number.prototype.toString());
}

// CHECK#2
if (typeof(Number.prototype.toString()) !== "string"){
  $ERROR('#2: typeof(Number.prototype.toString()) !== "string". Actual: ' + typeof(Number.prototype.toString()));
}

// CHECK#3
s = Number.prototype.toString;
o = new Number();
o.toString = s;
if (o.toString() !== "0"){
  $ERROR('#3: s = Number.prototype.toString; o = new Number(); o.toString = s; o.toString() !== "0". Actual: ' + o.toString());
}

// CHECK#4
s = Number.prototype.toString;
o = new Number(1);
o.toString = s;
if (o.toString() !== "1"){
  $ERROR('#4: s = Number.prototype.toString; o = new Number(1); o.toString = s; o.toString() !== "1". Actual: ' + o.toString());
}

// CHECK#5
s = Number.prototype.toString;
o = new Number(-1);
o.toString = s;
if (o.toString() !== "-1"){
  $ERROR('#5: s = Number.prototype.toString; o = new Number(-1); o.toString = s; o.toString() !== "-1". Actual: ' + o.toString());
}

// CHECK#6
var MYNUM = new Number(255);
if (MYNUM.toString(10) !== "255"){
  $ERROR('#6: MYNUM.toString(10) !== "255". Actual: ' + MYNUM.toString(10));
}

// CHECK#7
var MYNUM = new Number(Number.NaN);
if (MYNUM.toString(10) !== "NaN"){
  $ERROR('#7: MYNUM.toString(10) !== "NaN". Actual: ' + MYNUM.toString(10));
}

// CHECK#8
var MYNUM = new Number(Infinity);
if (MYNUM.toString(10) !== "Infinity"){
  $ERROR('#8: MYNUM.toString(10) !== "NaN". Actual: ' + MYNUM.toString(10));
}

// CHECK#9
var MYNUM = new Number(-Infinity);
if (MYNUM.toString(10) !== "-Infinity"){
  $ERROR('#9: MYNUM.toString(10) !== "NaN". Actual: ' + MYNUM.toString(10));
}