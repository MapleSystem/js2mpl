var a = {foo:1, foo2:2};
print(a.toString());
var b = a.toString();
if (b !== "[object Object]")
  $ERROR("failed 1");
a = new Object();
var c = a.toString()
print(a.toString());
if (c !== "[object Object]")
  $ERROR("failed 2");


a = new Object("3333");
var d = a.toString();
print(a.toString());
if (d !== "3333")
  $ERROR("failed 3");

