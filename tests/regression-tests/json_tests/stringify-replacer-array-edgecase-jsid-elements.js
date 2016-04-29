// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-replacer-array-edgecase-jsid-elements.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 648471;
var summary =
  "Better/more correct handling for replacer arrays with getter array index " +
  "properties";

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
/* JSID_INT_MIN/MAX copied from jsapi.h. */

var obj =
  {
    /* [JSID_INT_MIN - 1, JSID_INT_MIN + 1] */
    "-1073741825": -1073741825,
    "-1073741824": -1073741824,
    "-1073741823": -1073741823,
    "-1": -1,
    0: 0,
    1: 1,
    /* [JSID_INT_MAX - 1, JSID_INT_MAX + 1] */
    1073741822: 1073741822,
    1073741823: 1073741823,
    1073741824: 1073741824,
  };

for (var s in obj)
{
  var n = obj[s];
  assertEq(1, +s, n);
  assertEq("Failed to stringify numeric property " + n + "correctly", JSON.stringify(obj, [n]),
           '{"' + s + '":' + n + '}');
  assertEq("Failed to stringify string property " + n + "correctly", JSON.stringify(obj, [s]),
           '{"' + s + '":' + n + '}');
  assertEq("Failed to stringify string then number properties ('" + s + "', " + n + ") correctly", JSON.stringify(obj, [s, ]),
           '{"' + s + '":' + n + '}');
  assertEq("Failed to stringify number then string properties (" + n + ", '" + s + "') correctly", JSON.stringify(obj, [n, s]),
           '{"' + s + '":' + n + '}');
}


assertEq("Failed to stringify string property -0 correctly", JSON.stringify({ "-0": 17, 0: 42 }, ["-0"]),
         '{"-0":17}');

