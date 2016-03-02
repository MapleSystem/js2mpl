function f0(){
return this.a;
}

var g0 = f0.bind({a:"azerty"});
print(g0());
if (g0() !== "azerty") {
  $ERROR("#1 error");
}

var o = {a:37, f:f0, g:g0};
print(o.f(), o.g()); // 37, azerty
if (o.f() !== 37) {
  $ERROR("#2 error", o.f());
}

if (o.g() !== "azerty") {
  $ERROR("#3 error");
}


function f1(b, c){
return this.a + b + c;
}

var g1 = f1.bind({a:"azerty"}, "bound");
print(g1(3)); // azerty

o = {a:37, f:f1, g:g1};
print(o.f(1), o.g(1)); // 37, azerty

if (o.f(1) !== 38) {
  $ERROR("#4 error");
}

if (o.g(1) != "azertybound1") {
  $ERROR("#5 error");
}


var gg = Array.prototype.join.bind([0, 1, 2], "abc");

print(gg());
if (gg() !== "0abc1abc2")
  $ERROR("#6 error");




