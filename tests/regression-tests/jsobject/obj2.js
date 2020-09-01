var foo = new Object();
foo.f1 = "hello";
foo.f2 = 333;
for (var i in foo) {
  print(i, foo[i]);
  if (i === "f1") {
    if (foo[i] !== "hello")
      $ERROR("#1 failed");
  }
  if (i === "f2") {
    if (foo[i] !== 333)
      $ERROR("#2 failed");
  }
}

