/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 858381;
var summary =
  "Array length setting/truncating with non-dense, indexed elements";

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/
function assertEq(actual, expected)
{
  if (actual !== expected){
    $ERROR('Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

function testTruncateDenseAndSparse()
{
  var arr;

  // initialized length 16, capacity same
  arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15];

  // plus a sparse element
  arr[300] = 300;

  // lop off the sparse element and half the dense elements, shrink capacity
  arr.length = 8;

  assertEq(300 in arr, false);
  assertEq(arr[300], undefined);
  assertEq(arr.length, 8);
}
testTruncateDenseAndSparse();

function testTruncateSparse()
{
  // initialized length 8, capacity same
  var arr = [0, 1, 2, 3, 4, 5, 6, 7];

  // plus a sparse element
  arr[300] = 300;

  // lop off the sparse element, leave initialized length/capacity unchanged
  arr.length = 8;

  assertEq(300 in arr, false);
  assertEq(arr[300], undefined);
  assertEq(arr.length, 8);
}
testTruncateSparse();

function testTruncateDenseAndSparseShrinkCapacity()
{
  // initialized length 11, capacity...somewhat larger, likely 16
  var arr = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10];

  // plus a sparse element
  arr[300] = 300;

  // lop off the sparse element, reduce initialized length, reduce capacity
  arr.length = 8;

  assertEq(300 in arr, false);
  assertEq(arr[300], undefined);
  assertEq(arr.length, 8);
}
testTruncateDenseAndSparseShrinkCapacity();

function testTruncateSparseShrinkCapacity()
{
  // initialized length 8, capacity same
  var arr = [0, 1, 2, 3, 4, 5, 6, 7];

  // capacity expands to accommodate, initialized length remains same (not equal
  // to capacity or length)
  arr[15] = 15;

  // now no elements past initialized length
  delete arr[15];

  // ...except a sparse element
  arr[300] = 300;

  // trims sparse element, doesn't change initialized length, shrinks capacity
  arr.length = 8;

  assertEq(300 in arr, false);
  assertEq(arr[300], undefined);
  assertEq(arr.length, 8);
}
testTruncateSparseShrinkCapacity();

/******************************************************************************/
print("Tests complete");
