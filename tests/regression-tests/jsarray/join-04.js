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

var undefiney = [1, undefined, 3, 4, 5];
assertEq(20, undefiney.join(), "1,,3,4,5");
assertEq(21, undefiney.join(","), "1,,3,4,5");
assertEq(22, undefiney.join(undefined), "1,,3,4,5");
assertEq(23, undefiney.join(4), "14434445");

count = 0;
assertEq(24, undefiney.join(stringifyCounter), "1objobj3obj4obj5");count++;
assertEq(25, count, 1);