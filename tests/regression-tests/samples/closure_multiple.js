var inc;
var dec;
var val;
function foo() {
  var counter = 0;
  inc = function() {
    counter++;
    return counter;
  };
  dec = function() {
    counter--;
    return counter;
  };
}

foo();

var x = inc();
inc();
inc();
inc();
var y = dec();
dec();
dec();
var z = dec();

if (x != 1) $ERROR("closure_multiple failed: expect x == 1 but get", x);
if (y != 3) $ERROR("closure_multiple failed: expect y == 3 but get", y);
if (z != 0) $ERROR("closure_multiple failed: expect z == 0 but get", z);
print("pass");
