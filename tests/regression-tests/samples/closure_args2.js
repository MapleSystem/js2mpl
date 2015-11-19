var init;

function bar(pp) {
  function innerbar() {
    return pp;
  }
  return innerbar;
}

init = 10;
var a = bar(99);
var x = a();

if (x != 99) $ERROR("closure_args failed: expect x == 99 but get", x);
print("pass");
