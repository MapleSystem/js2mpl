/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */
//-----------------------------------------------------------------------------
var BUGNUMBER = 492849;
var summary = 'Object.is{Sealed,Frozen}, Object.{seal,freeze}';

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

// Empty object

var o1 = {};

assertEq(1, Object.isExtensible(o1), true);
assertEq(2, Object.isSealed(o1), false);
assertEq(3, Object.isFrozen(o1), false);

Object.preventExtensions(o1);

// An non-extensible empty object has no properties, so it is vacuously sealed
// and frozen.
assertEq(4, Object.isExtensible(o1), false);
assertEq(5, Object.isSealed(o1), true);
assertEq(6, Object.isFrozen(o1), true);


// Object with a data property

var o2 = { 1: 2 };

assertEq(7, Object.isExtensible(o2), true);
assertEq(8, Object.isSealed(o2), false);
assertEq(9, Object.isFrozen(o2), false);

Object.preventExtensions(o2);

assertEq(10, Object.isExtensible(o2), false);
assertEq(11, Object.isSealed(o2), false);
assertEq(12, Object.isFrozen(o2), false);

Object.seal(o2);

assertEq(13, Object.isExtensible(o2), false);
assertEq(14, Object.isSealed(o2), true);
assertEq(15, Object.isFrozen(o2), false);

assertEq(16, o2[1], 2);

var desc;

desc = Object.getOwnPropertyDescriptor(o2, "1");
assertEq(17, typeof desc, "object");
assertEq(18, desc.enumerable, true);
assertEq(19, desc.configurable, false);
assertEq(20, desc.value, 2);
assertEq(21, desc.writable, true);

o2[1] = 17;

assertEq(22, o2[1], 17);

desc = Object.getOwnPropertyDescriptor(o2, "1");
assertEq(23, typeof desc, "object");
assertEq(24, desc.enumerable, true);
assertEq(25, desc.configurable, false);
assertEq(26, desc.value, 17);
assertEq(27, desc.writable, true);

Object.freeze(o2);

assertEq(28, o2[1], 17);

desc = Object.getOwnPropertyDescriptor(o2, "1");
assertEq(29, typeof desc, "object");
assertEq(30, desc.enumerable, true);
assertEq(31, desc.configurable, false);
assertEq(32, desc.value, 17);
assertEq(33, desc.writable, false);


// Object with an accessor property

var o3 = { get foo() { return 17; } };

assertEq(34, Object.isExtensible(o3), true);
assertEq(35, Object.isSealed(o3), false);
assertEq(36, Object.isFrozen(o3), false);

Object.preventExtensions(o3);

assertEq(37, Object.isExtensible(o3), false);
assertEq(38, Object.isSealed(o3), false);
assertEq(39, Object.isFrozen(o3), false);

Object.seal(o3);

// An accessor property in a sealed object is unchanged if that object is
// frozen, so a sealed object containing only accessor properties is also
// vacuously frozen.
assertEq(40, Object.isExtensible(o3), false);
assertEq(41, Object.isSealed(o3), true);
assertEq(42, Object.isFrozen(o3), true);

Object.freeze(o3);

assertEq(43, Object.isExtensible(o3), false);
assertEq(44, Object.isSealed(o3), true);
assertEq(45, Object.isFrozen(o3), true);


/******************************************************************************/
print("All tests passed!");
