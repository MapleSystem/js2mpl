print("main starting");
var a = require('./required/require_a');
var b = require('./required/require_b');
print("in main, a.done = ");
// print(a.done);
if (a.done != true) {
  $ERROR("in main, expect a.done = true");
}
print(",b.done = ");
// print(b.done);
if (b.done != true) {
  $ERROR("in main, expect b.done = true");
}
