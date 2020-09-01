// CHECK #12: unshift
var arr = [1, 2, 3];
arr.unshift(-1, -2, 0);
if (arr.toString() !== '-1,-2,0,1,2,3'){
  $ERROR('#12 unshift: Actual: ' + arr.toString() + ' Expected: ' + '-1,-2,0,1,2,3');
}