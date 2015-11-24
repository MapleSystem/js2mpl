var f = function square(i) {
  return i*i;
}

var x = f(7);
if (x != 49) $ERROR("funcptr failed: expect x == 49 but get", x);
x = f(9);
if (x != 81) $ERROR("funcptr failed: expect x == 81 but get", x);
print("pass");
