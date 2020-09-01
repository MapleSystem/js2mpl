// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-nonarray-noncallable-replacer.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 653782;
var summary =
  "Treat non-array, non-callable replacers as if none had been specified";

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
var obj = { p: 2 };
var str = '{"p":2}';

assertEq(1, JSON.stringify(obj), str);
assertEq(2, JSON.stringify(obj, ["p"]), str);
assertEq(3, JSON.stringify(obj, null), str);
assertEq(4, JSON.stringify(obj, undefined), str);
assertEq(5, JSON.stringify(obj, 2), str);
assertEq(6, JSON.stringify(obj, true), str);
assertEq(7, JSON.stringify(obj, false), str);
assertEq(8, JSON.stringify(obj, "foopy"), str);
assertEq(9, JSON.stringify(obj, {}), str);
assertEq(10, JSON.stringify(obj, new Boolean(true)), str);
assertEq(11, JSON.stringify(obj, new Number(42)), str);
assertEq(12, JSON.stringify(obj, new String("aequorin")), str);
