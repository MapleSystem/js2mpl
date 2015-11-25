/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
print("ES5: Array.prototype.join");

/**************
 * BEGIN TEST *
 **************/
var count;
var stringifyCounter = "obj";

function assertEq(message, actual, expected)
{
  if (actual !== expected){
    $ERROR('CHECK #'+ message +': ' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

var arr = [1, 2, 3, 4, 5];
assertEq(1, arr.join(), "1,2,3,4,5");
assertEq(2, arr.join(","), "1,2,3,4,5");
assertEq(3, arr.join(undefined), "1,2,3,4,5");
assertEq(4, arr.join(4), "142434445");
assertEq(5, arr.join(""), "12345");

count = 0;
assertEq(6, arr.join(stringifyCounter), "1obj2obj3obj4obj5");
count++;
assertEq(7, count, 1);