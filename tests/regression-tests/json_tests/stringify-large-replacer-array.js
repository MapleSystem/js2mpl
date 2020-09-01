// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-large-replacer-array.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 816033;
var summary = "JSON.stringify with a large replacer array";

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

var replacer = [];
for (var i = 0; i < 100; i++)
  replacer.push(i);

assertEq(JSON.stringify({ "foopy": "FAIL", "93": 17 }, replacer), '{"93":17}');
