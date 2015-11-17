var arr = [2, 4, 'hello'];
var x = arr[2];
if (x !== 'hello')
  $ERROR("getelem failed: expect x == 'hello' but get", arr[2]);
arr[2] = 'hi';
var x = arr[2];
if (x !== 'hi')
  $ERROR("setelem failed: expect x == 'hi' but get", arr[2]);
