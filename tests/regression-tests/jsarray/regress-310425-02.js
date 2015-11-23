/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 310425;
var summary = 'Array.indexOf/lastIndexOf edge cases';
var actual = '';
var expect = '';

print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);
 
expect = -1;
actual = Array(1).indexOf(1);
if (actual !== expect)
  $ERROR("testcase failed :", actual, expect);
