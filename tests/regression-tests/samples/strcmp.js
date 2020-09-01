function info(a) {
  var str = "info: " + a;
  return str;
}

a = "info: hello";
b = info("hello")
if (a!==b) $ERROR('#1 const_str !== str');
c = a.toString();
if (a!==c) $ERROR('#2 const_str !== const_str.toString()');
d = b.toString();
if (b!==d) $ERROR('#3 str !== str.toString()');
if (c!==d) $ERROR('#4 const_str.toString() !== str.toString()');
print("pass");
