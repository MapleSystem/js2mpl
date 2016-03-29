/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 909602;
var summary =
  "Array.prototype.pop shouldn't touch elements greater than length on " +
  "non-arrays";

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

function doTest(obj, index)
{
  // print("testing " + JSON.stringify(obj) + " with index " + index);
  assertEq(Array.prototype.pop.call(obj), undefined);
  assertEq(index in obj, true);
  assertEq(obj[index], 42);
}

// much much much later (sparse) element

// non-zero length
function testPop5()
{
  var obj = { length: 2, 655: 42 };
  doTest(obj, 655);
}
for (var i = 0; i < 50; i++)
  testPop5();

// zero length
function testPop6()
{
  var obj = { length: 0, 655: 42 };
  doTest(obj, 655);
}
for (var i = 0; i < 50; i++)
  testPop6();
/******************************************************************************/
print("Tests complete");
