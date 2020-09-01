// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

//-----------------------------------------------------------------------------
var BUGNUMBER = 548671;
var summary =
  "Don't use a shared-permanent inherited property to implement " +
  "[].length or (function(){}).length";

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/

var a = [];
a.p = 1;
var x = Object.create(a);
// CHECK#1
if (x.length !== 0){
  $ERROR('#1: x.length === 0. Actual: ' + x.length);
}

// CHECK#2
if (x.p !== 1){
  $ERROR('#2: x.p === 1. Actual: ' + x.p);
}

// CHECK#3
if (a.length !== 0){
  $ERROR('#3: a.length === 0. Actual: ' + a.length);
}


