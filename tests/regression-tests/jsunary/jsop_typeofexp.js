var x = typeof("abc");
var y = typeof(12);
var z = typeof(true + 0);
var m = typeof(3 + "hello");
print(x);
print(y);
print(z);
print(m);
if (x !== "string")
  $ERROR("typeof failed: expect x == string but get", x);
if(y !== "number")
  $ERROR("typeof failed: expect y == number but get", y);
if(z !== "number")
  $ERROR("typeof failed: expect z == number but get", z);
if(m !== "string")
  $ERROR("typeof failed: expect m == string but get", m);
else  
  print("typeofexp: pass\n");

