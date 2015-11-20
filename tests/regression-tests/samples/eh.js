function foo(x, y, z) {
  var x, y, z;
  x = y + z;
  throw x;
}

var g = 111;

try {
  foo(1,2,3);
}
catch (err) {
  try {
    err += 5;
    throw err+2;
  }
  catch (xx) {
    g = xx;
  }
}
finally {
  g++;
}
if (g != 13)
  $ERROR("exception handling test failed; g should be 13 but get", g, "\n");
