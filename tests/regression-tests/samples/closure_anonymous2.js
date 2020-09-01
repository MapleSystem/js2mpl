function bar(p1) {
  var counter = p1;
  return function (q1) {
    var anony1 = counter;
    counter ++;
    anony1 += counter;
    return function (r) {
      r += counter + q1 + anony1;
      return r;
    };
  };
}

var a = bar(100);
var x = a(10);
var y = x(20);

if (y != 332) $ERROR("closure_anonymous failed: expect y == 332 but get", y);
print("pass");
