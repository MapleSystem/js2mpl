var init;

function bar() {
  var counter = init;
  function innerbar(iv1, iv2) {
    counter  = counter + iv1 + iv2;
    return counter;
  }
  return innerbar;
}

init = 10;
var a = bar();
var tmp = a(11,22);
var tmp1 = a(11,22);

if (tmp != 43)  $ERROR("closure2 failed: expect tmp == 43 but get", tmp);
if (tmp1 != 76) $ERROR("closure2 failed: expect tmp1 == 74 but get", tmp1);
print("pass");
