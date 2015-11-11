function bar(aa) {
  aa = 250;
  return aa;
}

var x = bar(100);

if (x == 250) {
  print(" setarg: pass\n");
} else {
  $ERROR("setarg failed: expect x == 250 but get", x);
}
