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
               (msg ? ": " + msg : "") + (stack ? "\nStack:\n" + stack : "");
      }
}

assertEq(JSON.parse('[1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]',
                        function revive0(k, v)
                        {
                          if (k === "")
                             return v;
                          return undefined;
                        }) + "",
          ",,,,,,,,,,,,,,,,,,,");


Array.prototype[3] = 17;

/* Now, with a property on the prototype chain, it'll show through. */
assertEq(JSON.parse('[1,2,3,4,5,6,7,8,9,0,1,2,3,4,5,6,7,8,9,0]',
                     function revive(k, v)
                     {
                       if (k === "")
                         return v;
                       return undefined;
                     }) + "",
         ",,,17,,,,,,,,,,,,,,,,");


