// Any copyright is dedicated to the Public Domain.
// http://creativecommons.org/licenses/publicdomain/

//-----------------------------------------------------------------------------
var BUGNUMBER = 430133;
var summary = 'ES5 Object.defineProperty(O, P, Attributes)';

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

var o = [];
Object.defineProperty(o, 0, { value: 17 });
var desc = Object.getOwnPropertyDescriptor(o, 0);
assertEq(1, desc !== undefined, true);
assertEq(2, desc.value, 17);
assertEq(3, desc.enumerable, false);
assertEq(4, desc.configurable, false);
assertEq(5, desc.writable, false);

desc = Object.getOwnPropertyDescriptor(o, "length");
assertEq(6, desc !== undefined, true);
assertEq(7, desc.enumerable, false);
assertEq(8, desc.configurable, false);
assertEq(9, desc.writable, true);
assertEq(10, desc.value, 1);
assertEq(11, o.length, 1);

Object.defineProperty(o, "foobar",
                      { value: 42, enumerable: false, configurable: true });
assertEq(12, o.foobar, 42);
desc = Object.getOwnPropertyDescriptor(o, "foobar");
assertEq(13, desc !== undefined, true);
assertEq(14, desc.value, 42);
assertEq(15, desc.configurable, true);
assertEq(16, desc.enumerable, false);
assertEq(17, desc.writable, false);

var called = false;
o = { set x(a) { called = true; } };
Object.defineProperty(o, "x", { get: function() { return "get"; } });
desc = Object.getOwnPropertyDescriptor(o, "x");
assertEq(18, "set" in desc, true);
assertEq(19, "get" in desc, true);
assertEq(20, called, false);
o.x = 13;
assertEq(21, called, true);

/*
var toSource = Object.prototype.toSource || function() { };
toSource.call(o); // a test for this not crashing*/

var called = false;
var o =
  Object.defineProperty({}, "foo",
                        { get: function() { return 17; },
                          set: function(v) { called = true; } });

assertEq(22, o.foo, 17);
o.foo = 42;
assertEq(23, called, true);

/*
 * XXX need tests for Object.defineProperty(array, "length", { ... }) when we
 * support it!
 */

/******************************************************************************/
print("All tests passed!");
