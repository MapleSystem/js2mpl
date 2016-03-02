var emptyObject = "{}";
x = JSON.parse(emptyObject);
if(typeof (x) !== "object")
 $ERROR("error")
x = JSON.parse("true");
if (x != true)
  $ERROR("parse boolean true failed");
x = JSON.parse("true          ");
if (x != true)
  $ERROR("parse boolean true failed");

x = JSON.parse("false");
if (x != false)
  $ERROR("parse boolean false failed");

x = JSON.parse("           null           ");
if (x != null)
  $ERROR("parse null failed");

// numbers
x = JSON.parse("12345");
if (x !== 12345)
  $ERROR("parse number 12345 failed");
x = JSON.parse("      23456          \r\r\r\r      \n\n\n\n ");
if(x !== 23456)
  $ERROR("pars number 23456 failed");

// strings
x = JSON.parse('"foo"');
print(x);
if (x != "foo")
  $ERROR("parse string foo failed");

x = JSON.parse('"\\r\\n"');
if (x != "\r\n")
  $ERROR("\r\n failed");
/*
x = JSON.parse('  "\\uabcd\uef4A"');
print(x);
if (x != "\uabcd\uef4A")
  $ERROR("#1 failed");

x = JSON.parse('"\\uabcd"  ');
if (x != "\uabcd")
  $ERROR("#2 failed");
*/
x = JSON.parse('"\\f"');
if (x != "\f")
  $ERROR("#3 failed");
