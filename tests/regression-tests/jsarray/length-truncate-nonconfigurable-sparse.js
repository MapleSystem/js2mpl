/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 858381;
var summary =
  "Array length redefinition behavior with non-configurable elements";

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

function addDataProperty(obj, prop, value, enumerable, configurable, writable)
{
  var desc =
    { enumerable: enumerable,
      configurable: configurable,
      writable: writable,
      value: value };
  Object.defineProperty(obj, prop, desc);
}

function nonstrict()
{
  var arr = [0, , 2, , , 5];

  addDataProperty(arr,  350, "foo", true,  true,  true);
  addDataProperty(arr, 400, "bar", true,  true,  false);
  addDataProperty(arr,   200, "qux", false, true,  true);
  addDataProperty(arr,   250, "eit", false, true,  false);
  addDataProperty(arr, 500, "fun", false, true,  false);

  // non-array indexes to spice things up
  addDataProperty(arr, "foopy", "sdfsd", false, false, false);
  addDataProperty(arr, "hello", "psych", true, false, false);
  addDataProperty(arr, "world", "psych", true, false, false);

  addDataProperty(arr,  300, "eep", false, false, false);

  // Truncate...but only as far as possible.
  arr.length = 1;

  assertEq(arr.length, 301);

  var props = Object.getOwnPropertyNames(arr).sort();
  var expected =
    ["0", "2", "5", "200", "250", "300",
     "foopy", "hello", "world", "length"].sort();

  assertEq(props.length, expected.length);
  for (var i = 0; i < props.length; i++)
    assertEq(props[i], expected[i]);
}
nonstrict();
/*
function strict()
{
  "use strict";

  var arr = [0, , 2, , , 5];

  addDataProperty(arr,  350, "foo", true,  true,  true);
  addDataProperty(arr, 400, "bar", true,  true,  false);
  addDataProperty(arr,   200, "qux", false, true,  true);
  addDataProperty(arr,   250, "eit", false, true,  false);
  addDataProperty(arr, 500, "fun", false, true,  false);

  // non-array indexes to spice things up
  addDataProperty(arr, "foopy", "sdfsd", false, false, false);
  addDataProperty(arr, "hello", "psych", true, false, false);
  addDataProperty(arr, "world", "psych", true, false, false);

  addDataProperty(arr,  300, "eep", false, false, false);

  try
  {
    arr.length = 1;
    throw new Error("didn't throw?!");
  }
  catch (e)
  {
    assertEq(e instanceof TypeError, true,
             "non-configurable property should trigger TypeError, got " + e);
  }

  assertEq(arr.length, 301);

  var props = Object.getOwnPropertyNames(arr).sort();
  var expected =
    ["0", "2", "5", "200", "250", "300",
     "foopy", "hello", "world", "length"].sort();

  assertEq(props.length, expected.length);
  for (var i = 0; i < props.length; i++)
    assertEq(props[i], expected[i], "unexpected property: " + props[i]);
}
strict();*/
/******************************************************************************/

print("Tests complete");
