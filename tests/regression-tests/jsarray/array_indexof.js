// CHECK #13: indexOf
var array = [2, 5, 9];
if (array.indexOf(2) !== 0){
  $ERROR('#13 indexOf: Actual: ' + array.indexOf(2) + ' Expected: ' + 0);
}
if (array.indexOf(7) !== -1){
  $ERROR('#13 indexOf: Actual: ' + array.indexOf(7) + ' Expected: ' + -1);
}
if (array.indexOf(9, 2) !== 2){
  $ERROR('#13 indexOf: Actual: ' + array.indexOf(9, 2) + ' Expected: ' + 2);
}
if (array.indexOf(2, -1) !== -1){
  $ERROR('#13 indexOf: Actual: ' + array.indexOf(2, -1) + ' Expected: ' + -1);
}
if (array.indexOf(2, -3) !== 0){
  $ERROR('#13 indexOf: Actual: ' + array.indexOf(2, -3) + ' Expected: ' + 0);
}

// CHECK #13: indexOf with holes
var arr = [1,2,undefined,3, ,5];
var searchelem = [ ];
var index1 = arr.indexOf( searchelem, 0);
var index2 = arr.indexOf( undefined, 0);
if (index1 !== -1){
  $ERROR('#13 indexOf with holes: Actual: ' + index1 + ' Expected: ' + -1);
}
if (index2 !== 2){
  $ERROR('#13 indexOf with holes: Actual: ' + index2 + ' Expected: ' + 2);
}