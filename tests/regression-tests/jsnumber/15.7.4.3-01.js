/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = "412068";
var summary = "num.toLocaleString incorrectly accesses its first argument " +
              "even when no first argument has been given";

print(BUGNUMBER);
print(summary);

/**************
 * BEGIN TEST *
 **************/
// CHECK#1
var a = 3;
if ("3" !== a.toLocaleString()){
  $ERROR('#1: "3" !== 3..toLocaleString(). Actual: ' +a.toLocaleString());
}
// CHECK#2
var b = 9;
if ("9" !== b.toLocaleString(8)){
  $ERROR('#2: "9" !== 9..toLocaleString(8). Actual: ' +b.toLocaleString(8));
}