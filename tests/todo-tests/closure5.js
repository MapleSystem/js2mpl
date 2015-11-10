var init;
var a;

function bar() {
  // counter is declared after innerbar but can be accessed 
  // when innerbar is called as jsvascript does variable hoisting
  a = function innerbar() {
    counter ++;
    return counter;
  }
  var counter = init;
  return;
}

init = 10;
bar();
a();
var x = a();
bar();
var y = a();

if (x != 12) $ERROR("closure5 failed: expect x == 12 but get", x);
if (y != 11) $ERROR("closure5 failed: expect y == 11 but get", y);
print("pass");
