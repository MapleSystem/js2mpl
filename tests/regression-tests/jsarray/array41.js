// CHECK #20: reduce
var total = [0, 1, 2, 3].reduce(function(a, b) {
  return a + b;
});
if (total !== 6){
  $ERROR('#20 reduce: Actual: ' + total + ' Expected: ' + 6);
}

// CHECK #21: reduceRight
var result = [0, 1, 2, 3, 4].reduceRight(function(previousValue, currentValue, index, array) {
  return previousValue + currentValue;
}, 10);
if (result !== 20){
  $ERROR('#21 reduceRight: Actual: ' + result + ' Expected: ' + 20);
}
