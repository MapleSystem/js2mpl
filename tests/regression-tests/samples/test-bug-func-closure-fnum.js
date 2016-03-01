/*===
foo,foo
===*/

/* This used to have a bug: closure numbers would get incorrectly used. */

function F() {
  this.values = [];
}
F.prototype.f = function() {
  this.values[this.values.length] = 'foo';
}
F.prototype.g = function() {
  this.values[this.values.length] = 'bar';
}

function test() {
  var obj = new F();
  obj.f();
  obj.f();
  obj.g();
  if (obj.values[0] != "foo") $ERROR("#1 failed: expect foo but get ", obj.values[0]);
  if (obj.values[1] != "foo") $ERROR("#2 failed: expect foo but get ", obj.values[1]);
  if (obj.values[2] != "bar") $ERROR("#3 failed: expect bar but get ", obj.values[2]);
}

test();
print("pass");
