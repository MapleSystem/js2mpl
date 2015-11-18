// Add string
var v1 = "aaa";
var v2 = "bbb";
var v3 = v1 + v2;
v2 = "ddd";
print(v3,v2);
v3 = "ccc";  // need to gc the result of v1 + v2
print(v3);

