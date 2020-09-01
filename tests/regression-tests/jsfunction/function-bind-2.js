/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

var gTestfile = 'function-bind.js';
var BUGNUMBER = 429507;
var summary = "ES5: Function.prototype.bind";

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



// ad-hoc testing
assertEq(1, Function.prototype.hasOwnProperty("bind"), true);

var bind = Function.prototype.bind;
assertEq(2, bind.length, 1);

/*
 * 3. Let A be a new (possibly empty) internal list of all of the argument
 *    values provided after thisArg (arg1, arg2 etc), in order.
 * 4. Let F be a new native ECMAScript object .
 * 5. Set all the internal methods, except for [[Get]], of F as specified in
 *    8.12.
 * 6. Set the [[Get]] internal property of F as specified in 15.3.5.4.
 * 7. Set the [[TargetFunction]] internal property of F to Target.
 * 8. Set the [[BoundThis]] internal property of F to the value of thisArg.
 * 9. Set the [[BoundArgs]] internal property of F to A.
 */
// throughout


/* 10. Set the [[Class]] internal property of F to "Function". */
var toString = Object.prototype.toString;
assertEq(3, toString.call(function(){}), "[object Function]");
assertEq(4, toString.call(function a1(){}), "[object Function]");
assertEq(5, toString.call(function(a){}), "[object Function]");
assertEq(6, toString.call(function a2(b){}), "[object Function]");
assertEq(7, toString.call(function(){}.bind()), "[object Function]");
assertEq(8, toString.call(function a3(){}.bind()), "[object Function]");
assertEq(9, toString.call(function(a){}.bind()), "[object Function]");
assertEq(10, toString.call(function a4(b){}.bind()), "[object Function]");


/*
 * 11. Set the [[Prototype]] internal property of F to the standard built-in
 *     Function prototype object as specified in 15.3.3.1.
 */
assertEq(11, Object.getPrototypeOf(bind.call(function(){})), Function.prototype);
assertEq(12, Object.getPrototypeOf(bind.call(function a5(){})), Function.prototype);
assertEq(13, Object.getPrototypeOf(bind.call(function(a){})), Function.prototype);
assertEq(14, Object.getPrototypeOf(bind.call(function a6(b){})), Function.prototype);

/*
 * 12. Set the [[Call]] internal property of F as described in 15.3.4.5.1.
 */
var a = Array.bind(1, 2);
assertEq(15, a().length, 2);
assertEq(16, a(4).length, 2);
assertEq(17, a(4, 8).length, 3);

function t() { return this; }
var bt = t.bind(t);
assertEq(18, bt(), t);
