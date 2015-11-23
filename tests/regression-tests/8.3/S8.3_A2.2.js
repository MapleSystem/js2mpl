// Copyright 2009 the Sputnik authors.  All rights reserved.
// This code is governed by the BSD license found in the LICENSE file.

/**
 * The false is reserved word
 *
 * @path ch08/8.3/S8.3_A2.2.js
 * @description Checking if execution of "false=0" fails
 * @negative
 */

var x = true;
var y = false;

if (Number(x) !== 1) {
            $ERROR("#1.1 x !== true, but actual is "+ x);
}

if (Number(y) !== 0) {
            $ERROR("#1.1 false !== 0, but actual is "+ y);
}

