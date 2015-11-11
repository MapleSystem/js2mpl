var obj={firstname:"John",lastname:"Doe",age:50,eyecolor:"blue"};
obj[2] = 30;
var x = obj[2];
obj[3] = "grey";
var y = obj[3];
if (x !== 30)
  $ERROR("setelem failed: expect x == 30 but get", obj[2]);
if (y !== "grey")
  $ERROR("setelem failed: expect y == grey but get", obj[3]);