/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

assertEq(1, undefined, void 0);

assertEq(2, Function.prototype.hasOwnProperty('prototype'), false);
assertEq(3, Function.prototype.prototype, undefined);

var builtin_ctors = [
    Object, Function, Array, String, Boolean, Number
];

for (var i = 0; i < builtin_ctors.length; i++) {
    var c = builtin_ctors[i];
    assertEq(4, typeof c.prototype, (c === Function) ? "function" : "object");
    assertEq(5, c.prototype.constructor, c);
}
