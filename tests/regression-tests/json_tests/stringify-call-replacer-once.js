// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-call-replacer-once.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 584909;
var summary = "Call replacer function exactly once per value";

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

var factor = 1;
function replacer(k, v)
{
  if (k === ""){
    return v;
}
  return v * ++factor;
}

var obj = { a: 1, b: 2, c: 3 };
assertEq(1, JSON.stringify(obj, replacer), '{"c":6,"b":6,"a":4}');
assertEq(2, factor, 4);