/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */
var b = Object.create(Array.prototype);
b.length = 12;

  if (b.length !== 12){
    $ERROR('Error:' + 'Expected: ' + 12 +'. Actual: ' + b.length);
  }