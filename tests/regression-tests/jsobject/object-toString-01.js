/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

var gTestfile = 'object-toString-01.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 575522;
var summary = '({}).toString.call(null) == "[object Null]", ' +
              '({}).toString.call(undefined) == "[object Undefined]", ';

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

var toString = Object.prototype.toString;

assertEq(1, toString.call(null), "[object Null]");
assertEq(2, toString.call(undefined), "[object Undefined]");

assertEq(3, toString.call(true), "[object Boolean]");
assertEq(4, toString.call(false), "[object Boolean]");

assertEq(5, toString.call(0), "[object Number]");
assertEq(7, toString.call(1), "[object Number]");
assertEq(8, toString.call(-1), "[object Number]");

assertEq(12, toString.call("foopy"), "[object String]");

assertEq(13, toString.call({}), "[object Object]");

/******************************************************************************/

print("All tests passed!");
