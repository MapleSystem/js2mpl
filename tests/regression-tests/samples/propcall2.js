function F(b) {
  this.gv = b;
}

F.prototype.f = function(a) {
  return this.gv+a;
}

F.prototype.g = function(a) {
  return this.gv-a;
}

var o = new F(9);
var x = o.f(5);
var y = o.g(5);
var o2 = new F(90);
var x2 = o2.f(50);
var y2 = o2.g(50);

if (x != 14) $ERROR("failed: expect x == 14 but get", x);
if (y != 4) $ERROR("failed: expect y == 4 but get", y);
if (x2 != 140) $ERROR("failed: expect x == 140 but get", x2);
if (y2 != 40) $ERROR("failed: expect y == 40 but get", y2);
print("pass");

