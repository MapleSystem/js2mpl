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
actual = [].lastIndexOf(undefined, -1);
if (actual !== expect)
  $ERROR("CHECK #1:", actual, expect);

expect = -1;
actual = [].indexOf(undefined, -1);
if (actual !== expect)
  $ERROR("CHECK #2:", actual, expect);

var x = [];
x[1 << 8] = 1;
expect = (1 << 8);
actual = x.lastIndexOf(1);
if (actual !== expect)
  $ERROR("CHECL #3:", actual, expect);
