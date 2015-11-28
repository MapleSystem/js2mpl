// CHECK #3: concat
var letter = ['a', 'b', 'c'];
var number = [1, 2, 3];
var concatarr = letter.concat(number);
if (concatarr.toString() !== "a,b,c,1,2,3"){
  $ERROR('#2 concat: Actual: ' + concatstr.toString() + ' Expected: ' + "a,b,c,1,2,3");
}

// CHECK #3: concat with holes
var arr = [1,2,3,,5];
var arr1 = [6, , 8, ];
var arr2 = arr.concat(arr1, 9);
if (arr2.toString() !== "1,2,3,,5,6,,8,9"){
  $ERROR('#2 concat with holes: Actual: ' + arr2.toString() + ' Expected: ' + "1,2,3,,5,6,,8,9");
}

// CHECK #4: join
var name = ['firstname', 'middlename', 'lastname'];
var str = name.join('*');
if (str !== "firstname*middlename*lastname"){
  $ERROR('#4 join: Actual: ' + concatstr + ' Expected: ' + "firstname*middlename*lastname");
}