/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 866700;
var summary = "Assertion redefining non-writable length to a non-numeric value";

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

var arr = [];
Object.defineProperty(arr, "length", { value: 0, writable: false });

// Per Array's magical behavior, the value in the descriptor gets canonicalized
// *before* SameValue comparisons occur, so this shouldn't throw.
Object.defineProperty(arr, "length", { value: '' });

assertEq(arr.length, 0);

/******************************************************************************/
print("Tests complete");
