/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Date: 2001-07-15
 *
 * SUMMARY: Testing Number.prototype.toFixed(fractionDigits)
 * See EMCA 262 Edition 3 Section 15.7.4.5
 *
 * Also see http://bugzilla.mozilla.org/show_bug.cgi?id=90551
 *
 */
//-----------------------------------------------------------------------------

var BUGNUMBER = '(none)';
var summary = 'Testing Number.prototype.toFixed(fractionDigits)';
print(BUGNUMBER);
print(summary);

// CHECK#1
var testNum = 234.2040506;
if (testNum.toFixed(4) !== '234.2041'){
  $ERROR('#1: testNum.toFixed(4) !== "234.2041". Actual: ' + testNum.toFixed(4));
}

// CHECK#2
if (0.00001.toFixed(2) !== '0.00'){
  $ERROR('#2:0.00001.toFixed(2) !== "0.00". Actual: ' + 0.00001.toFixed(2));
}

// CHECK#3
if (0.000000000000000000001.toFixed(20) !== '0.00000000000000000000'){
  $ERROR('#3: 0.000000000000000000001.toFixed(20) !== "0.00000000000000000000". Actual: ' + 0.000000000000000000001.toFixed(20));
}