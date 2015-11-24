function foo() {
  var counter = 0;
  function changeBy(val) {
    counter += val;
  }
  return {
    increment: function() {
      changeBy(1);
    },
    decrement: function() {
      changeBy(-1);
    },
    value: function() {
      return counter;
    }
  };   
}

var a = foo();
var x = a.value();
a.increment();
a.increment();
var y = a.value();
a.decrement();
var z = a.value();

if (x != 0) $ERROR("closure_multiple3 failed: expect x == 0 but get", x);
if (y != 2) $ERROR("closure_multiple3 failed: expect y == 2 but get", y);
if (z != 1) $ERROR("closure_multiple3 failed: expect z == 1 but get", z);
print("pass");
