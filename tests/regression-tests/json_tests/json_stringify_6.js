function assertEq(a, e, msg)
{
        function SameValue(v1, v2)
        {
                if (v1 === 0 && v2 === 0)
                        return 1 / v1 === 1 / v2;
                if (v1 !== v1 && v2 !== v2)
                        return true;
                return v1 === v2;
        }

        if (!SameValue(a, e))
        {
                var stack = new Error().stack || "";
                throw "Assertion failed: got " + a + ", expected " + e +
                        (msg ? ": " + msg : "") +
                        (stack ? "\nStack:\n" + stack : "");
        }
}

function redefine(obj, prop, fun)
{
          var desc =
                      { value: fun, writable: true, configurable: true, enumerable: false };
            Object.defineProperty(obj, prop, desc);
}

assertEq(JSON.stringify(new Boolean(false)), "false");

assertEq(JSON.stringify(new Number(5)), "5");

assertEq(JSON.stringify(new String("foopy")), '"foopy"');


var numToString = Number.prototype.toString;
var numValueOf = Number.prototype.valueOf;
var objToString = Object.prototype.toString;
var objValueOf = Object.prototype.valueOf;
var boolToString = Boolean.prototype.toString;
var boolValueOf = Boolean.prototype.valueOf;

redefine(Boolean.prototype, "toString", function() { return 17; });
assertEq(JSON.stringify(new Boolean(false)), "false")
delete Boolean.prototype.toString;
assertEq(JSON.stringify(new Boolean(false)), "false");
delete Object.prototype.toString;
assertEq(JSON.stringify(new Boolean(false)), "false");
delete Boolean.prototype.valueOf;
assertEq(JSON.stringify(new Boolean(false)), "false");
delete Object.prototype.valueOf;
assertEq(JSON.stringify(new Boolean(false)), "false");


redefine(Boolean.prototype, "toString", boolToString);
redefine(Boolean.prototype, "valueOf", boolValueOf);
redefine(Object.prototype, "toString", objToString);
redefine(Object.prototype, "valueOf", objValueOf);

redefine(Number.prototype, "toString", function() { return 42; });
assertEq(JSON.stringify(new Number(5)), "5");
redefine(Number.prototype, "valueOf", function() { return 17; });
assertEq(JSON.stringify(new Number(5)), "17");

redefine(Number.prototype, "toString", numToString);
redefine(Number.prototype, "valueOf", numValueOf);
redefine(Object.prototype, "toString", objToString);
redefine(Object.prototype, "valueOf", objValueOf);


redefine(String.prototype, "valueOf", function() { return 17; });
assertEq(JSON.stringify(new String(5)), '"5"');
redefine(String.prototype, "toString", function() { return 42; });
assertEq(JSON.stringify(new String(5)), '"42"');
