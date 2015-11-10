var init;
var innerbar;
var innermostbar;

function bar() {
  var counter = init;
  innerbar = function bar1() {
    counter += 100
    innermostbar = function bar2() {
      counter += 1000
      return counter;
    }
    return counter;
  }
  return counter;
}

init = 10;
var a = bar();
var b = innerbar();
var c = innermostbar();

if (a != 10) $ERROR("failed: expect 10 but get",  a, "\n");
if (b != 110) $ERROR("failed: expect 110 but get",  b, "\n");
if (c != 1110) $ERROR("failed: expect 1110 but get",  c, "\n");
print("pass");

