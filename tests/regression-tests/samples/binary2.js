function Binary(par1, par2)
{
  var sum;
  sum = par1 | par2;
  print(sum);
  print(" expect 819\n");
  if (sum == 819 ){
    print(" pass\n");
  } else {
    $ERROR("bit or failed\n",  sum, "\n");
  }

  sum = par1 ^ par2;
  print(sum);
  print(" expect 819\n");
  if (sum == 819 ){
    print(" pass\n");
  } else {
    $ERROR("bit xor failed\n",  sum, "\n");
  }

  sum = par1 & par2;
  print(sum);
  print(" expect 0\n");
  if (sum == 0 ){
    print(" pass\n");
  } else {
    $ERROR("bit and failed\n",  sum, "\n");
  }

  sum = par1 == par2;
  print(sum);
  print(" expect false\n");
  if (sum == false ){
    print(" pass\n");
  } else {
    $ERROR("== failed\n",  sum, "\n");
  }

  sum = par1 != par2;
  print(sum);
  print(" expect true\n");
  if (sum == true ){
    print(" pass\n");
  } else {
    $ERROR("!= failed\n",  sum, "\n");
  }

  sum = par1 < par2;
  print(sum);
  print(" expect true\n");
  if (sum == true ){
    print(" pass\n");
  } else {
    $ERROR("< failed\n",  sum, "\n");
  }

  sum = par1 > par2;
  print(sum);
  print(" expect false\n");
  if (sum == false ){
    print(" pass\n");
  } else {
    $ERROR("> failed\n",  sum, "\n");
  }

  sum = par1 >= par2;
  print(sum);
  print(" expect false\n");
  if (sum == false ){
    print(" pass\n");
  } else {
    $ERROR(">= failed\n",  sum, "\n");
  }

  sum = par1 << par2;
  print(sum);
  print(" expect 1092\n");
  if (sum == 1092 ){
    print(" pass\n");
  } else {
    $ERROR("<< failed\n",  sum, "\n");
  }

  sum = par1 >> par2;
  print(sum);
  print(" expect 68\n");
  if (sum == 68 ){
    print(" pass\n");
  } else {
    $ERROR(">> failed\n",  sum, "\n");
  }

  sum = par1 >>> par2;
  print(sum);
  print(" expect 68\n");
  if (sum == 68 ){
    print(" pass\n");
  } else {
    $ERROR(">>> failed\n",  sum, "\n");
  }

  sum = par1 + par2;
  print(sum);
  print(" expect 819\n");
  if (sum == 819 ){
    print(" pass\n");
  } else {
    $ERROR("+ failed\n",  sum, "\n");
  }

  sum = par1 - par2;
  print(sum);
  print(" expect -273\n");
  if (sum == -273 ){
    print(" pass\n");
  } else {
    $ERROR("- failed\n",  sum, "\n");
  }

  sum = par1 * par2;
  print(sum);
  print(" expect 149058\n");
  if (sum == 149058 ){
    print(" pass\n");
  } else {
    $ERROR("* failed\n",  sum, "\n");
  }
/*
  sum = par1 / par2;
  print(sum);
  print(" expect 0.5\n");
  if (sum == 0.5){
    print(" pass\n");
  } else {
    $ERROR("/ failed\n",  sum, "\n");
  }
*/
  sum = par1 % par2;
  print(sum);
  print(" expect 273\n");
  if (sum == 273 ){
    print(" pass\n");
  } else {
    $ERROR("% failed\n",  sum, "\n");
  }
  return(sum)
}


var ret = Binary(0x111,0x222)
print(ret);
print(" expect 273\n");
