// Copyright 2009 the Sputnik authors.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/**
 * Result of boolean conversion from undefined value is false
 *
 * @path ch09/9.2/S9.2_A1_T1.js
 * @description Undefined, void and others are converted to Boolean by explicit transformation
 */
// CHECK#1
if (Boolean(undefined) !== false) {
  $ERROR('#1: Boolean(undefined) === false. Actual: ' + (Boolean(undefined)));
}

// CHECK#2
if (Boolean(void 0) !== false) {
  print('#2: Boolean(void 0) == false. Actual: ' + (Boolean(void 0)));
}
// CHECK#3
//if (Boolean("var x") !== false) {
//  print('#3: Boolean(eval("var x")) === false. Actual: ' + (Boolean(eval("var x"))));
//}

// CHECK#4
if (Boolean() !== false) {
  $ERROR('#4: Boolean() === false. failed ');
}

