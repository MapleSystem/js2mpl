/* ccalllib.c
extern "C" {
  int foo(int a, int b, int c) {
    return a + b + c;
  }
}
*/

func = "foo";
var v = ccall(func, 1, 2, 3);

if (v != 6) $ERROR("failed: expect 6 but get",  v);
print("pass");

