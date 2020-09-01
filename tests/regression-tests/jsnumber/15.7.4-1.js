/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.4-1.js
   ECMA Section:       15.7.4.1 Properties of the Number Prototype Object
   Description:
   Author:             christine@netscape.com
   Date:               16 september 1997
*/


var SECTION = "15.7.4-1";
var VERSION = "ECMA_1";
print( SECTION + "Properties of the Number prototype object");

// CHECK#1
if (Number.prototype.valueOf() !== 0){
  $ERROR('#1: Number.prototype.valueOf() !== 0. Actual: ' + Number.prototype.valueOf());
}
// CHECK#2
if (typeof(Number.prototype) !== "object"){
  $ERROR('#2: typeof(Number.prototype) !== "object". Actual: ' + typeof(Number.prototype));
}
// CHECK#3
if (Number.prototype.constructor !== Number){
  $ERROR('#3: Number.prototype.constructor !== Number. Actual: ' + Number.prototype.constructor);
}