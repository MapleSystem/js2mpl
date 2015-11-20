var init;
var innerbar;
var innermostbar;

function bar(p1) {
  var counter = init;
  innerbar = function bar1(q1) {
    counter ++;
    q1 += counter + p1;
    innermostbar = function bar2(r1) {
      counter ++;
      r1 += p1 + q1 + counter;
      return r1;
    }
    return q1;
  }
  p1 += counter;
  return p1;
}

init = 10;
var a = bar(1);
var b = innerbar(10);
var c = innermostbar(100);

if (a != 11) $ERROR("failed: expect 11 but get",  a, "\n");
if (b != 32) $ERROR("failed: expect 32 but get",  b, "\n");
if (c != 155) $ERROR("failed: expect 155 but get",  c, "\n");
print("pass");

