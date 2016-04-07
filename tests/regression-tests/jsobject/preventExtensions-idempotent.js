/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

var gTestfile = 'preventExtensions-idempotent.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 599459;
var summary = 'Object.preventExtensions should be idempotent';

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

var obj = {};
assertEq(1, Object.preventExtensions(obj), obj);
assertEq(2, Object.isExtensible(obj), false);
assertEq(3, Object.preventExtensions(obj), obj);
assertEq(4, Object.isExtensible(obj), false);

print("All tests passed!");
