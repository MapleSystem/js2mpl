/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Author: Tom Schuster
 */


function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}
JSON.stringify(new Boolean(false), function(k, v) { 
    assertEq(1, typeof v, "object"); 
});

assertEq(2, Boolean.prototype.hasOwnProperty('toJSON'), false);

Object.prototype.toJSON = function() { return 2; };
assertEq(3, JSON.stringify(new Boolean(true)), "2");