function bar() {
  qq = 99;
  function innerbar() {
    return qq;
  }
  return innerbar;
}

var a = bar();
var x = a();
qq = 100
var y = a();

if (x != 99)
  $ERROR("closure_scope failed: expect x == 99 but get", x);
if (y != 100)
  $ERROR("closure_scope failed: expect y == 100 but get", x);
print("pass");
