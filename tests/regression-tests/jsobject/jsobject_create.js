
var my_obj = Object.create({}, {foo1:{ value:2, enumerable: false }, foo2:{value:3}});
my_obj.foo = 1;

var names = Object.getOwnPropertyNames(my_obj);

print(names[0]);
print(names[1]);
print(names[2]);

if(names[2] !== "foo2")
  $ERROR("fail 1");
if(names[1] !== "foo1")
  $ERROR("fail 2");
if(names[0] !== "foo")
  $ERROR("fail 3");


var my_obj2 = Object.create({});
names = Object.getOwnPropertyNames(my_obj2);

print(names[0]);

// undefined

if(names[0] !== undefined)
  $ERROR("fail 4");
