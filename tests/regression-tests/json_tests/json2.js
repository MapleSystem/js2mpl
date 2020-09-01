x = JSON.parse('{"foo":[]}');
if (Object.getPrototypeOf(x.foo) !== Array.prototype)
  ERROR("error");
x = JSON.parse('{"foo":[], "bar":[]}');
