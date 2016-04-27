/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 307791;
var summary = 'ES5 Object.keys(O)';
var actual = '';
var expect = '';

print("BUGNUMBER:" + BUGNUMBER);
print(summary);

/**************
 * BEGIN TEST *
 **************/
function assertEq(message1, actual, expected, message2)
{
  if (actual !== expected){
    $ERROR('#' + message1 + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual + message2);
  }
}
assertEq(1, Object.keys.length, 1);

var o, keys;

o = { a: 3, b: 2 };
keys1 = Object.keys(o).sort();
keys2 = ["a", "b"].sort()
assertEq(2, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);

o = { get a() { return 17; }, b: 2 };
keys1 = Object.keys(o).sort();
keys2 = ["a", "b"].sort();
assertEq(3, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);

o = { __iterator__: function() { return Iterator({a: 2, b: 3}); } };
keys1 = Object.keys(o);
keys2 = ["__iterator__"];
assertEq(4, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);

o = { a: 1, b: 2 };
delete o.a;
o.a = 3;
keys1 = Object.keys(o).sort();
keys2 = ["b", "a"].sort()
assertEq(5, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);

o = [0, 1, 2];
keys1 = Object.keys(o).sort();
keys2 = ["0", "1", "2"].sort();
assertEq(6, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);

o = { a: 1, b: 2, c: 3 };
delete o.b;
o.b = 5;
keys1 = Object.keys(o).sort();
keys2 = ["a", "c", "b"].sort();
assertEq(8, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);

function f() { }
f.prototype.p = 1;
o = new f();
o.g = 1;
keys1 = Object.keys(o);
keys2 = ["g"]
assertEq(9, (keys1.length === keys2.length &&
         keys1.every(function(v, i) { return v === keys2[i]; })), true,
         " " + keys);
/******************************************************************************/

print("All tests passed!");
