function foo(a,b) {return a+b;}
var x = foo.length;
var y = foo(3,x);

if (x != 2)
  $ERROR("failed: expect x == 2 but get", x);
if (y != 5)
  $ERROR("failed: expect y == 5 but get", y);
print("pass");
