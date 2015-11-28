// CHECK #5: pop
var arr = [1, 2, 3];
var last = arr.pop();
if (last !== 3){
  $ERROR('#5 pop: Actual: ' + last + ' Expected: ' + 3);
}
if (arr.toString() !== '1,2'){
  $ERROR('#5 pop: Actual: ' + arr.toString() + ' Expected: ' +'1,2');
}

// CHECK #5: pop with holes
var arr = [1,2,3,,5];
if (arr.pop() !== 5){
  $ERROR('#5 pop with holes: Actual: ' + arr.pop() + ' Expected: ' + 5);
}
if (arr.length !== 4){
  $ERROR('#5 pop with holes: Actual: ' + arr.length + ' Expected: ' + 4);
}
if (arr.pop() !== undefined){
  $ERROR('#5 pop with holes: Actual: ' + arr.pop() + ' Expected: ' + undefined);
}
if (arr.length !== 3){
  $ERROR('#5 pop with holes: Actual: ' + arr.length + ' Expected: ' + 3);
}

// CHECK #6: push
var arr = [1, 2, 3];
var newlength = arr.push('a', 'b', 'c');
if (newlength !== 6){
  $ERROR('#6 push: Actual: ' + newLength + ' Expected: ' + 6);
}
if (arr.toString() !== '1,2,3,a,b,c'){
  $ERROR('#6 push: Actual: ' + arr.toString() + ' Expected: ' + '1,2,3,a,b,c');
}

// CHECK #6: push with holes
var arr = [1,2,3,,5];
var arr1 = [6, , 8];
arr.push(arr1, 9);
if (arr.toString() !== '1,2,3,,5,6,,8,9'){
  $ERROR('#6 push: Actual: ' + arr.toString() + ' Expected: ' + '1,2,3,,5,6,,8,9');
}