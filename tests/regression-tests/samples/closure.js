var init;

function bar() {
  var counter = init;
  function innerbar() {
    counter ++;
    return counter;
  }
  return innerbar;
}

init = 10;
var a = bar();
var x = a();
var y = a();

if (x != 11) $ERROR("closure failed: expect x == 11 but get", x);
if (y != 12) $ERROR("closure failed: expect y == 12 but get", y);
print("pass");

