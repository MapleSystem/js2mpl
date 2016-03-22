var x = function (a, b, c, d, e, f) {
  return 1;
}

function test() {
  var a = {};
  var b = {};
  return x(a) + b;
}

if (test() !== "1" + "[object Object]")
  $ERROR("error 1");




