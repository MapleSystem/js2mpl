/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

var BUGNUMBER = 677820;
var summary =
  "String.prototype.match must define matches on the returned array, not set " +
  "them";

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

var called = false;
function setterFunction(v) { called = true; }
function getterFunction(v) { return "getter"; }

Object.defineProperty(Array.prototype, 1,
  { get: getterFunction, set: setterFunction });

assertEq(1, called, false);
var desc = Object.getOwnPropertyDescriptor(Array.prototype, 1);
assertEq(2, desc.get, getterFunction);
assertEq(3, desc.set, setterFunction);
assertEq(4, desc.enumerable, false);
assertEq(5, desc.configurable, false);
assertEq(6, [][1], "getter");

assertEq(7, called, false);

print("Tests complete");
