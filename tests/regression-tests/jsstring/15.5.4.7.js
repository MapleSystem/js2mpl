/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

var BUGNUMBER = 612838;
var summary = "String.prototype.indexOf with empty searchString";

print(BUGNUMBER + ": " + summary);
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

assertEq(1, "123".indexOf("", -1), 0);
assertEq(2, "123".indexOf("", 0), 0);
assertEq(3, "123".indexOf("", 1), 1);
assertEq(4, "123".indexOf("", 3), 3);
assertEq(5, "123".indexOf("", 4), 3);
assertEq(6, "123".indexOf("", 12345), 3);

print("All tests passed!");
