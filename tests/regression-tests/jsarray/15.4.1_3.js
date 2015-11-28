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


var arr = Array(1,2,3);
if (arr.toString !== Array.prototype.toString)
  $ERROR("CHECK #9 failed :", arr.toString, Array.prototype.toString);

actual = Array(1,2,3).length;
expect = 3;
if (actual !== expect)
  $ERROR("CHECK #10 failed :", actual, expect);

actual = typeof Array(12345);
expect = "object";
if (actual !== expect)
  $ERROR("CHECK #11 failed :", actual, expect);

var arr = Array(12345);
arr.getClass = Object.prototype.toString;
actual = arr.getClass();
expect = "[object Array]";
if (actual !== expect)
  $ERROR("CHECK #12 failed :", actual, expect);

var arr = Array(1,2,3,4,5);
if (arr.toString !== Array.prototype.toString)
  $ERROR("CHECK #13 failed :", arr.toString, Array.prototype.toString);

actual = Array(12345).length;
expect = 12345;
if (actual !== expect)
  $ERROR("CHECK #14 failed :", actual, expect);