func = "add";
var v = ccall(func, 123, 40);

if (v != 163) $ERROR("failed: expect 163 but get",  v);
print("pass");

