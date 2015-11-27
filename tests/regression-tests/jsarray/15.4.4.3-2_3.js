print ("Array.prototype.join");

var arr = new Array(10);

arr[2] = "doubt";
// CHECK#13
if (arr.join(",") !== ",,doubt,,,,,,,"){
  $ERROR('#13: arr.join(",") === ",,doubt,,,,,,,". Actual: ' + arr.join(","));
}

arr[9] = "failure";
// CHECK#14
if (arr.join(",") !== ",,doubt,,,,,,,failure"){
  $ERROR('#14: arr.join(",") === ",,doubt,,,,,,,failure". Actual: ' + arr.join(","));
}

delete arr[2];
// CHECK#15
if (arr.join(",") !== ",,,,,,,,,failure"){
  $ERROR('#15: arr.join(",") === ",,,,,,,,,failure". Actual: ' + arr.join(","));
}
