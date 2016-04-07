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

assertEq(1, function testcase() {
    var proto = {};
    Object.defineProperty(proto, "prop", {get: function () {return {};}, enumerable: true});
    var ConstructFun = function () {};
    ConstructFun.prototype = proto;
    var child = new ConstructFun;
    return Object.getOwnPropertyNames(child).indexOf('prop');
}(), -1);

