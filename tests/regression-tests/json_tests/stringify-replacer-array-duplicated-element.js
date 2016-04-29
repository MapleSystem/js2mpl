// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-replacer-array-hijinks.js';
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
assertEq(1, JSON.stringify({ 1: 1 }, [1, 1]), '{"1":1}');

assertEq(2, JSON.stringify({ 1: 1 }, [1, "1"]), '{"1":1}');

assertEq(3, JSON.stringify({ 1: 1 }, ["1", 1]), '{"1":1}');

assertEq(4, JSON.stringify({ 1: 1 }, ["1", "1"]), '{"1":1}');

assertEq(5, JSON.stringify({ 1: 1 }, [1, new String(1)]), '{"1":1}');

assertEq(6, JSON.stringify({ 1: 1 }, [1, new Number(1)]), '{"1":1}');

assertEq(7, JSON.stringify({ 1: 1 }, ["1", new Number(1)]), '{"1":1}');

assertEq(8, JSON.stringify({ 1: 1 }, ["1", new String(1)]), '{"1":1}');

assertEq(9, JSON.stringify({ 1: 1 }, [new String(1), new String(1)]), '{"1":1}');

assertEq(10, JSON.stringify({ 1: 1 }, [new String(1), new Number(1)]), '{"1":1}');

assertEq(11, JSON.stringify({ 1: 1 }, [new Number(1), new String(1)]), '{"1":1}');

assertEq(12, JSON.stringify({ 1: 1 }, [new Number(1), new Number(1)]), '{"1":1}');