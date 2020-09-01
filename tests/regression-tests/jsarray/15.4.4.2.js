/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.4.2.js
   ECMA Section:       15.4.4.2 Array.prototype.toString()
   Description:        The elements of this object are converted to strings
   and these strings are then concatenated, separated by
   comma characters.  The result is the same as if the
   built-in join method were invoiked for this object
   with no argument.
   Author:             christine@netscape.com
   Date:               7 october 1997
*/

var SECTION = "15.4.4.2";
var VERSION = "ECMA_1";
var TITLE   = "Array.prototype.toString";

print (SECTION + " "+ TITLE);

// CHECK#1
if (Array.prototype.toString.length !== 0){
  $ERROR('#1: Array.prototype.toString.length === 0. Actual: ' + Array.prototype.toString.length);
}

// CHECK#2
if ((new Array()).toString() !== ""){
  $ERROR('#2: (new Array()).toString() === "". Actual: ' + (new Array()).toString());
}

// CHECK#3
if ((new Array(2)).toString() !== ","){
  $ERROR('#3: (new Array(2)).toString() === "". Actual: ' + (new Array(2)).toString());
}

// CHECK#4
if ((new Array(0,1)).toString() !== "0,1"){
  $ERROR('#4: (new Array(0,1)).toString() === "". Actual: ' + (new Array(0,1)).toString());
}

/*
// CHECK#5
if ((new Array( Number.NaN, Number.POSITIVE_INFINITY, Number.NEGATIVE_INFINITY)).toString() !== "NaN,Infinity,-Infinity"){
  $ERROR('#5: (new Array( Number.NaN, Number.POSITIVE_INFINITY, Number.NEGATIVE_INFINITY)).toString() === "NaN,Infinity,-Infinity". Actual: '
         + (new Array( Number.NaN, Number.POSITIVE_INFINITY, Number.NEGATIVE_INFINITY)).toString());
}
*/
// CHECK#6
//if ((new Array( Boolean(1), Boolean(0))).toString() !== "true,false"){
//  $ERROR('#6: (new Array( Boolean(1), Boolean(0))).toString() === "true,false". Actual: ' + (new Array( Boolean(1), Boolean(0))).toString());
//}

// CHECK#7
if ((new Array(void 0,null)).toString() !== ","){
  $ERROR('#1: (new Array(void 0,null)).toString() === ",". Actual: ' + (new Array(void 0,null)).toString());
}


