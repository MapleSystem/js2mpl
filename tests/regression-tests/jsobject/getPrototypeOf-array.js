/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

var gTestfile = 'getPrototypeOf-array.js';
var BUGNUMBER = 769041;
var summary =
  "The [[Prototype]] of an object whose prototype chain contains an array " +
  "isn't that array's [[Prototype]]";

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

var arr = [];
assertEq(1, Array.isArray(arr), true);
var objWithArrPrototype = Object.create(arr);
assertEq(2, !Array.isArray(objWithArrPrototype), true);
assertEq(3, Object.getPrototypeOf(objWithArrPrototype), arr);
var objWithArrGrandPrototype = Object.create(objWithArrPrototype);
assertEq(4, !Array.isArray(objWithArrGrandPrototype), true);
assertEq(5, Object.getPrototypeOf(objWithArrGrandPrototype), objWithArrPrototype);

/******************************************************************************/
print("Tests complete");
