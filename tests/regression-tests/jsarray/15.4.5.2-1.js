/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.5.2-1.js
   ECMA Section:       Array.length
   Description:
   15.4.5.2 length
   The length property of this Array object is always numerically greater
   than the name of every property whose name is an array index.

   The length property has the attributes { DontEnum, DontDelete }.
   Author:             christine@netscape.com
   Date:               12 november 1997
*/

var SECTION = "15.4.5.2-1";
var VERSION = "ECMA_1";
var TITLE   = "Array.length";

print (SECTION + " "+ TITLE);

// CHECK#1
var A = new Array();
if (A.length !== 0){
  $ERROR('#1: A.length === 0. Actual: ' + A.length);
}

/*
new TestCase(   SECTION,
		"var A = new Array(); A[Math.pow(2,32)-2] = 'hi'; A.length",
		Math.pow(2,32)-1,
		eval("var A = new Array(); A[Math.pow(2,32)-2] = 'hi'; A.length") );
*/

// CHECK#2
var A = new Array();
A.length = 123;
if (A.length !== 123){
  $ERROR('#2: A.length === 123. Actual: ' + A.length);
}
/*
// CHECK#3
var A = new Array();
A.length = 123;
var PROPS = '';
for ( var p in A )
{ PROPS += ( p == 'length' ? p : '');}
if (PROPS!== ""){
  $ERROR('#3: PROPS === "". Actual: ' + PROPS);
}
*/
/*
// CHECK#4
var A = new Array();
A.length = 123;
if (delete A.length!== false){
  $ERROR('#4: delete A.length === false. Actual: ' + delete A.length);
}
*/
// CHECK#1
var A = new Array();
A.length = 123;
delete A.length;
if (A.length!== 123){
  $ERROR('#1: A.length === 123. Actual: ' + A.length);
}
