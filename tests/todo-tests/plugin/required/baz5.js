var Baz = function(){};

Baz.prototype.log = function() {
  // console.log('baz!');
  print('baz!');
}

exports.Baz = new Baz();
