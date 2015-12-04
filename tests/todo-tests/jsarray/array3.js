// CHECK #15: every
function isBigEnough(element, index, array) {
  return element >= 10;
}
if ([12, 5, 8, 130, 44].every(isBigEnough) !== false){
  $ERROR('#15 every: Actual: ' + [12, 5, 8, 130, 44].every(isBigEnough) + ' Expected: ' + 'false');
}
if ([12, 54, 18, 130, 44].every(isBigEnough) !== true){
  $ERROR('#15 every: Actual: ' + [12, 54, 18, 130, 44].every(isBigEnough) + ' Expected: ' + 'true');
}

// CHECK #16: some
function isBiggerThan10(element, index, array) {
  return element > 10;
}
if ([2, 5, 8, 1, 4].some(isBiggerThan10) !== false){
  $ERROR('#16 some: Actual: ' + [2, 5, 8, 1, 4].some(isBiggerThan10) + ' Expected: ' + false);
}
if ([12, 5, 8, 1, 4].some(isBiggerThan10) !== true){
  $ERROR('#16 some: Actual: ' + [12, 5, 8, 1, 4].some(isBiggerThan10) + ' Expected: ' + true);
}

// CHECK #17: forEach
function func1(element, index, array) {
  array[index] += index;
}
var arr = [2, 5, , 9];
arr.forEach(func1);
if (arr.toString() !== '2,6,,12'){
  $ERROR('#17 forEach: Actual: ' + arr.toString() + ' Expected: ' + '2,6,,12');
}

// CHECK #18: map
function func2(element, index, array) {
  return (element + index);
}
var arr = [2, 5, , 9];
var maparr = arr.map(func2);
if (maparr.toString() !== '2,6,,12'){
  $ERROR('#18 map: Actual: ' + maparr.toString() + ' Expected: ' + '2,6,,12');
}

// CHECK #19: filter
var filtered = [12, 5, 8, 130, 44].filter(isBigEnough);
if (filtered.toString() !== '12,130,44'){
  $ERROR('#19 filter: Actual: ' + filtered.toString() + ' Expected: ' + '12,130,44');
}