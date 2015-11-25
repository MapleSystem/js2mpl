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

var holey = [1, 2, , 4, 5];
assertEq(8, holey.join(), "1,2,,4,5");
assertEq(9, holey.join(","), "1,2,,4,5");
assertEq(10, holey.join(undefined), "1,2,,4,5");
assertEq(11, holey.join(4), "14244445");

count = 0;
assertEq(12, holey.join(stringifyCounter), "1obj2objobj4obj5");
count++;
assertEq(13, count, 1);