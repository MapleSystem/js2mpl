function Unary(par)
{
  var sum;
  sum = !par;

  sum = ~par;

  sum = -par;

  return(sum)
}


var x = Unary(11223344);

if (x == -11223344) {
  print(" unary: pass\n");
} else {
  $ERROR("unary failed: expect x == -11223344 but get", x);
}

