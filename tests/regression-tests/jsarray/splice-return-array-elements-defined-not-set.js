/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 668024;
var summary =
  'Array.prototype.splice should define, not set, the elements of the array ' +
  'it returns';

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

var arr = [0, 1, 2, 3, 4, 5];
var removed = arr.splice(0, 6);

assertEq(1,arr.length, 0);
assertEq(2,removed.length, 6);
assertEq(3,removed[0], 0);
assertEq(4,removed[1], 1);
assertEq(5,removed[2], 2);
assertEq(6,removed[3], 3);
assertEq(7,removed[4], 4);
assertEq(8,removed[5], 5);

