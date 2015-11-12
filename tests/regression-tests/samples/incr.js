function foo() {
  var counter = 1;
  return counter ++;
};

x = foo();

if (x != 1) $ERROR("failed: expect 1 but get", x);
print("pass");
