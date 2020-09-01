function bar(q1, q2) {
  return function foo() {
    return q2;
  }
}

var x = bar(250, 100);
var y = x();

if (y != 100) $ERROR("failed: expect y == 100 but get", y);
print("pass");
