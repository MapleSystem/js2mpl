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

var nully = [1, 2, 3, null, 5];
assertEq(14, nully.join(), "1,2,3,,5");
assertEq(15, nully.join(","), "1,2,3,,5");
assertEq(16, nully.join(undefined), "1,2,3,,5");
assertEq(17, nully.join(4), "14243445");

count = 0;
assertEq(18, nully.join(stringifyCounter), "1obj2obj3objobj5");count++;
assertEq(19, count, 1);
