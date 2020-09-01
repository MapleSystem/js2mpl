/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 518663;
var summary =
  'Object.getOwnPropertyNames should play nicely with enumerator caching';

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

for (var p in JSON);
var names = Object.getOwnPropertyNames(JSON);
assertEq(1, names.length >= 2, true,
         "wrong number of property names?  [" + names + "]");
assertEq(2, names.indexOf("parse") >= 0, true);
assertEq(3, names.indexOf("stringify") >= 0, true);

/******************************************************************************/

print("All tests passed!");
