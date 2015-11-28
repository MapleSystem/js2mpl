/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 421325;
var summary = 'Dense Arrays and holes';
var actual = '';
var expect = '';


print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);
 
Array.prototype[1] = 'bar';
var a = []; 
a[0]='foo'; 
a[2] = 'baz'; 
expect = 'foo,bar,baz';
actual = a + '';
if (actual !== expect)
  $ERROR("testcase failed :", actual, expect);
