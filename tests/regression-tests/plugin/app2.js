// export an anonymous function
var v1 = "sssssaaaaa";
var bar = require('./required/bar2');
var v2 = bar(v1);
if (bar(v1) !== "sssssaaaaabar!") {
  $ERROR("expect sssssaaaaabar");
}
print(v2);
