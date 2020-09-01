var f = function Square(i)
{
  return(i*i)
}

var v1 = f(12);
if (v1!=144) $ERROR("failed: expect 144 but get", v1);
print("pass");
