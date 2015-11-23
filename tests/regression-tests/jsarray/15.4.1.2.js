/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.4.1.2.js
   ECMA Section:       15.4.1.2 Array(len)

   Description:        When Array is called as a function rather than as a
   constructor, it creates and initializes a new array
   object.  Thus, the function call Array(...) is
   equivalent to the object creationi new Array(...) with
   the same arguments.

   An array is created and returned as if by the
   expression new Array(len).

   Author:             christine@netscape.com
   Date:               7 october 1997
*/
var SECTION = "15.4.1.2";
var VERSION = "ECMA_1";
var TITLE   = "Array Constructor Called as a Function:  Array(len)";

print (SECTION + " "+ TITLE);

expect = 0;
actual = (Array()).length;
if (actual !== expect)
  $ERROR("CHECK #1 failed :", actual, expect);

expect = 0;
actual = (Array(0)).length;
if (actual !== expect)
  $ERROR("CHECK #2 failed :", actual, expect);

expect = 1;
actual = (Array(1)).length;
if (actual !== expect)
  $ERROR("CHECK #3 failed :", actual, expect);

expect = 10;
actual = (Array(10)).length;
if (actual !== expect)
  $ERROR("CHECK #4 failed :", actual, expect);

expect = 1;
actual = (Array('1')).length;
if (actual !== expect)
  $ERROR("CHECK #5 failed :", actual, expect);

expect = 1000;
actual = (Array(1000)).length;
if (actual !== expect)
  $ERROR("CHECK #6 failed :", actual, expect);

expect = 1;
actual = (Array('1000')).length;
if (actual !== expect)
  $ERROR("CHECK #7 failed :", actual, expect);

/*
expect = ToUint32(4294967295);
actual = (Array(4294967295)).length;
if (actual !== expect)
  $ERROR("CHECK #8 failed :", actual, expect);

expect = ToUint32(Math.pow(2,31)-1);
actual = (Array(Math.pow(2,31)-1)).length;
if (actual !== expect)
  $ERROR("CHECK #9 failed :", actual, expect);


expect = ToUint32(Math.pow(2,31));
actual = (Array(Math.pow(2,31))).length;
if (actual !== expect)
  $ERROR("CHECK #10 failed :", actual, expect);

expect = ToUint32(Math.pow(2,31)+1);
actual = (Array(Math.pow(2,31)+1)).length;
if (actual !== expect)
  $ERROR("CHECK #11 failed :", actual, expect);
*/
expect = 1;
actual = (Array('8589934592')).length;
if (actual !== expect)
  $ERROR("CHECK #12 failed :", actual, expect);

expect = 1;
actual = (Array('4294967296')).length;
if (actual !== expect)
  $ERROR("CHECK #13 failed :", actual, expect);

/*
expect = ToUint32(1073741823);
actual = (Array(1073741823)).length;
if (actual !== expect)
  $ERROR("CHECK #14 failed :", actual, expect);

expect = ToUint32(1073741824);
actual = (Array(1073741824)).length;
if (actual !== expect)
  $ERROR("CHECK #15 failed :", actual, expect);
*/

expect = 1;
actual = (Array('a string')).length;
if (actual !== expect)
  $ERROR("CHECK #16 failed :", actual, expect);

/*
function ToUint32( n ) {
  n = Number( n );
  var sign = ( n < 0 ) ? -1 : 1;

  if ( Math.abs( n ) == 0 || Math.abs( n ) == Number.POSITIVE_INFINITY) {
    return 0;
  }
  n = sign * Math.floor( Math.abs(n) )

    n = n % Math.pow(2,32);

  if ( n < 0 ){
    n += Math.pow(2,32);
  }

  return ( n );
}
*/