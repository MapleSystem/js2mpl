function foo(n)
{
  var i = 1;
  var sum = 0;
  while (i < n)
  {
    sum += i;
    i++;
  }
  return sum;
}

var x = foo(1);
var y = foo(10);

if (x == 0) {
  print(" while: pass\n");
} else {
  $ERROR("while failed: expect x == 0 but get", x);
}
if (y == 45) {
  print("while: pass\n");
} else {
  $ERROR("while failed: expect y == 45 but get", y);
}
