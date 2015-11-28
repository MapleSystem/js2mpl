/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.1.js
   ECMA Section:       15.4.1 The Array Constructor Called as a Function

   Description:        When Array is called as a function rather than as a
   constructor, it creates and initializes a new array
   object.  Thus, the function call Array(...) is
   equivalent to the object creationi new Array(...) with
   the same arguments.

   Author:             christine@netscape.com
   Date:               7 october 1997
*/

var SECTION = "15.4.1";
var VERSION = "ECMA_1";
var TITLE   = "The Array Constructor Called as a Function";

print (SECTION + " "+ TITLE);


actual = Array(1,2,3) +'';
expect = "1,2,3";
if (actual !== expect)
  $ERROR("CHECK #6 failed :", actual, expect);

actual = typeof Array(1,2,3);
expect = "object";
if (actual !== expect)
  $ERROR("CHECK #7 failed :", actual, expect);

var arr = Array(1,2,3);
arr.getClass = Object.prototype.toString;
actual = arr.getClass();
expect = "[object Array]";
if (actual !== expect)
  $ERROR("CHECK #8 failed :", actual, expect);