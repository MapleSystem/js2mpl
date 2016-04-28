// Ported from dom/src/json/test/unit/test_wrappers.js
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

function assertStringify(message, v, expect)
{
  assertEq(message, JSON.stringify(v), expect);
}

assertStringify(1, {}, "{}");
assertStringify(2, [], "[]");
assertStringify(3, {"foo":"bar"}, '{"foo":"bar"}');
assertStringify(4, {"null":null}, '{"null":null}');
assertStringify(5, {"five":5}, '{"five":5}');
assertStringify(6, {"five":5, "six":6}, '{"five":5,"six":6}');
assertStringify(7, {"x":{"y":"z"}}, '{"x":{"y":"z"}}');
assertStringify(8, {"w":{"x":{"y":"z"}}}, '{"w":{"x":{"y":"z"}}}');
assertStringify(9, [1,2,3], '[1,2,3]');
assertStringify(10, {"w":{"x":{"y":[1,2,3]}}}, '{"w":{"x":{"y":[1,2,3]}}}');
assertStringify(11, {"false":false}, '{"false":false}');
assertStringify(12, {"true":true}, '{"true":true}');
assertStringify(13, {"child has two members": {2:"and this one","this":"one"}},
                '{"child has two members":{"this":"one","2":"and this one"}}');
assertStringify(14, {"x":{"a":"b","c":{"y":"z"},"f":"g"}},
                '{"x":{"a":"b","c":{"y":"z"},"f":"g"}}');
assertStringify(15, {"x":[1,{"y":"z"},3]}, '{"x":[1,{"y":"z"},3]}');
assertStringify(16, [new String("hmm")], '["hmm"]');
assertStringify(17, [new Boolean(true)], '[true]');
assertStringify(18, [new Number(42)], '[42]');

assertStringify(19, [1,,3], '[1,null,3]');

var x = {"free":"variable"};
assertStringify(20, x, '{"free":"variable"}');
assertStringify(21, {"y":x}, '{"y":{"free":"variable"}}');

// array prop
assertStringify(22, { a: [1,2,3] }, '{"a":[1,2,3]}');

assertStringify(23, {"y": { foo: function(hmm) { return hmm; } } }, '{"y":{}}');

// test toJSON
var hmm = { toJSON: function() { return {"foo":"bar"} } };
assertStringify(24, {"hmm":hmm}, '{"hmm":{"foo":"bar"}}');
assertStringify(25, hmm, '{"foo":"bar"}'); // on the root

// toJSON on prototype
var Y = function() {
  this.not = "there?";
  this.d = "e";
};
Y.prototype = {
  not: "there?",
  toJSON: function() { return {"foo":"bar"}}
};
var y = new Y();
assertStringify(26, y.toJSON(), '{"foo":"bar"}');
assertStringify(27, y, '{"foo":"bar"}');

// return undefined from toJSON
assertStringify(28, {"hmm": { toJSON: function() { return; } } }, '{}');

// array with named prop
var x = new Array();
x[0] = 1;
x.foo = "bar";
assertStringify(29, x, '[1]');

// prototype
var X = function() { this.a = "b" };
X.prototype = { c: "d" };
assertStringify(30, new X(), '{"a":"b"}');

