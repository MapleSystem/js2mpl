/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 430133;
var summary = 'ES5 Object.defineProperties(O, Properties)';

print(BUGNUMBER + ": " + summary);

/**************
 * BEGIN TEST *
 **************/
function assertEq(message,actual, expected)
{
  if (actual !== expected){
    $ERROR('#' + message + ' Error:' + 'Expected: ' + expected +'. Actual: ' + actual);
  }
}

assertEq(1, "defineProperties" in Object, true);
assertEq(2,Object.defineProperties.length, 2);

var o, props, desc, passed;

o = {};
props =
  {
    a: { value: 17, enumerable: true, configurable: true, writable: true },
    b: { value: 42, enumerable: false, configurable: false, writable: false }
  };
Object.defineProperties(o, props);
assertEq(3,"a" in o, true);
assertEq(4,"b" in o, true);
desc = Object.getOwnPropertyDescriptor(o, "a");
assertEq(5,desc.value, 17);
assertEq(6,desc.enumerable, true);
assertEq(7,desc.configurable, true);
assertEq(8,desc.writable, true);
desc = Object.getOwnPropertyDescriptor(o, "b");
assertEq(9,desc.value, 42);
assertEq(10,desc.enumerable, false);
assertEq(11,desc.configurable, false);
assertEq(12,desc.writable, false);

/* //can not suppory NaN
props =
  {
    c: { value: NaN, enumerable: false, configurable: true, writable: true },
    b: { value: 44 }
  };
var error = "before";
try
{
  Object.defineProperties(o, props);
  error = "no exception thrown";
}
catch (e)
{
  if (e instanceof TypeError)
    error = "typeerror";
  else
    error = "bad exception: " + e;
}
assertEq(13 +  "didn't throw or threw wrongly",error, "typeerror");
assertEq(14 +  "new property added","c" in o, true);
assertEq(15 +  "old property value preserved",o.b, 42);
*/

function Properties() { }
Properties.prototype = { b: { value: 42, enumerable: true } };
props = new Properties();
Object.defineProperty(props, "a", { enumerable: false });
o = {};
Object.defineProperties(o, props);
assertEq(16,"a" in o, false);
assertEq(17 + "Object.defineProperties(O, Properties) should only use enumerable " +
         "properties on Properties", Object.getOwnPropertyDescriptor(o, "a"), undefined);
assertEq(18,"b" in o, false);
assertEq(19 + "Object.defineProperties(O, Properties) should only use enumerable " +
         "properties directly on Properties", Object.getOwnPropertyDescriptor(o, "b"), undefined);

Number.prototype.foo = { value: 17, enumerable: true };
Boolean.prototype.bar = { value: 8675309, enumerable: true };
String.prototype.quux = { value: "Are you HAPPY yet?", enumerable: true };
o = {};
Object.defineProperties(o, 5); // ToObject only throws for null/undefined
assertEq(20 + "foo is not an enumerable own property","foo" in o, false);
Object.defineProperties(o, false);
assertEq(21 + "bar is not an enumerable own property","bar" in o, false);
Object.defineProperties(o, "");
assertEq(22 + "quux is not an enumerable own property","quux" in o, false);
/*
error = "before";
try
{
  Object.defineProperties(o, "1");
}
catch (e)
{
print("hi")
print(e)
  if (e instanceof TypeError)
    error = "typeerror";
  else
    error = "bad exception: " + e;
}
print(TypeError)
assertEq(23 + "should throw on Properties == '1' due to '1'[0] not being a " +
         "property descriptor" ,error, "typeerror");

error = "before";
try
{
  Object.defineProperties(o, null);
}
catch (e)
{
  if (e instanceof TypeError)
    error = "typeerror";
  else
    error = "bad exception: " + e;
}
assertEq(24 + "should throw on Properties == null",error, "typeerror");

error = "before";
try
{
  Object.defineProperties(o, undefined);
}
catch (e)
{
  if (e instanceof TypeError)
    error = "typeerror";
  else
    error = "bad exception: " + e;
}
assertEq(25 + "should throw on Properties == undefined",error, "typeerror");
*/
/******************************************************************************/

print("All tests passed!");
