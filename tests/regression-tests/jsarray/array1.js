// CHECK #1: toString
var arr = [3, 'Tom', 2, 'Jerry'];
var str = arr.toString();
if (str !== "3,Tom,2,Jerry"){
  $ERROR('#1 toString: Actual: ' + str + ' Expected: ' + "3,Tom,2,Jerry");
}

// CHECK #2: toLocaleString
//var arr = [3, 'Tom', 2, 'Jerry'];
//var str = arr.toLocaleString();
//if (str !== "3,Tom,2,Jerry"){
//  $ERROR('#2 toLocaleString: Actual: ' + str + ' Expected: ' + "3,Tom,2,Jerry");
//}
