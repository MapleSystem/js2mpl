/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 600392;
var summary =
  'Object.preventExtensions([]).length = 0 should do nothing, not throw';

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

function testEmpty()
{
  var a = [];
  assertEq(1, a.length, 0);
//  assertEq(2, Object.preventExtensions(a), a);
  assertEq(3, a.length, 0);
  a.length = 0;
  assertEq(4, a.length, 0);
}
testEmpty();

function testEmptyStrict()
{
  "use strict";
  var a = [];
  assertEq(5, a.length, 0);
 // assertEq(6, Object.preventExtensions(a), a);
  assertEq(7, a.length, 0);
  a.length = 0;
  assertEq(8, a.length, 0);
}
testEmptyStrict();

function testNonEmpty()
{
  var a = [1, 2, 3];
  assertEq(9, a.length, 3);
//  assertEq(10, Object.preventExtensions(a), a);
  assertEq(11, a.length, 3);
  a.length = 0;
  assertEq(12, a.length, 0);
}
testNonEmpty();

function testNonEmptyStrict()
{
  "use strict";
  var a = [1, 2, 3];
  assertEq(13, a.length, 3);
//  assertEq(14, Object.preventExtensions(a), a);
  assertEq(15, a.length, 3);
  a.length = 0;
  assertEq(16, a.length, 0);
}
testNonEmptyStrict();

