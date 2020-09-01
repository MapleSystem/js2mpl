var i = 3;
var j = 5;
var x = (i > 4) ? ((j > 10) ? 8 : 9) : j;
if (x === 5)
  print("pass\n");
else $ERROR("test failed: expect 5 but get ", x);
var y = (i < 4) ? ((j > 10) ? 8 : 9) : j;
if (y === 9)
  print("pass\n");
else $ERROR("test failed: expect 9 but get ", y);
var z = (i < 4) ? ((j < 10) ? 8 : 9) : j;
if (z === 8)
  print("pass\n");
else $ERROR("test failed: expect 8 but get ", z);
var w = (i > 4) ? ((j < 10) ? 8 : 9) : ((j === 5) ? 6 : 7);
if (w === 6)
  print("pass\n");
else $ERROR("test failed: expect 6 but get ", w);
var u = (i > 4) ? ((j < 10) ? 8 : 9) : ((j !== 5) ? 6 : i);
if (u === 3)
  print("pass\n");
else $ERROR("test failed: expect 3 but get ", u);
