// CHECK #14: lastIndexOf
var array = [2, 5, 9, 2];
if (array.lastIndexOf(2) !== 3){
  $ERROR('#14 lastIndexOf: Actual: ' + array.lastIndexOf(2) + ' Expected: ' + 3);
}
if (array.lastIndexOf(7) !== -1){
  $ERROR('#14 lastIndexOf: Actual: ' + array.lastIndexOf(7) + ' Expected: ' + -1);
}
if (array.lastIndexOf(2,3) !== 3){
  $ERROR('#14 lastIndexOf: Actual: ' + array.lastIndexOf(2,3) + ' Expected: ' + 3);
}
if (array.lastIndexOf(2,2) !== 0){
  $ERROR('#14 lastIndexOf: Actual: ' + array.lastIndexOf(2,2) + ' Expected: ' + 0);
}
if (array.lastIndexOf(2,-2) !== 0){
  $ERROR('#14 lastIndexOf: Actual: ' + array.lastIndexOf(2,-2) + ' Expected: ' + 0);
}
if (array.lastIndexOf(2,-1) !== 3){
  $ERROR('#14 lastIndexOf: Actual: ' + array.lastIndexOf(2,-1) + ' Expected: ' + 3);
}