/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */
function assertEq(message, actual, expected)
{
  if (actual !== expected){
    $ERROR('CHECK #'+ message +': ' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

var testArray = [1, 2, 3]
assertEq(1, testArray['0' + '\0'], undefined);
assertEq(2, testArray['1' + '\0' + 'aaaa'], undefined)
assertEq(3, testArray['\0' + '2'], undefined);
assertEq(4, testArray['\0' + ' 2'], undefined);

testArray['\0'] = 'hello';
testArray[' \0'] = 'world';
assertEq(5, testArray['\0'], 'hello');
assertEq(6, testArray[' \0'], 'world');

