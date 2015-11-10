function myFunction(var1, var2)
{
  return (var1 + var2) + 1;
}

var res = myFunction("Hello", "World");
print(res);
if (res !== "HelloWorld1") 
  $ERROR("typeof failed: expect x == string but get", x);

res = myFunction(345, "World");
print(res);
if (res !== "345World1") 
  $ERROR("typeof failed: expect x == string but get", x);


res = myFunction(3.3434, "World");
print(res);
if (res !== "3.3434World1") 
  $ERROR("typeof failed: expect x == string but get", x);


res = myFunction("World", 345);
print(res);
if (res !== "World3451") 
  $ERROR("typeof failed: expect x == string but get", x);


res = myFunction("World", 33.3434);
print(res);
if (res !== "World33.34341") 
  $ERROR("typeof failed: expect x == string but get", x);

res = myFunction("World", 33.00);
print(res);
if (res !== "World331") 
  $ERROR("typeof failed: expect x == string but get", x);
