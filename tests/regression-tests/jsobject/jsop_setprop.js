var x = new Object;
x.a = 1;
x.b = 2;
if (x.a !== 1)
  $ERROR("typeof failed: expect x.a == 1 but get", x.a);
if (x.b !== 2)
  $ERROR("typeof failed: expect x.b == 2 but get", x.b);
x.a = "abc";
if (x.a !== "abc")
  $ERROR("typeof failed: expect x.a == abc but get", x.a);
