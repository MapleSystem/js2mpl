/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

var gTestfile = '15.2.3.10-01.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 492849;
var summary = 'ES5: Implement Object.preventExtensions, Object.isExtensible';

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

function trySetProperty(o, p, v)
{
  function setProperty()
  {
    o[p] = v;
  }

  assertEq(1, Object.prototype.hasOwnProperty.call(o, p), false);

  try
  {
    setProperty();
    if (o[p] === v)
      return "set";
    if (p in o)
      return "set-converted";
    return "swallowed";
  }
  catch (e)
  {
    return "throw";
  }
}

assertEq(3, typeof Object.preventExtensions, "function");
assertEq(4, Object.preventExtensions.length, 1);

var slowArray = [1, 2, 3];
slowArray.slow = 5;
var objs =
  [{}, { 1: 2 }, { a: 3 }, [], [1], [, 1], slowArray, function a(){}];

for (var i = 0, sz = objs.length; i < sz; i++)
{
  var o = objs[i];
  assertEq(5 + "object " + i + " not extensible?", Object.isExtensible(o), true);

  var o2 = Object.preventExtensions(o);
  assertEq(6, o, o2);

  assertEq(7 + "object " + i + " is extensible?", Object.isExtensible(o), false);

  assertEq(8 + "unexpected behavior for property-addition to object " + i, trySetProperty(o, "baz", 17), "swallowed");
}

/******************************************************************************/

print("All tests passed!");
