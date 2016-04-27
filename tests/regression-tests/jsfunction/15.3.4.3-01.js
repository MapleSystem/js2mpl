/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 562448;
var summary = 'Function.prototype.apply should accept any arraylike arguments';
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

function fun() { }

var global = this;

var thisObj = {}; 

/*
 * NB: There was an erratum removing the steps numbered 5 and 7 in the original
 *     version of ES5; see also the comments in js_fun_apply.
 */

/* Step 5. */
var called = false;
var argsObjectLength =
  { length: { valueOf: function() { called = true; return 17; } } };

fun.apply({}, argsObjectLength);
assertEq("should have been set in valueOf called via ToUint32", called, true);

var upvar = "unset";
var argsObjectPrimitiveLength =
  {
    length:
      {
        valueOf: function() { upvar = "valueOf"; return {}; },
        toString: function()
        {
          upvar = upvar === "valueOf" ? "both" : "toString";
          return 17;
        }
      }
  };
fun.apply({}, argsObjectPrimitiveLength);
assertEq("didn't call all hooks properly", upvar, "both");


/* Step 6-9. */
var seenThis, res, steps;
var argsAccessors =
  {
    length: 4,
    get 0() { steps.push("0"); return 1; },
    get 1() { steps.push("1"); return 2; },
    // make sure values shine through holes
    get 3() { steps.push("3"); return 8; },
  };

Object.prototype[2] = 729;

seenThis = "not seen";
function argsAsArray(arg1, arg2, arg3, arg4)
{
  seenThis = this;
  var arguments = [arg1, arg2, arg3, arg4];
  return Array.prototype.map.call(arguments, function (v) { return v; });
}

steps = [];
res = argsAsArray.apply(thisObj, argsAccessors);
assertEq("saw wrong this", seenThis, thisObj);
assertEq("wrong steps: " + steps, steps.length, 3);
assertEq("bad step 0", steps[0], "0");
assertEq("bad step 1", steps[1], "1");
assertEq("bad step 3", steps[2], "3");

assertEq("wrong return: " + res, res.length, 4);
assertEq("wrong ret[0]", res[0], 1);
assertEq("wrong ret[0]", res[1], 2);
assertEq("wrong ret[0]", res[2], 729);
assertEq("wrong ret[0]", res[3], 8);