// export a named object
var baz = require("required/baz5").Baz;
var v1 = baz.log();

if (v1 !== "baz!") {
  $ERROR("expect baz!");
}
print(v1);
