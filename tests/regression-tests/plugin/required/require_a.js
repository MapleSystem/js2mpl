// ole.log('a starting');
print('a starting');
exports.done = false;
var b = require('./required/require_b');
// console.log('in a, b.done = %j', b.done);
print('in a b.done = ');
// print(b.done)
if (b.done != true) {
  $ERROR("in a b.done expect true");
}
exports.done = true;
// console.log('a done');
print("a done");
