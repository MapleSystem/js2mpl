var f = function square(i) {
          return i*i;
        }

var x = f(7);
if (x == 49) {
  print(" funcptr: pass\n");
} else {
  $ERROR("funcptr failed: expect x == 49 but get", x);
}
x = f(9);
if (x == 81) {
  print(" funcptr: pass");
} else {
  $ERROR("funcptr failed: expect x == 49 but get", x);
}
