function myFunction(var1, var2)
{
  return (var1 + var2) + 1;
}

var res = myFunction("Hello", "World");

if (res === "HelloWorld1")
   print("pass");
else $ERROR("test fails: expect HelloWorld1 but get ", res);


