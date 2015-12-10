/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 469397;
var summary = '(0.5).toFixed(0) == 1';
print(BUGNUMBER);
print(summary);

// CHECK#1
if ((0.5).toFixed(0) !== '1'){
  $ERROR('#1: (0.5).toFixed(0) !== "1". Actual: ' + (0.5).toFixed(0));
}