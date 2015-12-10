// |reftest| fails
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Date: 2001-07-15
 *
 * SUMMARY: Testing Number.prototype.toPrecision(precision)
 * See EMCA 262 Edition 3 Section 15.7.4.7
 *
 * Also see http://bugzilla.mozilla.org/show_bug.cgi?id=90551
 *
 */
//-----------------------------------------------------------------------------
var BUGNUMBER = '(none)';
var summary = 'Testing Number.prototype.toPrecision(precision)';
print(BUGNUMBER);
print(summary);

// CHECK#1
var testNum = 5.123456;
if (testNum.toPrecision(4) !== '5.123'){
  $ERROR('#1: testNum.toPrecision(4) !== "5.123". Actual: ' + testNum.toPrecision(4));
}

// CHECK#2
if (Number.POSITIVE_INFINITY.toPrecision(-3) !== 'Infinity'){
  $ERROR('#2: Number.POSITIVE_INFINITY.toPrecision(-3) !== "Infinity". Actual: ' + Number.POSITIVE_INFINITY.toPrecision(-3));
}

// CHECK#3
if (Number.NEGATIVE_INFINITY.toPrecision(-3) !== '-Infinity'){
  $ERROR('#3: Number.NEGATIVE_INFINITY.toPrecision(-3) !== "-Infinity". Actual: ' + Number.NEGATIVE_INFINITY.toPrecision(-3));
}

// CHECK#4
if (Number.NaN.toPrecision(-3) !== 'NaN'){
  $ERROR('#4: Number.NaN.toPrecision(-3) !== "NaN". Actual: ' + Number.NaN.toPrecision(-3));
}