// Copyright 2009 the Sputnik authors.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/**
 * First expression is evaluated first, and then second expression
 *
 * @path ch11/11.8/11.8.7/S11.8.7_A2.4_T3.js
 * @description Checking with undeclarated variables
 */

//CHECK#1
if ((NUMBER = Number, "MAX_VALUE") in NUMBER !== true) {
  $ERROR('#1: (NUMBER = Number, "MAX_VALUE") in NUMBER !== true');
}