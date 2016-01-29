// export an anonymous object

var buz = require('required/buz4');
var v1 = buz.log();
if (v1 !== "buz!") {
  $ERROR("expect buz!");
}
print(v1);
