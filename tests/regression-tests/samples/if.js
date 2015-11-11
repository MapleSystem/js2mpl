function foo(a)
{
  var d = 4;
  if (a > 2)
  {
    a = 111;
    return a;
  }
  d = 555;
  return d;
}

var x = foo(8);
var y = foo(1);

if (x == 111) {
  print(" if: pass\n");
} else {
  $ERROR("if failed: expect x == 111 but get", x);
}
if (y == 555) {
  print("if: pass\n");
} else {
  $ERROR("if failed: expect y == 555 but get", y);
}
