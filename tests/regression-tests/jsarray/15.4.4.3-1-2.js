/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:    15.4.4.3-1.js
   ECMA Section: 15.4.4.3-1 Array.prototype.join()
   Description:  The elements of this object are converted to strings and
   these strings are then concatenated, separated by comma
   characters. The result is the same as if the built-in join
   method were invoiked for this object with no argument.
   Author:       christine@netscape.com, pschwartau@netscape.com
   Date:         07 October 1997
   Modified:     14 July 2002
   Reason:       See http://bugzilla.mozilla.org/show_bug.cgi?id=155285
   ECMA-262 Ed.3  Section 15.4.4.5 Array.prototype.join()
   Step 3: If |separator| is |undefined|, let |separator|
   be the single-character string ","
   *
   */

var SECTION = "15.4.4.3-1";
var VERSION = "ECMA_1";
print (SECTION + " "+ " Array.prototype.join()");

var ARR_PROTOTYPE = Array.prototype;

// CHECK#1
if (Array.prototype.join.length !== 1){
  $ERROR('#1: Array.prototype.join.length === 1. Actual: ' + Array.prototype.join.length);
}

// CHECK#2
//if (delete Array.prototype.join.length !== false){
//  $ERROR('#2: delete Array.prototype.join.length === false. Actual: ' + delete Array.prototype.join.length);
//}

// CHECK#3
delete Array.prototype.join.length;
if (Array.prototype.join.length !== 1){
  $ERROR('#3: delete Array.prototype.join.length; Array.prototype.join.length === 1. Actual: ' + Array.prototype.join.length);
}

// case where array length is 0
// CHECK#4
var TEST_ARRAY = new Array(); 
if (TEST_ARRAY.join() !== ""){
  $ERROR('#4: var TEST_ARRAY = new Array(); TEST_ARRAY.join() === "". Actual: ' + TEST_ARRAY.join());
}

// array length is 0, but spearator is specified
// CHECK#5
var TEST_ARRAY = new Array()
if (TEST_ARRAY.join(' ') !== ""){
  $ERROR('#5: var TEST_ARRAY = new Array(); TEST_ARRAY.join(" ") === "". Actual: ' + TEST_ARRAY.join(' '));
}

// length is greater than 0, separator is supplied
// CHECK#6
var TEST_ARRAY = new Array(null, void 0, true, false, 123, new Object(), new Boolean(true) );
if (TEST_ARRAY.join('&') !== "&&true&false&123&[object Object]&true"){
  $ERROR('#6: TEST_ARRAY.join("&") === "&&true&false&123&[object Object]&true" Actual: ' + TEST_ARRAY.join('&'));
}

// length is greater than 0, separator is empty string
// CHECK#7
var TEST_ARRAY = new Array(null, void 0, true, false, 123, new Object(), new Boolean(true) );
if (TEST_ARRAY.join('') !== "truefalse123[object Object]true"){
  $ERROR('#7: TEST_ARRAY.join("") === "truefalse123[object Object]true". Actual: ' + TEST_ARRAY.join(''));
}

// length is greater than 0, separator is undefined
// CHECK#8
var TEST_ARRAY = new Array(null, void 0, true, false, 123, new Object(), new Boolean(true) );
if (TEST_ARRAY.join(void 0) !== ",,true,false,123,[object Object],true"){
  $ERROR('#8: TEST_ARRAY.join(void 0) === ",,true,false,123,[object Object],true". Actual: ' + TEST_ARRAY.join(void 0));
}

// length is greater than 0, separator is not supplied
// CHECK#9
var TEST_ARRAY = new Array(null, void 0, true, false, 123, new Object(), new Boolean(true) );
if (TEST_ARRAY.join() !== ",,true,false,123,[object Object],true"){
  $ERROR('#9: TEST_ARRAY.join() === ",,true,false,123,[object Object],true". Actual: ' + TEST_ARRAY.join());
}

// separator is a control character
// CHECK#10
//var TEST_ARRAY = new Array(null, void 0, true, false, 123, new Object(), new Boolean(true) ); 
//if (TEST_ARRAY.join('\v') !== decodeURIComponent("%0B%0Btrue%0Bfalse%0B123%0B[object Object]%0Btrue")){
//  $ERROR('#10: TEST_ARRAY.join('\v') === decodeURIComponent("%0B%0Btrue%0Bfalse%0B123%0B[object Object]%0Btrue"). Actual: ' +TEST_ARRAY.join('\v'));
//}

// length of array is 1
// CHECK#11
var TEST_ARRAY = new Array(true); 
if (TEST_ARRAY.join('\v') !== "true"){
  $ERROR('#11: TEST_ARRAY.join("\v") ==="true". Actual: ' + TEST_ARRAY.join('\v'));
}
/*
SEPARATOR = "\t"
TEST_LENGTH = 100;
TEST_STRING = "";
ARGUMENTS = "";
TEST_RESULT = "";

for ( var index = 0; index < TEST_LENGTH; index++ ) {
  ARGUMENTS   += index;
  ARGUMENTS   += ( index == TEST_LENGTH -1 ) ? "" : ",";

  TEST_RESULT += index;
  TEST_RESULT += ( index == TEST_LENGTH -1 ) ? "" : SEPARATOR;
}

TEST_ARRAY = eval( "new Array( "+ARGUMENTS +")" );
// CHECK#12
if (TEST_ARRAY.join("+SEPARATOR+") !== TEST_RESULT){
  $ERROR('#12: TEST_ARRAY.join("+SEPARATOR+") === TEST_RESULT. Actual: ' + TEST_ARRAY.join( SEPARATOR ));
}
*/
/*
// CHECK#13
if ((new Array( Boolean(true), Boolean(false), null,  void 0, Number(1e+21), Number(1e-7))).join() !== "true,false,,,1e+21,1e-7"){
  $ERROR('#13: (new Array( Boolean(true), Boolean(false), null,  void 0, Number(1e+21), Number(1e-7))).join() === "true,false,,,1e+21,1e-7". Actual: ' 
         + (new Array( Boolean(true), Boolean(false), null,  void 0, Number(1e+21), Number(1e-7))).join());
}
*/
/*
// this is not an Array object
// CHECK#14
var OB = new Object_1('true,false,111,0.5,1.23e6,NaN,void 0,null');
if (OB.join(':')!== "true:false:111:0.5:1230000:NaN::"){
  $ERROR('#14: OB.join(":")=== "true:false:111:0.5:1230000:NaN::". Actual: ' + OB.join(':'));
}

function Object_1( value ) {
  this.array = value.split(",");
  this.length = this.array.length;
  for ( var i = 0; i < this.length; i++ ) {
    this[i] = eval(this.array[i]);
  }
  this.join = Array.prototype.join;
  this.getClass = Object.prototype.toString;
}*/