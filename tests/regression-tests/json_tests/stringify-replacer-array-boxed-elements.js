// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

var gTestfile = 'stringify-replacer-array-boxed-elements.js';
//-----------------------------------------------------------------------------
var BUGNUMBER = 648471;
var summary = "Boxed-string/number objects in replacer arrays";

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
var S = new String(3);
var N = new Number(4);

assertEq(1, JSON.stringify({ 3: 3, 4: 4 }, [S]),
         '{"3":3}');
assertEq(2, JSON.stringify({ 3: 3, 4: 4 }, [N]),
         '{"4":4}');

Number.prototype.toString = function() { return 3; };
assertEq(3, JSON.stringify({ 3: 3, 4: 4 }, [N]),
         '{"3":3}');

String.prototype.toString = function() { return 4; };
assertEq(4, JSON.stringify({ 3: 3, 4: 4 }, [S]),
         '{"4":4}');

Number.prototype.toString = function() { return new String(42); };
assertEq(5, JSON.stringify({ 3: 3, 4: 4 }, [N]),
         '{"4":4}');

String.prototype.toString = function() { return new Number(17); };
assertEq(6, JSON.stringify({ 3: 3, 4: 4 }, [S]),
         '{"3":3}');

Number.prototype.toString = null;
assertEq(7, JSON.stringify({ 3: 3, 4: 4 }, [N]),
         '{"4":4}');

String.prototype.toString = null;
assertEq(8, JSON.stringify({ 3: 3, 4: 4 }, [S]),
         '{"3":3}');

Number.prototype.valueOf = function() { return 17; };
assertEq(9, JSON.stringify({ 4: 4, 17: 17 }, [N]),
         '{"17":17}');

String.prototype.valueOf = function() { return 42; };
assertEq(10, JSON.stringify({ 3: 3, 42: 42 }, [S]),
         '{"42":42}');
