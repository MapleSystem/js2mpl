/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.2.2-1.js
   ECMA Section:       15.4.2.2 new Array(len)

   Description:        This description only applies of the constructor is
   given two or more arguments.

   The [[Prototype]] property of the newly constructed
   object is set to the original Array prototype object,
   the one that is the initial value of Array.prototype(0)
   (15.4.3.1).

   The [[Class]] property of the newly constructed object
   is set to "Array".

   If the argument len is a number, then the length
   property  of the newly constructed object is set to
   ToUint32(len).

   If the argument len is not a number, then the length
   property of the newly constructed object is set to 1
   and the 0 property of the newly constructed object is
   set to len.

   This file tests cases where len is a number.

   The cases in this test need to be updated since the
   ToUint32_t description has changed.

   Author:             christine@netscape.com
   Date:               7 october 1997
*/
var SECTION = "15.4.2.2-1";
var VERSION = "ECMA_1";
var TITLE   = "The Array Constructor:  new Array( len )";

print (SECTION + " "+ TITLE);

// CHECK#1
if ((new Array(0)).toString() !== ""){
  $ERROR('#1: (new Array(0)).toString() === "". Actual: ' + (new Array(0)).toString());
}

// CHECK#2
if (typeof new Array(0) !== "object"){
  $ERROR('#2: typeof new Array(0) === "object". Actual: ' + typeof new Array(0));
}

// CHECK#3
if ((new Array(0)).length !== 0){
  $ERROR('#3: (new Array(0)).length === 0. Actual: ' + (new Array(0)).length);
}

// CHECK#4
if ((new Array(0)).toString !== Array.prototype.toString){
  $ERROR('#4: (new Array(0)).toString === Array.prototype.toString. Actual: ' + (new Array(0)).toString);
}

// CHECK#5
if ((new Array(1)).toString() !== ""){
  $ERROR('#5: (new Array(1)).toString() === "". Actual: ' + (new Array(1)).toString());
}

// CHECK#6
if ((new Array(1)).length !== 1){
  $ERROR('#6: (new Array(1)).length === 1. Actual: ' + (new Array(1)).length);
}

// CHECK#7
if ((new Array(1)).toString !== Array.prototype.toString){
  $ERROR('#7: (new Array(1)).toString === Array.prototype.toString. Actual: ' + (new Array(1)).toString);
}
/*
// CHECK#8
if ((new Array(-0)).length !== 0){
  $ERROR('#8: (new Array(-0)).length === 0. Actual: ' + (new Array(-0)).length);
}
*/
// CHECK#9
if ((new Array(0)).length !== 0){
  $ERROR('#9: (new Array(0)).length === 0. Actual: ' + (new Array(0)).length);
}

// CHECK#10
if ((new Array(10)).length !== 10){
  $ERROR('#10: (new Array(10)).length === 10. Actual: ' + (new Array(10)).length);
}

// CHECK#11
if ((new Array('1')).length !== 1){
  $ERROR('#11: (new Array("1")).length === 1. Actual: ' + (new Array('1')).length);
}

// CHECK#12
if ((new Array(100)).length !== 100){
  $ERROR('#12: (new Array(1000)).length === 1000. Actual: ' + (new Array(100)).length);
}

// CHECK#13
if ((new Array('100')).length !== 1){
  $ERROR('#13: (new Array("100")).length === 1. Actual: ' + (new Array('1000')).length);
}

// CHECK#14
//if ((new Array(4294967295)).length !== ToUint32(4294967295)){
//  $ERROR('#14: (new Array(4294967295)).length === ToUint32(4294967295). Actual: ' + (new Array(4294967295)).length);
//}

// CHECK#17
//if ((new Array(1073741824)).length !== ToUint32(1073741824)){
//  $ERROR('#17: (new Array(1073741824)).length === ToUint32(1073741824). Actual: ' + (new Array(1073741824)).length);
//}
/*
function ToUint32( n ) {
  n = Number( n );
  var sign = ( n < 0 ) ? -1 : 1;

  if ( Math.abs( n ) == 0 || Math.abs( n ) == Number.POSITIVE_INFINITY) {
    return 0;
  }
  n = sign * Math.floor( Math.abs(n) )

    n = n % Math.pow(2,32);

  if ( n < 0 ){
    n += Math.pow(2,32);
  }

  return ( n );
}
*/
