function Binary(par1, par2)
{
  var sum;
  sum = par1 | par2;
  print(sum);
  print(" expect 819\n");

  sum = par1 ^ par2;
  print(sum);
  print(" expect 819\n");

  sum = par1 & par2;
  print(sum);
  print(" expect 0\n");

  sum = par1 == par2;
  print(sum);
  print(" expect false\n");

  sum = par1 != par2;
  print(sum);
  print(" expect true\n");

  sum = par1 < par2;
  print(sum);
  print(" expect true\n");

  sum = par1 > par2;
  print(sum);
  print(" expect false\n");

  sum = par1 >= par2;
  print(sum);
  print(" expect false\n");

  sum = par1 << par2;
  print(sum);
  print(" expect 1092\n");

  sum = par1 >> par2;
  print(sum);
  print(" expect 68\n");

  sum = par1 >>> par2;
  print(sum);
  print(" expect 68\n");

  sum = par1 + par2;
  print(sum);
  print(" expect 819\n");

  sum = par1 - par2;
  print(sum);
  print(" expect -273\n");

  sum = par1 * par2;
  print(sum);
  print(" expect 149058\n");

  sum = par1 / par2;
  print(sum);
  print(" expect 0.5\n");

  sum = par1 % par2;
  print(sum);
  print(" expect 273\n");
  return(sum)
}


var ret = Binary(0x111,0x222)
print(ret);
print(" expect 273\n");
