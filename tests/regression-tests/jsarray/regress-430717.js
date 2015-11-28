/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 430717;
var summary = 'Dense Arrays should inherit deleted elements from Array.prototype';
var actual = '';
var expect = '';
print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);

Array.prototype[2] = "two";
var a = [0,1,2,3];
delete a[2];

expect = 'two';
actual = a[2];
if (actual !== expect)
  $ERROR("testcase failed :", actual, expect);