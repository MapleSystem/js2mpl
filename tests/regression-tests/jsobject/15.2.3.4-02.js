/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 518663;
var summary = 'Object.getOwnPropertyNames: array objects';

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}
var a, names, expected;

a = [0, 1, 2];
names = Object.getOwnPropertyNames(a).sort();
expected = ["0", "1", "2", "length"].sort();
assertEq(1, (names.length === expected.length &&
         names.every(function(v, i) { return v === expected[i]; })), true);

a = [1, , , 7];
a.p = 2;
Object.defineProperty(a, "q", { value: 42, enumerable: false });
names = Object.getOwnPropertyNames(a).sort();
expected = ["0", "3", "p", "q", "length"].sort();
assertEq(2, (names.length === expected.length &&
         names.every(function(v, i) { return v === expected[i]; })), true);


a = [];
names = Object.getOwnPropertyNames(a).sort();
expected = ["length"];
assertEq(3, (names.length === expected.length &&
         names.every(function(v, i) { return v === expected[i]; })), true);

/******************************************************************************/

print("All tests passed!");
