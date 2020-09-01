// Ported from dom/src/json/test/unit/test_decode_primitives.js
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

var x;

// check an empty object, just for sanity
var emptyObject = "{}";
x = JSON.parse(emptyObject);
assertEq(1, typeof x, "object");
assertEq(2, x instanceof Object, true);

x = JSON.parse(emptyObject);
assertEq(3, typeof x, "object");

// booleans and null
x = JSON.parse("true");
assertEq(4, x, true);

x = JSON.parse("true          ");
assertEq(5, x, true);

x = JSON.parse("false");
assertEq(6, x, false);

x = JSON.parse("           null           ");
assertEq(7, x, null);

// numbers
x = JSON.parse("1234567890");
assertEq(8, x, 1234567890);


// strings
x = JSON.parse('"foo"');
assertEq(9, x, "foo");

x = JSON.parse('"\\r\\n"');
assertEq(10, x, "\r\n");

x = JSON.parse('"\\f"');
assertEq(12, x, "\f");
