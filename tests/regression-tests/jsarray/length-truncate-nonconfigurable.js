/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------

var BUGNUMBER = 858381;
var summary =
  "Array length redefinition behavior with non-configurable elements";

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/
 
function assertEq(message, actual, expected)
{
  if (actual !== expected){
    $ERROR('CHECK #'+ message +': ' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

var arr = [0, 1, 2];
Object.defineProperty(arr, 1, { configurable: false });

Object.defineProperty(arr, "length", { value: 0, writable: false });

assertEq("#1: length is highest remaining index plus one", arr.length, 2);

var desc = Object.getOwnPropertyDescriptor(arr, "length");
assertEq(6, desc !== undefined, true);

assertEq(2, desc.value, 2);
assertEq(3, desc.writable, false);
assertEq(4, desc.enumerable, false);
assertEq(5, desc.configurable, false);
