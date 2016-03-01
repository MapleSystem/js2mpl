var arr = [0,1,,3,,5];
Array.prototype[2] = 2;
Object.prototype[4] = 4;

// CHECK#1
if (arr[2]!== 2){
  $ERROR('#1: arr[2] === 2. Actual: ' + arr[2]);
}
// CHECK#2
if (arr[4]!== 4){
  $ERROR('#1: arr[4] === 4. Actual: ' + arr[4]);
}
// CHECK#3
if (arr.join("")!== "012345"){
  $ERROR('#1: arr.join("") === "012345". Actual: ' + arr.join(""));
}
