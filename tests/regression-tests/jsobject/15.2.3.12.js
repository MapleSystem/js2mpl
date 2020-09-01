/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

/* Object.isFrozen */
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

assertEq(1, Object.isFrozen({}), false);

assertEq(2, Object.isFrozen(Object.preventExtensions({})), true);

var o = Object.defineProperty({}, 'x', { writable:true, configurable:true });
Object.preventExtensions(o);
assertEq(3, Object.isFrozen(o), false);

var o = Object.defineProperty({}, 'x', { writable:false, configurable:true });
Object.preventExtensions(o);
assertEq(4, Object.isFrozen(o), false);

var o = Object.defineProperty({}, 'x', { writable:true, configurable:false });
Object.preventExtensions(o);
assertEq(5, Object.isFrozen(o), false);

var o = Object.defineProperty({}, 'x', { writable:false, configurable:false });
assertEq(6, Object.isFrozen(o), false);

var o = Object.defineProperty({}, 'x', { writable:false, configurable:false });
Object.preventExtensions(o);
assertEq(7, Object.isFrozen(o), true);

var o = Object.defineProperties({}, { x: { writable:true,  configurable:true },
                                      y: { writable:false, configurable:false } });
Object.preventExtensions(o);
assertEq(8, Object.isFrozen(o), false);

var o = Object.defineProperties({}, { x: { writable:false, configurable:false },
                                      y: { writable:true,  configurable:true } });
Object.preventExtensions(o);
assertEq(9, Object.isFrozen(o), false);

var o = Object.defineProperties({}, { x: { writable:true,  configurable:true },
                                      y: { writable:true,  configurable:true } });
Object.preventExtensions(o);
assertEq(10, Object.isFrozen(o), false);

