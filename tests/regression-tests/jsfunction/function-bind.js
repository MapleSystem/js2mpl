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

/*
 * 13. Set the [[Construct]] internal property of F as described in 15.3.4.5.2.
 */
 
function Point(x, y)
{
  this.x = x;
  this.y = y;
}
var YAxisPoint = Point.bind(null, 0)
assertEq(19, YAxisPoint.hasOwnProperty("prototype"), false);
var p = new YAxisPoint(5);
assertEq(20, p.x, 0);
assertEq(21, p.y, 5);
/*assertEq(22, YAxisPoint instanceof Point, false);
assertEq(23, p instanceof Point, true);
assertEq(24, p instanceof YAxisPoint, true);*/
assertEq(25, Object.prototype.toString.call(YAxisPoint), "[object Function]");
assertEq(26, YAxisPoint.length, 1);

/*
 * 14. Set the [[HasInstance]] internal property of F as described in
 *     15.3.4.5.3.
 */
function JoinArguments(a1,a2,a3,a4,a5,a6,a7)
{
  var arr = [a1,a2,a3,a4,a5,a6,a7]
  this.args = Array.prototype.join.call(arr, ", ");
}

var Join1 = JoinArguments.bind(null, 1);
var Join2 = Join1.bind(null, 2);
var Join3 = Join2.bind(null, 3);
var Join4 = Join3.bind(null, 4);
var Join5 = Join4.bind(null, 5);
var Join6 = Join5.bind(null, 6);

var r = new Join6(7);
/*assertEq(27, r instanceof Join6, true);
assertEq(28, r instanceof Join5, true);
assertEq(29, r instanceof Join4, true);
assertEq(30, r instanceof Join3, true);
assertEq(31, r instanceof Join2, true);
assertEq(32, r instanceof Join1, true);
assertEq(33, r instanceof JoinArguments, true);*/
assertEq(34, r.args, "1, 2, 3, 4, 5, 6, 7");


/*
 * 15. If the [[Class]] internal property of Target is "Function", then
 *   a. Let L be the length property of Target minus the length of A.
 *   b. Set the length own property of F to either 0 or L, whichever is larger.
 * 16. Else set the length own property of F to 0.
 */
function one(a) { }
assertEq(35, one.bind().length, 1);
assertEq(36, one.bind(null).length, 1);
assertEq(37, one.bind(null, 1).length, 0);
assertEq(38, one.bind(null, 1, 2).length, 0);

/*
 * 17. Set the attributes of the length own property of F to the values
 *     specified in 15.3.5.1.
 */
var len1Desc =
  Object.getOwnPropertyDescriptor(function(a, b, c){}.bind(), "length");
assertEq(39, len1Desc.value, 3);
assertEq(40, len1Desc.writable, false);
assertEq(41, len1Desc.enumerable, false);
assertEq(42, len1Desc.configurable, false);

var len2Desc =
  Object.getOwnPropertyDescriptor(function(a, b, c){}.bind(null, 2), "length");
assertEq(43, len2Desc.value, 2);
assertEq(44, len2Desc.writable, false);
assertEq(45, len2Desc.enumerable, false);
assertEq(46, len2Desc.configurable, false);


/*
 * 18. Set the [[Extensible]] internal property of F to true.
 */
var bound = (function() { }).bind();

var isExtensible = Object.isExtensible || function() { return true; };
assertEq(47, isExtensible(bound), true);

bound.foo = 17;
var fooDesc = Object.getOwnPropertyDescriptor(bound, "foo");
assertEq(48, fooDesc.value, 17);
assertEq(49, fooDesc.writable, true);
assertEq(50, fooDesc.enumerable, true);
assertEq(51, fooDesc.configurable, true);


/* 22. Return F. */
var passim = function q(){}.bind(1);
assertEq(52, typeof passim, "function");