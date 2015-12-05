var foo = new Object();
foo.f1 = "hello";
foo.f2 = 333;
for (var i in foo)
  print(i, foo[i]);
