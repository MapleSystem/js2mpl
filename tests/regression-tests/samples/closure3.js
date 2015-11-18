var init;
var a;

function bar() {
  var counter = init;
  function innerbar() {
    counter ++;
    return counter;
  }
  a = innerbar;
  return;
}

init = 10;
bar();
a();
var x = a();
bar();
var y = a();


if (x != 12) $ERROR("closure3 failed: expect x == 12 but get", x);
if (y != 11) $ERROR("closure3 failed: expect y == 11 but get", y);
print("pass");
