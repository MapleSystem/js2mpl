function square(i) {
  return i*i;
}

var f = square;
var x = f(7);
if (x == 49) {
  print(" funcptr: pass\n");
} else {
  $ERROR("funcptr failed: expect x == 49 but get", x);
}
