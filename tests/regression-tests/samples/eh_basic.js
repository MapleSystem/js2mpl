var g = 111;

try {
  throw 5;
}
catch (err) {
  g = err + 7;
}
finally {
  g++;
}
if (g != 13) $ERROR("exception handling test failed; g should be 13 but get", g, "\n");
print("pass");
