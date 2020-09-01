function foo()
{
    var i = 0
    var x = 0
    for (i = 0; i < 1000; i++)
    {
        x = x + i
    }
    return (x)
}

var y = foo()

if (y == 499500) {
  print(" for: pass\n");
} else {
  $ERROR("for failed: expect y == 499500 but get y=", y);
}
