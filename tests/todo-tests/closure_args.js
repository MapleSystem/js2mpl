var init;

function bar(pp) {
  var counter = init;
  var qq = counter+pp;
  function innerbar(aa) {
    counter ++;
    aa += counter + qq;
    return aa;
  }
  return innerbar;
}

init = 10;
var a = bar(100);
var x = a(200);
var y = a(200);

if (x != 321) $ERROR("closure_args failed: expect x == 321 but get", x);
if (y != 322) $ERROR("closure_args failed: expect y == 322 but get", y);
print("pass");

