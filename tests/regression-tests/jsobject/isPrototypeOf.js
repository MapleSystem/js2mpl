/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

var gTestfile = 'isPrototypeOf.js';
var BUGNUMBER = 619283;
var summary = "Object.prototype.isPrototypeOf";

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

var isPrototypeOf = Object.prototype.isPrototypeOf;

/*
 * 1. If V is not an Object, return false.
 */
assertEq(2, isPrototypeOf(), false);
assertEq(3, isPrototypeOf(1), false);
assertEq(4, isPrototypeOf(Number.MAX_VALUE), false);
assertEq(6, isPrototypeOf(""), false);
assertEq(7, isPrototypeOf("sesquicentennial"), false);
assertEq(8, isPrototypeOf(true), false);
assertEq(9, isPrototypeOf(false), false);
assertEq(11, isPrototypeOf(undefined), false);
assertEq(12, isPrototypeOf(null), false);

/*
 * 3. Repeat
 */

/*
 * 3a. Let V be the value of the [[Prototype]] internal property of V.
 * 3b. If V is null, return false.
 */
var protoGlobal = Object.create(this);
assertEq(13, Object.prototype.isPrototypeOf(Object.prototype), false);
assertEq(14, String.prototype.isPrototypeOf({}), false);
assertEq(15, Object.prototype.isPrototypeOf(Object.create(null)), false);

/* 3c. If O and V refer to the same object, return true. */
assertEq(16, Object.prototype.isPrototypeOf({}), true);
assertEq(17, this.isPrototypeOf(protoGlobal), true);
assertEq(18, Object.prototype.isPrototypeOf({}), true);
assertEq(19, Object.prototype.isPrototypeOf(new Number(17)), true);
assertEq(20, Object.prototype.isPrototypeOf(function(){}), true);

/******************************************************************************/

print("All tests passed!");
