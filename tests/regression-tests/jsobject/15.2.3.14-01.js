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

function arraysEqual(a1, a2)
{
  return a1.length === a2.length &&
         a1.every(function(v, i) { return v === a2[i]; });
}

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
keys = Object.keys(o).sort();
assertEq(2, arraysEqual(keys, ["a", "b"].sort()), true,
         " " + keys);

o = { get a() { return 17; }, b: 2 };
keys = Object.keys(o).sort(),
assertEq(3, arraysEqual(keys, ["a", "b"].sort()), true,
         " " + keys);

o = { __iterator__: function() { return Iterator({a: 2, b: 3}); } };
keys = Object.keys(o);
assertEq(4, arraysEqual(keys, ["__iterator__"]), true,
         " " + keys);

o = { a: 1, b: 2 };
delete o.a;
o.a = 3;
keys = Object.keys(o).sort();
assertEq(5, arraysEqual(keys, ["b", "a"].sort()), true,
         " " + keys);

o = [0, 1, 2];
keys = Object.keys(o).sort();
assertEq(6, arraysEqual(keys, ["0", "1", "2"].sort()), true,
         " " + keys);

o = { a: 1, b: 2, c: 3 };
delete o.b;
o.b = 5;
keys = Object.keys(o).sort();
assertEq(8, arraysEqual(keys, ["a", "c", "b"].sort()), true,
         " " + keys);

function f() { }
f.prototype.p = 1;
o = new f();
o.g = 1;
keys = Object.keys(o);
assertEq(9, arraysEqual(keys, ["g"]), true,
         " " + keys);
/******************************************************************************/

print("All tests passed!");
