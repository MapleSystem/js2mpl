print ("Array.prototype.join");

var arr = [0,1,,3,4];
Object.prototype[2] = 2;

// CHECK#1
if (arr.join("") !== "01234"){
  $ERROR('#1: arr.join("") === "01234". Actual: ' + arr.join(""));
}
// CHECK#2
if (arr.join(",") !== "0,1,2,3,4"){
  $ERROR('#2: arr.join(",") === "0,1,2,3,4". Actual: ' + arr.join(","));
}

arr[2] = "porkchops";
// CHECK#3
if (arr.join("*") !== "0*1*porkchops*3*4"){
  $ERROR('#3: arr.join("*") === "0*1*porkchops*3*4". Actual: ' + arr.join("*"));
}

delete Object.prototype[2];
// CHECK#4
if (arr.join("*") !== "0*1*porkchops*3*4"){
  $ERROR('#4: arr.join("*") === "0*1*porkchops*3*4". Actual: ' + arr.join("*"));
}

delete arr[2];
// CHECK#5
if (arr.join("*") !== "0*1**3*4"){
  $ERROR('#5: arr.join("*") === "0*1**3*4". Actual: ' + arr.join("*"));
}

Object.prototype[2] = null;
// CHECK#6
if (arr.join("*") !== "0*1**3*4"){
  $ERROR('#6: arr.join("*") ==="0*1**3*4". Actual: ' + arr.join("*"));
}

Object.prototype[2] = undefined;
// CHECK#7
if (arr.join("*") !== "0*1**3*4"){
  $ERROR('#7: arr.join("*") === "0*1**3*4". Actual: ' + arr.join("*"));
}
