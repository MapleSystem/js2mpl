// this test passes if the changes to BINDNAME and SETNAME on 2015-09-01 
// are undone
var foo = new Object();
foo.f1 = "hello";
foo.gee = 333;
foo[99] = 999;
var result = "";
for (var i in foo)
  result += i;
if (result === "99f1gee")
  print("pass\n");
else $ERROR("test failed: expect f1gee99 but get ", result);
