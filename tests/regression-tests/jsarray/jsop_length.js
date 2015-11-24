var array1 = [1,2,9,10,11];
var array2 = [3, "hello", 1.2];
var array3 = [];
var len1 = array1.length;
var len2 = array2.length;
var len3 = array3.length;
if (len1 !== 5)
  $ERROR("length failed: expect len1 == 5 but get", len1);
if (len2 !== 3)
  $ERROR("length failed: expect len2 == 3 but get", len2);
if (len3 !== 0)
  $ERROR("length failed: expect len3 == 0 but get", len3);