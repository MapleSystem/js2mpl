// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-toJSON-arguments.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 584909;
var summary = "Arguments when an object's toJSON method is called";

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
    p: {
         toJSON: function(key)
         {
           assertEq(key, "p");
           return 17;
         }
       }
  };

assertEq(JSON.stringify(obj), '{"p":17}');
