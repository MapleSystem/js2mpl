
var obj={firstname:"John",lastname:"Doe",age:50,eyecolor:"blue"};
if (obj.firstname !== "John")
  $ERROR("initprop failed: expect obj.firstname == John but get", obj.firstname);
if (obj.lastname !== "Doe")
  $ERROR("initprop failed: expect obj.lastname == Doe but get", obj.lastname);
if (obj.age !== 50)
  $ERROR("initprop failed: expect obj.age == 50 but get", obj.age);
if (obj.eyecolor !== "blue")
  $ERROR("initprop failed: expect obj.eyecolor == blue but get", obj.eyecolor);