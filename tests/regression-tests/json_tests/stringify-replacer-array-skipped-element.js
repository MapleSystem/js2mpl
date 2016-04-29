// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-replacer-array-skipped-element.js';
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

assertEq(1, JSON.stringify({ 3: 3, 4: 4 },
                        ["3", { toString: function() { return "4" } }]),
         '{"3":3}');

assertEq(2, JSON.stringify({ 3: 3, true: 4 }, ["3", true]),
         '{"3":3}');

assertEq(3, JSON.stringify({ 3: 3, true: 4 }, ["3", "true", true]),
         '{"3":3,"true":4}');

assertEq(4, JSON.stringify({ 3: 3, true: 4 }, ["3", true, "true"]),
         '{"3":3,"true":4}');

assertEq(5, JSON.stringify({ 3: 3, false: 4 }, ["3", false]),
         '{"3":3}');

assertEq(6, JSON.stringify({ 3: 3, false: 4 }, ["3", "false", false]),
         '{"3":3,"false":4}');

assertEq(7, JSON.stringify({ 3: 3, false: 4 }, ["3", false, "false"]),
         '{"3":3,"false":4}');

assertEq(8, JSON.stringify({ 3: 3, undefined: 4 }, ["3", undefined]),
         '{"3":3}');

assertEq(9, JSON.stringify({ 3: 3, undefined: 4 }, ["3", "undefined", undefined]),
         '{"3":3,"undefined":4}');

assertEq(10, JSON.stringify({ 3: 3, undefined: 4 }, ["3", undefined, "undefined"]),
         '{"3":3,"undefined":4}');

assertEq(11, JSON.stringify({ 3: 3, null: 4 }, ["3", null]),
         '{"3":3}');

assertEq(12, JSON.stringify({ 3: 3, null: 4 }, ["3", "null", null]),
         '{"3":3,"null":4}');

assertEq(13, JSON.stringify({ 3: 3, null: 4 }, ["3", null, "null"]),
         '{"3":3,"null":4}');
