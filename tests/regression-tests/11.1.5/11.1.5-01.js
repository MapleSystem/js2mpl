// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

//-----------------------------------------------------------------------------
var BUGNUMBER = 520696;
var summary =
  'Implement support for string literals as names for properties defined ' +
  'using ES5 get/set syntax';

print(BUGNUMBER + ": " + summary);

function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}
var o;

o = { get "a b c"() { return 17; } };
assertEq(1, "get" in Object.getOwnPropertyDescriptor(o, "a b c"), true);

var f = function literalInside() { return { set 'c d e'(q) { } }; };

function checkO()
{
  assertEq("2 exponential-named property is in object", 10000 in o, true);
}

o = { 1e4: 17 };
checkO();
