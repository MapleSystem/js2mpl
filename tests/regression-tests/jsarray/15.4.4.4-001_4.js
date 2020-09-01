/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/*
 *
 * Date:    19 September 2002
 * SUMMARY: Testing Array.prototype.concat()
 * See http://bugzilla.mozilla.org/show_bug.cgi?id=169795
 *
 */
 
var UBound = 0;
var BUGNUMBER = 169795;
var summary = 'Testing Array.prototype.concat(): [].concat(x)';
var status;
var statusitems = [];
var actual = '';
var actualvalues = [];
var expect= '';
var expectedvalues = [];
var x;

print('BUGNUMBER:',BUGNUMBER);
print(summary);

status = '#6';
x = [function() {return "Hello";}];
actual = [].concat(x).toString();
expect = x.toString();
if(actual !== expect)
  $ERROR(status, expect, actual);