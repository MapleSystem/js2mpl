var string = "abc";
var number = 12;
var undef = undefined;
var type_null = null;
var type_bool = true;
var x = typeof(string);
var y = typeof(number);
var z = typeof(undef);
var p = typeof(type_null);
var q = typeof(type_bool);
print(x);
print(y);
print(z);
print(p);
print(q);

if (x !== "string") 
  $ERROR("typeof failed: expect x == string but get", x);
if(y !== "number")
  $ERROR("typeof failed: expect y == number but get", y);
if(z !== "undefined")
  $ERROR("typeof failed: expect z == undefined but get", z);
if(p !== "object")
  $ERROR("typeof failed: expect p == object but get", p);
if(q !== "boolean")
  $ERROR("typeof failed: expect q == boolean but get", q);
else
  print("typeof: pass\n");  
