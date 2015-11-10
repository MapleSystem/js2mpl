function retdouble(par)
{
  return par;
}

// var v1 = Add(1, 2.22);
var v1 = retdouble(2.22);
if (v1 == 2.22 ){
  print(" div: pass res = ", v1, "\n");
} else {
  $ERROR("test failed v1 expect 3 but get",  v1, "\n");
}

var v1 = retdouble(1.22000026702878039408517452102E0);
if (v1 == 1.22000026702878039408517452102E0 ){
  print(" div: pass res = ", v1, "\n");
} else {
  $ERROR("test failed v1 expect 3 but get",  v1, "\n");
}
var v1 = retdouble(1.2200002670287804);
if (v1 == 1.2200002670287804){
  print(" div: pass res = ", v1, "\n");
} else {
  $ERROR("test failed v1 expect 3 but get",  v1, "\n");
}
