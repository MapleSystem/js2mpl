var a = 11
var b = 22
var c 

if (a ==11 || b == 22) {
  c =1
  print(" or: pass\n");
} else {
c = 2
  $ERROR("or failed: expect c == 11 but get", c);
}
