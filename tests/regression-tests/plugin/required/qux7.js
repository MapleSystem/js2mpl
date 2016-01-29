var Qux = function() {};
Qux.prototype.log = function() {
  // console.log('baz!');
  print("baz!");
}

exports.Qux = Qux;
