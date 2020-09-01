/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 260106;
var summary = 'Elisions in Array literals should not be enumed';
var actual = '';
var expect = '';
var status;
var prop;
var array;
var SECT_PREFIX = 'Section ';
var SECT_SUFFIX = ' of test - ';

function inSection(x)
{
  return SECT_PREFIX + x + SECT_SUFFIX;
}

function printBugNumber (num)
{
  BUGNUMBER = num;
  print ('BUGNUMBER: ' + num);
}

printBugNumber(BUGNUMBER);
print (summary);

status = summary + ' ' + inSection(2) + ' [,,1] ';
array = [,,1];
actual = '';
expect = '2';
for (prop in array)
{
  if (prop != 'length')
  {
    actual += prop;
  }
}
// CHECK#2
if (expect !== actual)
  $ERROR('#2:', status, expect, actual); 