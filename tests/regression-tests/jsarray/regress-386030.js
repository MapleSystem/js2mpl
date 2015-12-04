/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor: Blake Kaplan
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 386030;
var summary = 'Array.reduce should ignore holes';
var actual = '';
var expect = '';

print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);

function add(a, b) { return a + b; }

expect = 3;

a = new Array(2);
a[1] = 3;
actual = a.reduce(add);
if (actual !== expect)
  $ERROR(summary + ': 1', actual, expect);


a = new Array(2);
a[0] = 3;
actual = a.reduceRight(add);
if (actual !== expect)
  $ERROR(summary + ': 2', actual, expect);
