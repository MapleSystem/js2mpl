function divfunc(v1, v2) {
  return v1/v2;
}

var x = divfunc(1,2);
var y = divfunc(444,222);
//if (x != 0.5) $ERROR("failed: expect 0.5 but get", x);
if (y != 2) $ERROR("failed: expect 2 but get", y);
