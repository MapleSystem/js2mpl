
// empty object
var x = JSON.parse("{}");
if (Object.getOwnPropertyNames(x).length !== 0)
  $ERROR("#1:error")

// empty array
x = JSON.parse("[]");
if (Array.isArray(x) != true)
  $ERROR("#2:error");
if (x.length != 0)
  $ERROR("#3:error");

// one element array
x = JSON.parse("[[]]");
if (Array.isArray(x) != true)
  $ERROR("#4:error");
if (x.length != 1)
  $ERROR("#5:error");
if (Array.isArray(x[0]) != true)
  $ERROR("#6:error");
if (x[0].length != 0)
  $ERROR("#7:error");

// multiple arrays
x = JSON.parse("[[],[],[]]");
if (Array.isArray(x) != true)
  $ERROR("#8:error");
if (x.length != 3)
  $ERROR("#9:error");
if (Array.isArray(x[0]) != true)
  $ERROR("#10:error");
if (x[0].length != 0)
  $ERROR("#11:error");
if (Array.isArray(x[1]) != true)
  $ERROR("#12:error");
if (x[1].length != 0)
  $ERROR("#13:error");
if (Array.isArray(x[2]) != true)
  $ERROR("#14:error");
if (x[2].length != 0)
  $ERROR("#15:error");

// array key/value
x = JSON.parse('{"foo":[]}');
if (typeof(x) != "object")
  $ERROR("#16:error");
var props = Object.getOwnPropertyNames(x);
if (props.length != 1)
  $ERROR("#17:error");
if(props[0] != "foo")
  $ERROR("#18:error");
if (Array.isArray(x.foo) != true)
  $ERROR("#19:error");
if(x.foo.length !== 0)
  $ERROR("#20:error");

x = JSON.parse('{"foo":[], "bar":[]}');
if (typeof(x) != "object")
  $ERROR("#21:error");
props = Object.getOwnPropertyNames(x).sort();
if (props.length != 2)
  $ERROR("#22:error");
if(props[0] != "bar")
  $ERROR("#23:error");
if(props[1] != "foo")
  $ERROR("#23:error");
if (Array.isArray(x.foo) != true)
  $ERROR("#24:error");
if(x.foo.length !== 0)
  $ERROR("#25:error");
if (Array.isArray(x.bar) != true)
  $ERROR("#26:error");
if(x.bar.length !== 0)
  $ERROR("#27:error");

// nesting
x = JSON.parse('{"foo":[{}]}');
if (typeof(x) != "object")
  $ERROR("#28:error");
props = Object.getOwnPropertyNames(x);
if (props.length != 1)
  $ERROR("#29:error");
if(props[0] != "foo")
  $ERROR("#30:error");
if (Array.isArray(x.foo) != true)
  $ERROR("#31:error");
if(x.foo.length !== 1)
  $ERROR("#32:error");
if (typeof(x.foo[0]) != "object")
  $ERROR("#33:error");
if(Object.getOwnPropertyNames(x.foo[0]).length != 0)
  $ERROR("#34:error");

x = JSON.parse('{"foo":[{"foo":[{"foo":{}}]}]}');
if (typeof(x.foo[0].foo[0].foo) != "object")
  $ERROR("#35:error");

x = JSON.parse('{"foo":[{"foo":[{"foo":[]}]}]}');
if (Array.isArray(x.foo[0].foo[0].foo) != true)
  $ERROR("#36:error");

// strings
x = JSON.parse('{"foo":"bar"}');
if (typeof(x) != "object")
  $ERROR("#37:error");
props = Object.getOwnPropertyNames(x);
if(props.length !== 1)
  $ERROR("#38:error");
if (props[0] != "foo")
  $ERROR("#39:error");
if (x.foo != "bar")
  $ERROR("#40:error");

x = JSON.parse('["foo", "bar", "baz"]');
if (Array.isArray(x) != true)
  $ERROR("#41:error");
if (x.length != 3)
  $ERROR("#42:error");
if (x[0] != "foo")
  $ERROR("#43:error");
if (x[1] != "bar")
  $ERROR("#44:error");
if (x[2] != "baz")
  $ERROR("#45:error");

// numbers
x = JSON.parse('{"foo":55, "bar":5}');
if (typeof(x) != "object")
  $ERROR("#46:error");
props = Object.getOwnPropertyNames(x).sort();
if(props.length != 2)
  $ERROR("#47:error");
if (props[0] != "bar")
  $ERROR("#48:error");
if (props[1] != "foo")
  $ERROR("#49:error");
if (x.foo != 55)
  $ERROR("#50:error");
if (x.bar != 5)
  $ERROR("#51:error");

// keywords
x = JSON.parse('{"foo": true, "bar":false, "baz":null}');
if (typeof(x) != "object")
  $ERROR("#52:error");
props = Object.getOwnPropertyNames(x).sort();
if(props.length != 3)
  $ERROR("#53:error");
if (props[0] != "bar")
  $ERROR("#54:error");
if (props[1] != "baz")
  $ERROR("#55:error");
if (props[2] != "foo")
  $ERROR("#56:error");
if (x.foo != true)
  $ERROR("#57:error");
if (x.bar != false)
  $ERROR("#58:error");
if (x.baz != null)
  $ERROR("#59:error");
/*
// short escapes
x = JSON.parse('{"foo": "\\"", "bar":"\\\\", "baz":"\\b","qux":"\\f", "quux":"\\n", "quuux":"\\r","quuuux":"\\t"}');
props = Object.getOwnPropertyNames(x).sort();
assertEq(props.length, 7);
assertEq(props[0], "bar");
assertEq(props[1], "baz");
assertEq(props[2], "foo");
assertEq(props[3], "quuuux");
assertEq(props[4], "quuux");
assertEq(props[5], "quux");
assertEq(props[6], "qux");
assertEq(x.foo, '"');
assertEq(x.bar, '\\');
assertEq(x.baz, '\b');
assertEq(x.qux, '\f');
assertEq(x.quux, "\n");
assertEq(x.quuux, "\r");
assertEq(x.quuuux, "\t");

// unicode escape
x = JSON.parse('{"foo":"hmm\\u006dmm"}');
assertIsObject(x);
props = Object.getOwnPropertyNames(x);
assertEq(props.length, 1);
assertEq(props[0], "foo");
assertEq("hmm\u006dmm", x.foo);

x = JSON.parse('{"hmm\\u006dmm":"foo"}');
assertIsObject(x);
props = Object.getOwnPropertyNames(x);
assertEq(props.length, 1);
assertEq(props[0], "hmmmmm");
assertEq(x.hmm\u006dmm, "foo");

// miscellaneous
x = JSON.parse('{"JSON Test Pattern pass3": {"The outermost value": "must be an object or array.","In this test": "It is an object." }}');
assertIsObject(x);
props = Object.getOwnPropertyNames(x);
assertEq(props.length, 1);
assertEq(props[0], "JSON Test Pattern pass3");
assertIsObject(x["JSON Test Pattern pass3"]);
props = Object.getOwnPropertyNames(x["JSON Test Pattern pass3"]).sort();
assertEq(props.length, 2);
assertEq(props[0], "In this test");
assertEq(props[1], "The outermost value");
assertEq(x["JSON Test Pattern pass3"]["The outermost value"],
                         "must be an object or array.");
assertEq(x["JSON Test Pattern pass3"]["In this test"], "It is an object.");*/
