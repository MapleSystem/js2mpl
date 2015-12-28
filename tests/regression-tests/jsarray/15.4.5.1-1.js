/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.5.1-1.js
   ECMA Section:       [[ Put]] (P, V)
   Description:
   Array objects use a variation of the [[Put]] method used for other native
   ECMAScript objects (section 8.6.2.2).

   Assume A is an Array object and P is a string.

   When the [[Put]] method of A is called with property P and value V, the
   following steps are taken:

   1.  Call the [[CanPut]] method of A with name P.
   2.  If Result(1) is false, return.
   3.  If A doesn't have a property with name P, go to step 7.
   4.  If P is "length", go to step 12.
   5.  Set the value of property P of A to V.
   6.  Go to step 8.
   7.  Create a property with name P, set its value to V and give it empty
   attributes.
   8.  If P is not an array index, return.
   9.  If A itself has a property (not an inherited property) named "length",
   andToUint32(P) is less than the value of the length property of A, then
   return.
   10. Change (or set) the value of the length property of A to ToUint32(P)+1.
   11. Return.
   12. Compute ToUint32(V).
   13. For every integer k that is less than the value of the length property
   of A but not less than Result(12), if A itself has a property (not an
   inherited property) named ToString(k), then delete that property.
   14. Set the value of property P of A to Result(12).
   15. Return.
   Author:             christine@netscape.com
   Date:               12 november 1997
*/

var SECTION = "15.4.5.1-1";
var VERSION = "ECMA_1";
var TITLE   = "Array [[Put]] (P, V)";

print (SECTION + " "+ TITLE);

// P is "length"
// CHECK#1
var A = new Array(); A.length = 100; 
if (A.length !== 100){
  $ERROR('#1: A.length === 100. Actual: ' + A.length);
}

// A has Property P, and P is not length or an array index
// CHECK#2
var A = new Array(100); A.name = 'name of this array';
if (A.name !== "name of this array"){
  $ERROR('#2: A.name === "name of this array". Actual: ' + A.name);
}

// CHECK#3
var A = new Array(100); A.name = 'name of this array';
if (A.length !== 100){
  $ERROR('#3: A.length === 1000. Actual: ' + A.length);
}

// A has Property P, P is not length, P is an array index, and ToUint32(p) is less than the
// value of length
// CHECK#4
var A = new Array(200); A[123] = 'hola';
if (A[123].toString() !== "hola"){
  $ERROR('#4: A[123] === "hola". Actual: ' + A[123]);
}

// CHECK#5
var A = new Array(200); A[123] = 'hola'; 
if (A.length !== 200){
  $ERROR('#5: A.length === 200. Actual: ' + A.length);
}
