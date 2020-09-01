// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'parse-mega-huge-array.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 667527;
var summary = "JSON.parse should parse arrays of essentially unlimited size";

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

var str = '[';
for (var i = 0, sz = Math.pow(2, 7); i < sz; i++)
  str += '0,';
str += '0]';

var arr = JSON.parse(str);
assertEq(1, arr.length, Math.pow(2, 7) + 1);
