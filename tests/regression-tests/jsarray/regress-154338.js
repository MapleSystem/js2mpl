/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 *
 * Date:    26 June 2002
 * SUMMARY: Testing array.join() when separator is a variable, not a literal
 * See http://bugzilla.mozilla.org/show_bug.cgi?id=154338
 *
 */
//-----------------------------------------------------------------------------
var UBound = 0;
var BUGNUMBER = 154338;
var summary = 'Test array.join() when separator is a variable, not a literal';
var status = '';
var statusitems = [];
var actual = '';
var actualvalues = [];
var expect= '';
var expectedvalues = [];

function printBugNumber (num)
{
  BUGNUMBER = num;
  print ('BUGNUMBER: ' + num);
}
var SECT_PREFIX = 'Section ';
var SECT_SUFFIX = ' of test - ';

function inSection(x)
{
  return SECT_PREFIX + x + SECT_SUFFIX;
}

printBugNumber(BUGNUMBER);
print(summary);


var x = 'H';
var y = 'ome';


status = inSection(1);
var arr = Array('a', 'b');
actual = arr.join('H');
expect = 'aHb';
if (actual !== expect)
  $ERROR(status, actual, expect); 

status = inSection(2);
arr = Array('a', 'b');
actual = arr.join(x);
expect = 'aHb';
if (actual !== expect)
  $ERROR(status, actual, expect); 

status = inSection(3);
arr = Array('a', 'b');
actual = arr.join('ome');
expect = 'aomeb';
if (actual !== expect)
  $ERROR(status, actual, expect); 

status = inSection(4);
arr = Array('a', 'b');
actual = arr.join(y);
expect = 'aomeb';
if (actual !== expect)
  $ERROR(status, actual, expect); 