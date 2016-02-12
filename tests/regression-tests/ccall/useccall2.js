var v = ccall("add", 53, 123);
func = "sub";
var w = ccall(func, v, 250)

var x = ccall("add"+"4", 100, 250, 44, 5)

if (v != 176) $ERROR("failed: expect 176 but get",  v);
if (w != -74) $ERROR("failed: expect -74 but get",  w);
if (x != 399) $ERROR("failed: expect 399 but get",  w);
print("pass");

