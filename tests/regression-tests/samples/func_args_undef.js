function test(arg) {
  // Check and make sure that arg is not undefined
  if (typeof(arg) !== "undefined") {
    $ERROR('#1: Function argument that isn\'t provided has a value of undefined. Actual: ' + (typeof(arg)));
  }
}

test();
print("pass");
