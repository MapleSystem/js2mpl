function Testing() {
  var foo = new Object();
  foo.f1 = "hello";
  foo.gee = 333;
  foo[99] = 999;
  var result = "";
  for (var i in foo)
    result += i;
  return result;
}

var res = Testing();
if (res === "99geef1")
  print("pass\n");
else $ERROR("test failed: expect f1gee99 but get ", res);
