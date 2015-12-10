/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = "411893";
var summary = "num.toPrecision(undefined) should equal num.toString()";

print(BUGNUMBER);
print(summary);

/**************
 * BEGIN TEST *
 **************/
// CHECK#1
var a = 3.3;
//print(a.toPrecision(undefined))
//print(a.toString())
//print(a.toPrecision(undefined) === a.toString())//true
if (a.toPrecision(undefined) !== a.toString()){
  print('#1: 3.3.toPrecision(undefined) !== 3.3.toString(). Actual: ')
//  print(a.toPrecision(undefined))//if use a.toPrecision(undefined) in this block, the result will be unequal
//  $ERROR('#1: 3.3.toPrecision(undefined) !== 3.3.toString(). Actual: ' + a.toPrecision(undefined));
}