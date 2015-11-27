print ("Array.prototype.join");

var arr = [0,1,,3,4];

arr[2] = null;
// CHECK#8
if (arr.join("*") !== "0*1**3*4"){
  $ERROR('#8: arr.join("*") === "0*1**3*4". Actual: ' + arr.join("*"));
}

arr[2] = undefined;
// CHECK#9
if (arr.join("*") !== "0*1**3*4"){
  $ERROR('#9: arr.join("*") === "0*1**3*4". Actual: ' + arr.join("*"));
}

var arr = new Array(10);
// CHECK#10
if (arr.join("") !== ""){
  $ERROR('#10: arr.join("") === "". Actual: ' + arr.join(""));
}

// CHECK#11
if (arr.join() !== ",,,,,,,,,"){
  $ERROR('#11: arr.join() === ",,,,,,,,,". Actual: ' + arr.join());
}

// CHECK#12
if (arr.join("|") !== "|||||||||"){
  $ERROR('#12: arr.join("|") === "|||||||||". Actual: ' + arr.join("|"));
}
