// Ported from dom/src/json/test/unit/test_dropping_elements_in_stringify.js

function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

assertEq(1, JSON.stringify({foo: 123}),
         '{"foo":123}');
assertEq(2, JSON.stringify({foo: 123, bar: function () {}}),
         '{"foo":123}');
assertEq(3, JSON.stringify({foo: 123, bar: function () {}, baz: 123}),
         '{"foo":123,"baz":123}');

assertEq(4, JSON.stringify([123]),
         '[123]');
assertEq(5, JSON.stringify([123, function () {}]),
         '[123,null]');
assertEq(6, JSON.stringify([123, function () {}, 456]),
         '[123,null,456]');
