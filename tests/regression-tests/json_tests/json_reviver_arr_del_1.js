var str = "[";
var expected = "";
var expected2 = "";
for (var i = 0; i < 28; i++)
{
   str += "1,";
   if (i === 27)
     {
       expected += "1";
       expected2 += "1";
      }
   if (i === 3)
     expected2 += "17";
   expected += ",";
   expected2 += ",";
}
str += "1]";
var act = JSON.parse(str,
                    function reviver(k, v)
                    {
                     if (k === "" || k === "27")
                       return v;
                     return undefined;
                     }) + "";
print (act);
print (expected)
if (act  !=  expected)
  $ERROR("fail")

