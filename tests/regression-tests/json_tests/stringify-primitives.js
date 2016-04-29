// Ported from dom/src/json/test/unit/test_encode_primitives.js
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}
// sanity
var x = JSON.stringify({});
assertEq(1, x, "{}");

// booleans and null
x = JSON.stringify(true);
assertEq(2, x, "true");

x = JSON.stringify(false);
assertEq(3, x, "false");

x = JSON.stringify(new Boolean(false));
assertEq(4, x, "false");

x = JSON.stringify(null);
assertEq(5, x, "null");

x = JSON.stringify(1234);
assertEq(6, x, "1234");

x = JSON.stringify(new Number(1234));
assertEq(7, x, "1234");

x = JSON.stringify("asdf");
assertEq(8, x, '"asdf"');

x = JSON.stringify(new String("asdf"));
assertEq(9, x, '"asdf"');

assertEq(10, JSON.stringify(undefined), undefined);
assertEq(11, JSON.stringify(function(){}), undefined);
assertEq(12, JSON.stringify(JSON.stringify), undefined);
