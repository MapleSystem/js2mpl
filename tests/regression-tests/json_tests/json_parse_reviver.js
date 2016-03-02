
function doubler(k, v)
{
  if (typeof (v) == "number")
    return 2 * v;

  return v;
}

var x = JSON.parse('{"a":5,"b":6}', doubler);
//var x = JSON.parse('{"a":5,"b":6}');

if (x.hasOwnProperty('a') != true)
  $ERROR("#1:error");
if (!x.hasOwnProperty('b'))
  $ERROR("#2:error");
print(x.a);
//print(x.b);

if (x.a !== 10)
  $ERROR("#3:error");
if (x.b !== 12)
  $ERROR("#4:error")

x = JSON.parse('[3, 4, 5]', doubler);
if (x[0] !== 6)
  $ERROR("#5:error")
if (x[1] !== 8)
  $ERROR("#6:error")
if (x[2] !== 10)
  $ERROR("#7:error") 

// make sure reviver isn't called after a failed parse
/*
var called = false;
function dontCallMe(k, v)
{
          called = true;
}

try
{
  JSON.parse('{{{{{{{}}}}', dontCallMe);
  throw new Error("didn't throw?");
}
catch (e)
{
  assertEq(e instanceof SyntaxError, true, "wrong exception: " + e);
}
assertEq(called, false);
*/
