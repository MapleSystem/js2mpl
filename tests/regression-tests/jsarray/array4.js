// CHECK #7: reverse
var arr = [1, 2, 3];
arr.reverse();
if (arr.toString() !== '3,2,1'){
  $ERROR('#7 reverse: Actual: ' + arr.toString() + ' Expected: ' + '3,2,1');
}

// CHECK #7: reverse with holes
var arr = [ , 1,2,3,,5];
arr.reverse();
if (arr.toString() !== '5,,3,2,1,'){
  $ERROR('#7 reverse with holes: Actual: ' + arr.toString() + ' Expected: ' + '5,,3,2,1,');
}
if (arr.length !== 6){
  $ERROR('#7 reverse with holes: Actual: ' + arr.length + ' Expected: ' + 6);
}

// CHECK #8: shift
var arr = [1, 2, 3];
var first = arr.shift();
if (first !== 1){
  $ERROR('#8 shift: Actual: ' + first + ' Expected: ' + 1);
}
if (arr.toString() !== '2,3'){
  $ERROR('#8 shift: Actual: ' + arr.toString() + ' Expected: ' + '2,3');
}

// CHECK #9: slice
var fruits = ['Banana', 'Orange', 'Lemon', 'Apple', 'Mango'];
var citrus = fruits.slice(1, 3);
if (citrus.toString() !== 'Orange,Lemon'){
  $ERROR('#9 slice: Actual: ' + citrus.toString() + ' Expected: ' + 'Orange,Lemon');
}
var fruit = ['cherries', 'apples', 'bananas'];
fruit.sort();
if (fruit.toString() !== 'apples,bananas,cherries'){
  $ERROR('#9 slice: Actual: ' + fruit.toString() + ' Expected: ' + 'apples,bananas,cherries');
}

// CHECK #9: slice with holes
var arr = [ 1, 2, , , 5];
var arr1 = arr.slice(1,4);
if (arr1.toString() !== '2,,'){
  $ERROR('#9 slice with holes: Actual: ' + arr1.toString() + ' Expected: ' + '2,,');
}