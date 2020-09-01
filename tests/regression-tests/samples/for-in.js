function foo()
{
    var sum = 0
    var values = [10,9,8,7,6,5,4,3,2,1]
    for( var value in values)
    {
        sum +=value
    }
    return (sum)
}
 
var y = foo()
 
if (y === "00123456789") {
  print(" for-in: pass\n");
} else {
  $ERROR("for-in failed: expect y == 00123456789 but get ", y);
}
