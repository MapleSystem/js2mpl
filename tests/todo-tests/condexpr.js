var i = 3;
var j = 5;
var x = (i > 4) ? 11 : j;
if (x === 5)
  print("pass\n");
else $ERROR("test failed: expect 5 but get ", x);


