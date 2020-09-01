var a = {foo:function(p1, p2){return p1+p2}}
var b = a.foo(11,12)

if (b == 23 ){
  print(" pass\n");
} else {
  $ERROR("test failed expect 23 but get",  b, "\n");
}


