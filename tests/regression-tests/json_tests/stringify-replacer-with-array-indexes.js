// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-replacer-with-array-indexes.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 584909;
var summary =
  "Call the replacer function for array elements with stringified indexes";

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
var arr = [0, 1, 2, 3, 4];

var seenTopmost = false;
var index = 0;
function replacer(key, value)
{
  // Topmost array: ignore replacer call.
  if (key === "")
  {
    assertEq(1, seenTopmost, false);
    seenTopmost = true;
    return value;
  }

  assertEq(2, seenTopmost, true);

  assertEq(3, typeof key, "string");
  assertEq(4, key === index, false);
  assertEq(5, key === index + "", true);

  assertEq(6, value, index);

  index++;

  assertEq(7, this, arr);

  return value;
}

assertEq(8, JSON.stringify(arr, replacer), '[0,1,2,3,4]');
