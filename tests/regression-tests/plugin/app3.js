// export a named function
var fiz = require('required/fiz3').fiz;
var v1 = fiz();
if (v1 !== "fiz!") {
  $ERROR("expect fiz!");
}
print(v1);
