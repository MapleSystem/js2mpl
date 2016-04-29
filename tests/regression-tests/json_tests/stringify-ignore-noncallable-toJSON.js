// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-ignore-noncallable-toJSON.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 584909;
var summary = "If the toJSON property isn't callable, don't try to call it";

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/
function assertEq(actual, expected)
{
  if (actual !== expected){
    $ERROR(' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}
var obj =
  {
    p: { toJSON: null },
    m: { toJSON: {} }
  };

assertEq(JSON.stringify(obj), '{"p":{"toJSON":null},"m":{"toJSON":{}}}');
