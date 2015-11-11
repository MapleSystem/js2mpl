var a = 11
var b = 22
var c = a && b

if ( a== 11 && b ==22) {
  c = 1
  print(" and: pass\n");
} else {
  c =2
  $ERROR("and expect c = 22 but get", c);
}
