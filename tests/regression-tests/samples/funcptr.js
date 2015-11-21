function square(i) {
  return i*i;
}

var f = square;
var x = f(7);
if (x != 49) $ERROR("failed: expect x == 49 but get", x);
print("pass");
