var inc;
var dec;
var val;
function foo() {
  var counter = 0;
  inc = function() {
    counter++;
  };
  dec = function() {
    counter--;;
  };
  val = function() {
    return counter;
  };
}

foo();

var x = val();
inc();
inc();
var y = val();
dec();
var z = val();

if (x != 0) $ERROR("closure_multiple1 failed: expect x == 0 but get", x);
if (y != 2) $ERROR("closure_multiple1 failed: expect y == 2 but get", y);
if (z != 1) $ERROR("closure_multiple1 failed: expect z == 1 but get", z);
print("pass");
