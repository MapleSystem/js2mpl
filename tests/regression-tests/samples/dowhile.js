function foo(n)
{
  var i = 1;
  var sum = 0;
  do
  {
    sum += i;
    i++;
  } while (i < n)
  return sum;
}

var x = foo(1);
var y = foo(10);

if (x == 1) {
  print(" dowhile: pass\n");
} else {
  $ERROR("dowhile failed: expect x == 1 but get", x);
}
if (y == 45) {
  print("dowhile: pass\n");
} else {
  $ERROR("dowhile failed: expect y == 45 but get", y);
}
