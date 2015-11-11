var v1 = a;
// var a;
function xprop(v1) {
  a = 22;
  a += v1;
  return a;
}

// assertEq(xprop(3), 3);
var x1 = xprop(33);
if (x1 == 55) {
  print(" add-var.js: pass\n");
} else {
  $ERROR("for failed: expect x1 == 55 but get x1=", x1);
}
