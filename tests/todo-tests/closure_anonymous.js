function bar(p1) {
  var counter = p1;
  return function (q1, q2) {
    counter ++;
    q1 += counter + q2;
    return q1;
  }
}

var a = bar(100);
var x = a(10, 20);
var y = a(10, 20);

if (x != 131) $ERROR("closure_anonymous failed: expect x == 131 but get", x);
if (y != 132) $ERROR("closure_anonymous failed: expect y == 132 but get", y);
print("pass");
