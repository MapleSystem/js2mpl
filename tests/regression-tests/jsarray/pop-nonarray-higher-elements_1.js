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

// not-super-much-later element

// non-zero length
function testPop1()
{
  var obj = { length: 2, 3: 42 };
  doTest(obj, 3);
}
for (var i = 0; i < 50; i++)
  testPop1();

// zero length
function testPop2()
{
  var obj = { length: 0, 3: 42 };
  doTest(obj, 3);
}
for (var i = 0; i < 50; i++)
  testPop2();

// much-later (but dense) element

// non-zero length
function testPop3()
{
  var obj = { length: 2, 55: 42 };
  doTest(obj, 55);
}
for (var i = 0; i < 50; i++)
  testPop3();

// zero length
function testPop4()
{
  var obj = { length: 0, 55: 42 };
  doTest(obj, 55);
}
for (var i = 0; i < 50; i++)
  testPop4();

/******************************************************************************/
print("Tests complete");
