function foo()
{
    var sum = 0
    var values = [10,9,8,7,6,5,4,3,2,1]
    for( var value in values)
    {
        print(value);
        sum +=values[value]
        print(sum);
    }
    return (sum)
}
 
var x = foo();

if (x === 55)
  print("for-in2 passes");
else $ERROR("for-in2 fails: expect 55 but get ", x);

