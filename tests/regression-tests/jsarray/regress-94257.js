/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Date: 30 October 2001
 *
 * SUMMARY: Regression test for bug 94257
 * See http://bugzilla.mozilla.org/show_bug.cgi?id=94257
 *
 * Rhino used to crash on this code; specifically, on the line
 *
 *                       arr[1+1] += 2;
 */
//-----------------------------------------------------------------------------
var UBound = 0;
var BUGNUMBER = 94257;
var summary = "Making sure we don't crash on this code -";
var status = '';
var statusitems = [];
var actual = '';
var actualvalues = [];
var expect= '';
var expectedvalues = [];


var arr = new Array(6);
arr[1+1] = 1;
arr[1+1] += 2;



actual = arr[1+1];
expect = 3;
// CHECK#1
if (actual !== expect) {
  $ERROR('#1:', actual, expect); 
}


actual = arr[1+1+1];
expect = undefined;
// CHECK#2
if (actual !== expect) {
  $ERROR('#2: ', actual, expect); 
}


actual = arr[1];
expect = undefined;
// CHECK#3
if (actual !== expect) {
  $ERROR('#3: ', actual, expect); 
}


arr[1+2] = 'Hello';

actual = arr[1+1+1];
expect = 'Hello';
// CHECK#4
if (actual !== expect) {
  $ERROR('#4: ', actual, expect); 
}
