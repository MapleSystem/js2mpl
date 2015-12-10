/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.1.js
   ECMA Section:       15.7.1 The Number Constructor Called as a Function
   15.7.1.1
   15.7.1.2

   Description:        When Number is called as a function rather than as a
   constructor, it performs a type conversion.
   15.7.1.1    Return a number value (not a Number object)
   computed by ToNumber( value )
   15.7.1.2    Number() returns 0.

   need to add more test cases.  see the testcases for
   TypeConversion ToNumber.

   Author:             christine@netscape.com
   Date:               29 september 1997
*/

var SECTION = "15.7.1";
var VERSION = "ECMA_1";
var TITLE   = "The Number Constructor Called as a Function";

print( SECTION + " "+ TITLE);

// CHECK#1
if (Number() !== 0){
  $ERROR('#1: Number() !== 0. Actual: ' + Number());
}

// CHECK#2
var res = Number(void 0);
if (String(res) !== 'NaN'){
  $ERROR('#2: Number(void 0) !== NaN. Actual: ' + Number(void 0));
}

// CHECK#3
if (Number(null) !== 0){
  $ERROR('#3: Number(null) !== 0. Actual: ' + Number(null));
}

// CHECK#4
if (Number(new Number()) !== 0){
  $ERROR('#4: Number(new Number()) !== 0. Actual: ' + Number(new Number()));
}

// CHECK#5
if (Number(0) !== 0){
  $ERROR('#5: Number(0) !== 0. Actual: ' + Number(0));
}

// CHECK#6
if (Number(1) !== 1){
  $ERROR('#6: Number(1) !== 1. Actual: ' + Number(1));
}

// CHECK#7
if (Number(-1) !== -1){
  $ERROR('#7: Number(-1) !== -1. Actual: ' + Number(-1));
}

// CHECK#8
var res = Number( Number.NaN);
if (String(res) !== 'NaN'){
  $ERROR('#8: Number( Number.NaN ) !== Number.NaN. Actual: ' + Number( Number.NaN ));
}

// CHECK#9
var res = Number('string');
if (String(res) !== 'NaN'){
  $ERROR('#9: Number("string") !== Number.NaN. Actual: ' + Number('string'));
}

// CHECK#10
if (Number(new String()) !== 0){
  $ERROR('#10: Number(new String()) !== 0. Actual: ' + Number(new String()));
}

// CHECK#11
if (Number('') !== 0){
  $ERROR('#11: Number("") !== 0. Actual: ' + Number(''));
}

// CHECK#12
if (Number(Infinity) !== Number.POSITIVE_INFINITY){
  $ERROR('#12: Number(Infinity) !== Number.POSITIVE_INFINITY. Actual: ' + Number(Infinity));
}
/*
// CHECK#13
if (Number(new MyObject(100)) !== 100){
  $ERROR('#13: Number(new MyObject(100)) !== 100. Actual: ' + Number(new MyObject(100)));
}

function MyObject( value ) {
  this.value = value;
  this.valueOf = new Function( "return this.value" );
}
*/