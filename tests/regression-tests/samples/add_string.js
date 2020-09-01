function Add(par1, par2)
{
  var sum;
  sum = par1 + par2;
  return(sum)
}

var v1 = Add("Hello", "Js");

if (v1 == "HelloJs" ){
  print(" add: pass\n");
} else {
  $ERROR("test failed v1 expect 3 but get",  v1, "\n");
}
