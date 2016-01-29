// export a named prototype

var Qux = require('required/qux7').Qux;
var qux = new Qux();
var v1 = qux.log();
if (v1 !== "baz!") {
  $ERROR("expect baz!");
}
print(v1);
