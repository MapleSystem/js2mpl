/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.2.1-1.js
   ECMA Section:       15.4.2.1 new Array( item0, item1, ... )
   Description:        This description only applies of the constructor is
   given two or more arguments.

   The [[Prototype]] property of the newly constructed
   object is set to the original Array prototype object,
   the one that is the initial value of Array.prototype
   (15.4.3.1).

   The [[Class]] property of the newly constructed object
   is set to "Array".

   The length property of the newly constructed object is
   set to the number of arguments.

   The 0 property of the newly constructed object is set
   to item0... in general, for as many arguments as there
   are, the k property of the newly constructed object is
   set to argument k, where the first argument is
   considered to be argument number 0.

   This file tests the typeof the newly constructed object.

   Author:             christine@netscape.com
   Date:               7 october 1997
*/

var SECTION = "15.4.2.1-1";
var VERSION = "ECMA_1";
var TITLE   = "The Array Constructor:  new Array( item0, item1, ...)";

print (SECTION + " "+ TITLE);

actual = typeof new Array(1,2);
expect = "object";
if (actual !== expect)
  $ERROR("CHECK #1 failed :", actual, expect);

actual = (new Array(1,2)).toString;
expect = Array.prototype.toString;
if (actual !== expect)
  $ERROR("CHECK #2 failed :", actual, expect);

var arr = new Array(1,2,3); 
arr.getClass = Object.prototype.toString;
actual = arr.getClass();
expect = "[object Array]";
if (actual !== expect)
  $ERROR("CHECK #3 failed :", actual, expect);

actual = (new Array(1,2)).length;
expect = 2;
if (actual !== expect)
  $ERROR("CHECK #4 failed :", actual, expect);

var arr = (new Array(1,2));
actual = arr[0];
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #5 failed :", actual, expect);

var arr = (new Array(1,2));
actual = arr[1];
expect = 2;
if (actual !== expect)
  $ERROR("CHECK #6 failed :", actual, expect);

var arr = (new Array(1,2));
actual = String(arr);
expect = "1,2";
if (actual !== expect)
  $ERROR("CHECK #7 failed :", actual, expect);