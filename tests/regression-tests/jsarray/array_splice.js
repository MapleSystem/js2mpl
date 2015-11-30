// CHECK #11: splice
var arr = [1, 2, 3, 4];
var deleted = arr.splice(1, 2, 'hello');
if (arr.toString() !== '1,hello,4'){
  $ERROR('#11 splice: Actual: ' + arr.toString() + ' Expected: ' + '1,hello,4');
}
if (deleted.toString() !== '2,3'){
  $ERROR('#11 splice: Actual: ' + deleted.toString() + ' Expected: ' + '2,3');
}

// CHECK #11: splice with holes
var arr = [ 1, 2,  ,3, 4, 5];
var arr2 = ['hello', , 'hi']
var arr1 = arr.splice(1, 2, arr2);
if (arr.toString() !== '1,hello,,hi,3,4,5'){
  $ERROR('#11 splice with holes: Actual: ' + arr.toString() + ' Expected: ' + '1,hello,,hi,3,4,5');
}
if (arr1.toString() !== '2,'){
  $ERROR('#11 splice with holes: Actual: ' + arr1.toString() + ' Expected: ' + '2,');
}