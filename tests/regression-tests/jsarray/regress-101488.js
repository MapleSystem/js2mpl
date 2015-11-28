/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Date: 24 September 2001
 *
 * SUMMARY: Try assigning arr.length = new Number(n)
 * From correspondence with Igor Bukanov <igor@icesoft.no>
 * See http://bugzilla.mozilla.org/show_bug.cgi?id=101488
 *
 * Without the "new" keyword, assigning arr.length = Number(n) worked.
 * But with it, Rhino was giving an error "Inappropriate array length"
 * and SpiderMonkey was exiting without giving any error or return value -
 *
 * Comments on the Rhino code by igor@icesoft.no:
 *
 * jsSet_length requires that the new length value should be an instance
 * of Number. But according to Ecma 15.4.5.1, item 12-13, an error should
 * be thrown only if ToUint32(length_value) != ToNumber(length_value)
 */
//-----------------------------------------------------------------------------
var UBound = 0;
var BUGNUMBER = 101488;
var summary = 'Try assigning arr.length = new Number(n)';
var status = '';
var statusitems = [];
var actual = '';
var actualvalues = [];
var expect= '';
var expectedvalues = [];
var arr = [];

print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);

arr = Array();
arr.length = new Number(1);
actual = arr.length;
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #1:", actual, expect);

arr = Array(5);
arr.length = new Number(1);
actual = arr.length;
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #2:", actual, expect);

arr = Array();
arr.length = new Number(17);
actual = arr.length;
expect = 17;
if (actual !== expect)
  $ERROR("CHECK #3:", actual, expect);

arr = Array(5);
arr.length = new Number(17);
actual = arr.length;
expect = 17;
if (actual !== expect)
  $ERROR("CHECK #4:", actual, expect);


/*
 * Also try the above with the "new" keyword before Array().
 * Array() and new Array() should be equivalent, by ECMA 15.4.1.1
 */
arr = new Array();
arr.length = new Number(1);
actual = arr.length;
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #5:", actual, expect);

arr = new Array(5);
arr.length = new Number(1);
actual = arr.length;
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #6:", actual, expect);

arr = new Array();
arr.length = new Number(17);
actual = arr.length;
expect = 17;
if (actual !== expect)
  $ERROR("CHECK #7:", actual, expect);

arr = new Array(5);
arr.length = new Number(17);
actual = arr.length;
expect = 17;
if (actual !== expect)
  $ERROR("CHECK #8:", actual, expect);
