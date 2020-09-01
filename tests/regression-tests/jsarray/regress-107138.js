/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 * Date: 29 October 2001
 *
 * SUMMARY: Regression test for bug 107138
 * See http://bugzilla.mozilla.org/show_bug.cgi?id=107138
 *
 * The bug: arr['1'] == undefined instead of arr['1'] == 'one'.
 * The bug was intermittent and did not always occur...
 *
 * The cnSTRESS constant defines how many times to repeat this test.
 */
//-----------------------------------------------------------------------------
var UBound = 0;
var cnSTRESS = 10;
var cnDASH = '-';
var BUGNUMBER = 107138;
var summary = 'Regression test for bug 107138';
var status = '';
var statusitems = [];
var actual = '';
var actualvalues = [];
var expect= '';
var expectedvalues = [];


/*
 * Print a bugnumber message.
 */
function printBugNumber (num)
{
  BUGNUMBER = num;
  print ('BUGNUMBER: ' + num);
}
var SECT_SUFFIX = ' of test - ';
function inSection(x)
{
  return x + SECT_SUFFIX;
}

printBugNumber(BUGNUMBER);
print(summary);
var arr = ['zero', 'one'];
// This bug was intermittent. Stress-test it.
for (var j=0; j<cnSTRESS; j++)
{
  status = inSection(j + cnDASH + 1);
  actual = arr[0];
  expect = 'zero';
  if (actual !== expect) {
    $ERROR(status, actual, expect); 
  }

  status = inSection(j + cnDASH + 2);
  actual = arr['0'];
  expect = 'zero';
  if (actual !== expect) {
    $ERROR(status, actual, expect); 
  }

  status = inSection(j + cnDASH + 3);
  actual = arr[1];
  expect = 'one';
  if (actual !== expect) {
    $ERROR(status, actual, expect); 
  }

  status = inSection(j + cnDASH + 4);
  actual = arr['1'];
  expect = 'one';
  if (actual !== expect) {
    $ERROR(status, actual, expect); 
  }
}
