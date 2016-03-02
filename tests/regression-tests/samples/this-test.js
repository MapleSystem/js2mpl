function F() {
  this.values = 10;
  print("in F this = ", this);
}

F.prototype.f = function() {
  print("in F.prototype.f");
  print("this = ", this);
  print("in F.prototype.f, value = ", this.values);
  this.values += 100;
  if (this.values !== 110)
    $ERROR("#1 error");
  print("in F.prototype.f, value = ", this.values);
}

F.prototype.g = function() {
  print("in F.prototype.g");
  print("this = ", this);
  print("in F.prototype.g, value = ", this.values);
  this.values += 1000;
  if (this.values !== 1110)
    $ERROR("#2 error");
  print("in F.prototype.g, value = ", this.values);
}

function test() {
  var obj = new F();
  print("in test() obj = ", obj);
  if (obj.values !== 10)
    $ERROR("#3 error");
  print("in test() 1 obj.value = ", obj.values);
  obj.f();
  if (obj.values !== 110)
    $ERROR("#4 error");
  print("in test() 2 obj.value = ", obj.values);
  obj.g();
  if (obj.values !== 1110)
    $ERROR("#5 error");
  print("in test() 3 obj.value = ", obj.values);
}

test();
