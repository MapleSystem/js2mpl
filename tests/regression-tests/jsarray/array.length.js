var arr1 = [1,2,3];
var len = arr1.length;
if (len !== 3)
  $ERROR("arr1.length failed: expect len == 3 but get", len);

var arr2 = [];
len = arr2.length;
if (len !== 0)
  $ERROR("arr2.length failed: expect len == 0 but get", len);

var arr3 = new Array(3);
len = arr3.length;
if (len !== 3)
  $ERROR("arr3.length failed: expect len == 3 but get", len);

var arr4 = new Array();
len = arr4.length;
if (len !== 0)
  $ERROR("arr4.length failed: expect len == 0 but get", len);

var arr5 = new Array('hi');
len = arr5.length;
if (len !== 1)
  $ERROR("arr5.length failed: expect len == 1 but get", len);

var arr6 = new Array(1, 2, 3);
len = arr6.length;
if (len !== 3)
  $ERROR("arr6.length failed: expect len == 3 but get", len);