/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 664528;
var summary =
  "Sorting an array containing only holes and |undefined| should move all " +
  "|undefined| to the start of the array";

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/
function assertEq(actual, expected)
{
  if (actual !== expected){
    $ERROR('Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

var a = [, , , undefined];
a.sort();

assertEq(a.hasOwnProperty(0), true);
assertEq(a[0], undefined);
assertEq(a.hasOwnProperty(1), false);
assertEq(a.hasOwnProperty(2), false);
assertEq(a.hasOwnProperty(3), false);

/******************************************************************************/
print("Tests complete");
