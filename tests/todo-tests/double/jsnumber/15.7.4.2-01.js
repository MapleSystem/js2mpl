/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = "411889";
var summary = "num.toString(), num.toString(10), and num.toString(undefined)" +
              " should all be equivalent";
var actual, expect;

print(BUGNUMBER);
print(summary);

/**************
 * BEGIN TEST *
 **************/
// CHECK#1
var noargs = 3.3.toString();
var tenarg = 3.3.toString(10);
var undefarg = 3.3.toString(undefined);
if (noargs !== tenarg){
  $ERROR('#1:'+ "() !== (10): " + noargs + " !== " + tenarg);
}
if (tenarg !== undefarg){
  $ERROR('#1:'+ "(10) !== (undefined): " + tenarg + " !== " + undefarg);
}

expect = 1;
actual = 3.3.toString.length;
// CHECK#2
if (actual !== expect){
  $ERROR('#2: 3.3.toString.length should be 1 ' + actual);
}
