function Binary(par1, par2)
{
  var sum;
  sum = par1 | par2;
  if (sum != 819)
    $ERROR("test failed sum expect 819 but get",  sum, "\n");

  sum = par1 ^ par2;
  if (sum != 819)
    $ERROR("test failed sum expect 819 but get",  sum, "\n");

  sum = par1 & par2;
  if (sum != 0)
    $ERROR("test failed sum expect 0  but get",  sum, "\n");

  sum = par1 == par2;
  if (sum != 0)
    $ERROR("test failed sum expect 0  but get",  sum, "\n");

  sum = par1 != par2;
  if (sum != 1)
    $ERROR("test failed sum expect 1  but get",  sum, "\n");

  sum = par1 < par2;
  if (sum != 1)
    $ERROR("test failed sum expect 1  but get",  sum, "\n");

  sum = par1 > par2;
  if (sum != 0)
    $ERROR("test failed sum expect 0  but get",  sum, "\n");

  sum = par1 >= par2;
  if (sum != 0)
    $ERROR("test failed sum expect 0  but get",  sum, "\n");

  sum = par1 << par2;
  if (sum != 1092)
    $ERROR("test failed sum expect 1092 but get",  sum, "\n");

  sum = par1 >> par2;
  if (sum != 68)
    $ERROR("test failed sum expect 68 but get",  sum, "\n");

  sum = par1 >>> par2;
  if (sum != 68)
    $ERROR("test failed sum expect 68 but get",  sum, "\n");

  sum = par1 + par2;
  if (sum != 819)
    $ERROR("test failed sum expect 819 but get",  sum, "\n");

  sum = par1 - par2;
  if (sum != -273)
    $ERROR("test failed sum expect -273 but get",  sum, "\n");

  sum = par1 * par2;
  if (sum != 149058)
    $ERROR("test failed sum expect 149058 but get",  sum, "\n");

//  sum = par1 / par2;
 // print(sum);
 // print(" expect 0.5\n");

  sum = par1 % par2;
  if (sum != 273)
    $ERROR("test failed sum expect 273 but get",  sum, "\n");
  return(sum)
}


var ret = Binary(0x111,0x222)
  if (ret != 273)
    $ERROR("test failed ret expect 273 but get",  sum, "\n");
print(ret);
print(" expect 273\n");
