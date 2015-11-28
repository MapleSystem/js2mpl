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

var holey = [1, 2, , 4, 5];
assertEq(8, holey.join(), "1,2,,4,5");
assertEq(9, holey.join(","), "1,2,,4,5");
assertEq(10, holey.join(undefined), "1,2,,4,5");
assertEq(11, holey.join(4), "14244445");

count = 0;
assertEq(12, holey.join(stringifyCounter), "1obj2objobj4obj5");
count++;
assertEq(13, count, 1);

var nully = [1, 2, 3, null, 5];
assertEq(14, nully.join(), "1,2,3,,5");
assertEq(15, nully.join(","), "1,2,3,,5");
assertEq(16, nully.join(undefined), "1,2,3,,5");
assertEq(17, nully.join(4), "14243445");

count = 0;
assertEq(18, nully.join(stringifyCounter), "1obj2obj3objobj5");count++;
assertEq(19, count, 1);

var undefiney = [1, undefined, 3, 4, 5];
assertEq(20, undefiney.join(), "1,,3,4,5");
assertEq(21, undefiney.join(","), "1,,3,4,5");
assertEq(22, undefiney.join(undefined), "1,,3,4,5");
assertEq(23, undefiney.join(4), "14434445");

count = 0;
assertEq(24, undefiney.join(stringifyCounter), "1objobj3obj4obj5");count++;
assertEq(25, count, 1);

