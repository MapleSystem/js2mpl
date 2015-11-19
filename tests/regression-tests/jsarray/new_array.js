var arr1 = new Array(3);
var a = arr1[0]
if (a !== undefined)
  $ERROR("new Array(3) failed: expect a == undefined but get", a);

var arr2 = new Array();
a = arr2[0]
if (a !== undefined)
  $ERROR("new Array() failed: expect a == undefined but get", a);

var arr3 = new Array('hi');
a = arr3[0]
if (a !== 'hi')
  $ERROR("new Array('hi') failed: expect a == 'hi' but get", a);

var arr4 = new Array(1,2,'hi');
a = arr4[2]
if (a !== 'hi')
  $ERROR("new Array(1,2,'hi') failed: expect a == 'hi' but get", a);