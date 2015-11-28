/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.1.1.js
   ECMA Section:       15.4.1 Array( item0, item1,... )

   Description:        When Array is called as a function rather than as a
   constructor, it creates and initializes a new array
   object.  Thus, the function call Array(...) is
   equivalent to the object creation new Array(...) with
   the same arguments.

   An array is created and returned as if by the expression
   new Array( item0, item1, ... ).

   Author:             christine@netscape.com
   Date:               7 october 1997
*/
var SECTION = "15.4.1.1";
var VERSION = "ECMA_1";
var TITLE   = "Array Constructor Called as a Function";

print (SECTION + " "+ TITLE);

actual = (Array(1,2)).length;
expect = 2;
if (actual !== expect)
  $ERROR("CHECK #4 failed :", actual, expect);

var arr = (Array(1,2));
actual = arr[0];
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #5 failed :", actual, expect);

var arr = (Array(1,2));
actual = arr[1];
expect = 2;
if (actual !== expect)
  $ERROR("CHECK #6 failed :", actual, expect);

var arr = (Array(1,2));
actual = String(arr);
expect = "1,2";
if (actual !== expect)
  $ERROR("CHECK #7 failed :", actual, expect);