var init;
var a;

function bar() {
  var counter = init;
  function innerbar() {
    counter ++;
    return counter;
  }
  a = innerbar;
  return counter;
}

init = 10;
var i = bar();
a();
var x = a();

if (i != init) $ERROR("closure6 failed: expect x == init but get x=", x, "init=", init);
if (x != 12) $ERROR("closure6 failed: expect x == 12 but get x=", x);
print("pass");
