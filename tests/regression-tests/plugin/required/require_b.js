// console.log('b starting');
print('b starting');
exports.done = false;
var a = require('./required/require_a');
// console.log('in b, a.done = %j', a.done);
print("in b, a.done = ");
// print(a.done);
if (a.done != false) {
  $ERROR("in b, a.done expect false");
}
exports.done = true;
// console.log('b done');
print("b done");
