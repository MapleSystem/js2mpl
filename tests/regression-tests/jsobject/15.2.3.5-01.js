/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 492840;
var summary = 'ES5 Object.create(O [, Properties])';

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
assertEq(1, "create" in Object, true);
assertEq(2, Object.create.length, 2);

var o, desc, props, proto;

o = Object.create(null);
assertEq(3 + "bad null-proto", Object.getPrototypeOf(o), null);

o = Object.create(null, { a: { value: 17, enumerable: false } });
assertEq(4 + "bad null-proto", Object.getPrototypeOf(o), null);
assertEq(5, "a" in o, true);
desc = Object.getOwnPropertyDescriptor(o, "a");
assertEq(6 + "no descriptor?", desc !== undefined, true);
assertEq(7, desc.value, 17);
assertEq(8, desc.enumerable, false);
assertEq(9, desc.configurable, false);
assertEq(10, desc.writable, false);

props = Object.create({ bar: 15 });
Object.defineProperty(props, "foo", { enumerable: false, value: 42 });
proto = { baz: 12 };
o = Object.create(proto, props);
assertEq(11, Object.getPrototypeOf(o), proto);
assertEq(12, Object.getOwnPropertyDescriptor(o, "foo"), undefined);
assertEq(13, "foo" in o, false);
assertEq(14, Object.getOwnPropertyDescriptor(o, "bar"), undefined);
assertEq(15, "bar" in o, false);
assertEq(16, Object.getOwnPropertyDescriptor(o, "baz"), undefined);
assertEq(17, o.baz, 12);
assertEq(18, o.hasOwnProperty("baz"), false);

try {
  var actual =
    Object.create(Object.create({},
                                { boom: { get: function() { return "base"; }}}),
                  { boom: { get: function() { return "overridden"; }}}).boom
} catch (e) {
}
assertEq(19, actual, "overridden");

/******************************************************************************/
print("All tests passed!");