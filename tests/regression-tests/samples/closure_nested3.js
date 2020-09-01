var innerbar;
var innermostbar;

function bar() {
  var counter = 10;
  innerbar = function bar1() {
    var counter = 250;
    innermostbar = function bar2() {
      counter ++;
      return counter;
    }
    return counter;
  }
  innerbar2 = function bar12() {
    counter ++;
    return counter;
  }
  return counter;
}

var a = bar();
var b = innerbar();
var c = innermostbar();
var d = innerbar2();

if (a!=10) $ERROR("failed: expect 10 but get", a);
if (b!=250) $ERROR("failed: expect 250 but get", b);
if (c!=251) $ERROR("failed: expect 251 but get", c);
if (d!=11) $ERROR("failed: expect 11 but get", d);
print("pass");
