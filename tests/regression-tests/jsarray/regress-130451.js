/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 *
 * Date:    25 Mar 2002
 * SUMMARY: Array.prototype.sort() should not (re-)define .length
 * See http://bugzilla.mozilla.org/show_bug.cgi?id=130451
 *
 * From the ECMA-262 Edition 3 Final spec:
 *
 * NOTE: The sort function is intentionally generic; it does not require that
 * its |this| value be an Array object. Therefore, it can be transferred to
 * other kinds of objects for use as a method. Whether the sort function can
 * be applied successfully to a host object is implementation-dependent.
 *
 * The interesting parts of this testcase are the contrasting expectations for
 * Brendan's test below, when applied to Array objects vs. non-Array objects.
 *
 */
//-----------------------------------------------------------------------------
var UBound = 0;
var BUGNUMBER = 130451;
var summary = 'Array.prototype.sort() should not (re-)define .length';
var status = '';
var statusitems = [];
var actual = '';
var actualvalues = [];
var expect= '';
var expectedvalues = [];
var arr = [];
var cmp;

print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);
/*
 * First: test Array.prototype.sort() on Array objects
 */
arr = [0,1,2,3];
cmp = function(x,y) {return x-y;};
actual = arr.sort(cmp).length;
expect = 4;
if (actual !== expect)
  $ERROR("CHECK #1:", actual, expect);

arr = [0,1,2,3];
cmp = function(x,y) {return y-x;};
actual = arr.sort(cmp).length;
expect = 4;
if (actual !== expect)
  $ERROR("CHECK #2:", actual, expect);

arr = [0,1,2,3];
cmp = function(x,y) {return x-y;};
arr.length = 1;
actual = arr.sort(cmp).length;
expect = 1;
if (actual !== expect)
  $ERROR("CHECK #3:", actual, expect);

/*
 * This test is by Brendan. Setting arr.length to
 * 2 and then 4 should cause elements to be deleted.
 */
arr = [0,1,2,3];
cmp = function(x,y) {return x-y;};
arr.sort(cmp);

actual = arr.join();
expect = '0,1,2,3';
if (actual !== expect)
  $ERROR("CHECK #4:", actual, expect);

actual = arr.length;
expect = 4;
if (actual !== expect)
  $ERROR("CHECK #5:", actual, expect);

arr.length = 2;
actual = arr.join();
expect = '0,1';
if (actual !== expect)
  $ERROR("CHECK #6:", actual, expect);

arr.length = 4;
actual = arr.join();
expect = '0,1,,';  //<---- see how 2,3 have been lost
if (actual !== expect)
  $ERROR("CHECK #7:", actual, expect);