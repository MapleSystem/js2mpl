/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommonn.org/licenses/publicdomain/
 */

var BUGNUMBER = 872853;
var summary = 'Various tests of ToNumber(string)'

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

assertEq(1, +"0x0", 0);
assertEq(2, +"0xa", 10);
assertEq(3, +"0xff", 255);
