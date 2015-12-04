// CHECK #10: sort
var numbers = [4, 2, 5, 1, 3];
numbers.sort(function(a, b) {return a - b;});
if (numbers.toString() !== '1,2,3,4,5'){
  $ERROR('#10 sort: Actual: ' + numbers.toString() + ' Expected: ' + '1,2,3,4,5');
}

// CHECK #10: sort with holes
var numbers = [4, undefined, 2, , 5, , 1, 3];
numbers.sort(function(a, b) {return a - b;});
if (numbers.toString() !== '1,2,3,4,5,,,'){
  $ERROR('#10 sort with holes: Actual: ' + numbers.toString() + ' Expected: ' + '1,2,3,4,5,,,');
}