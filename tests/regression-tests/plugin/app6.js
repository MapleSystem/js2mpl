var Doo = require('required/doo6');
var doo = new Doo();
var v1 = doo.log();
if (v1 !== "doo!") {
  $ERROR("expect doo!");
}
print(v1);
