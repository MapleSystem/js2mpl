/* ccalllib.c
extern "C" {
  int foo(int a, int b, int c) {
    return a + b + c;
  }
}
*/

var v = ccall("foo", 1, 2, 3);
func = "foo";
var w = ccall(func, v, 10, 100)

if (v != 6) $ERROR("failed: expect 6 but get",  v);
if (w != 116) $ERROR("failed: expect 116 but get",  w);
print("pass");

