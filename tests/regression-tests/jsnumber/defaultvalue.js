/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommonn.org/licenses/publicdomain/
 */

var BUGNUMBER = 645464;
var summary =
  "[[DefaultValue]] behavior wrong for Number with overridden valueOf/toString";

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

// equality
/*
var n = new Number();
assertEq(1, n == 0, true);

var n2 = new Number();
n2.valueOf = function() { return 17; };
assertEq(2, n2 == 17, true);

var n3 = new Number();
n3.toString = function() { return 42; };
assertEq(3, n3 == 0, true);

function testEquality()
{
  var n = new Number();
  assertEq(4, n == 0, true);

  var n2 = new Number();
  n2.valueOf = function() { return 17; };
  assertEq(5, n2 == 17, true);

  var n3 = new Number();
  n3.toString = function() { return 42; };
  assertEq(6, n3 == 0, true);
}
testEquality();


// addition of Number to number

var n = new Number();
assertEq(7, n + 5, 5);

var n2 = new Number();
n2.toString = function() { return 9; };
assertEq(8, n2 + 3, 3);

var n3 = new Number();
n3.valueOf = function() { return 17; };
assertEq(9, n3 + 5, 22);

function testNumberAddition()
{
  var n = new Number();
  assertEq(10, n + 5, 5);

  var n2 = new Number();
  n2.toString = function() { return 9; };
  assertEq(11, n2 + 3, 3);

  var n3 = new Number();
  n3.valueOf = function() { return 17; };
  assertEq(12, n3 + 5, 22);
}
testNumberAddition();


// addition of Number to Number

var n = new Number();
assertEq(13, n + n, 0);

var n2 = new Number();
n2.toString = function() { return 5; };
assertEq(14, n2 + n2, 0);

var n3 = new Number();
n3.valueOf = function() { return 8; };
assertEq(15, n3 + n3, 16);

function testNonNumberAddition()
{
  var n = new Number();
  assertEq(16, n + n, 0);

  var n2 = new Number();
  n2.toString = function() { return 5; };
  assertEq(17, n2 + n2, 0);

  var n3 = new Number();
  n3.valueOf = function() { return 8; };
  assertEq(18, n3 + n3, 16);
}
testNonNumberAddition();
*/

// Number as bracketed property name

var obj = { 0: 17, 8: 42, 9: 8675309 };

var n = new Number();
assertEq(19, obj[n], 17);

var n2 = new Number();
print(n2)
n2.valueOf = function() { return 8; }
print(n2)
assertEq(20, obj[n2], 17);

var n3 = new Number();
n3.toString = function() { return 9; };
assertEq(21, obj[n3], 8675309);

function testPropertyNameToNumber()
{
  var obj = { 0: 17, 8: 42, 9: 8675309 };

  var n = new Number();
  assertEq(22, obj[n], 17);

  var n2 = new Number();
  n2.valueOf = function() { return 8; }
  assertEq(23, obj[n2], 17);

  var n3 = new Number();
  n3.toString = function() { return 9; };
  assertEq(24, obj[n3], 8675309);
}
testPropertyNameToNumber();


// Number as property name with |in| operator

var n = new Number();
assertEq(25, n in { 0: 5 }, true);

var n2 = new Number();
n2.toString = function() { return "baz"; };
assertEq(26, n2 in { baz: 42 }, true);

var n3 = new Number();
n3.valueOf = function() { return "quux"; };
assertEq(27, n3 in { 0: 17 }, true);

function testInOperatorName()
{
  var n = new Number();
  assertEq(28, n in { 0: 5 }, true);

  var n2 = new Number();
  n2.toString = function() { return "baz"; };
  assertEq(29, n2 in { baz: 42 }, true);

  var n3 = new Number();
  n3.valueOf = function() { return "quux"; };
  assertEq(30, n3 in { 0: 17 }, true);
}
testInOperatorName();

/******************************************************************************/

print("All tests passed!");
