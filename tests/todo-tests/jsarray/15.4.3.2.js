/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.3.2.js
   ECMA Section:       15.4.3.2 Array.length
   Description:        The length property is 1.

   Author:             christine@netscape.com
   Date:               7 october 1997
*/

var SECTION = "15.4.3.2";
var VERSION = "ECMA_1";
var TITLE   = "Array.length";
print (SECTION + " "+ TITLE);

// CHECK#1
if (Array.length !== 1){
  $ERROR('#1: Array.length === 1. Actual: ' + Array.length);
}