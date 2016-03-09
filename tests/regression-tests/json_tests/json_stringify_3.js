function A() {};
A.prototype = [];

function B() {};
B.prototype = new A();

function C() {};
C.prototype = new B();

function D() {};
D.prototype = new C();

function E() {};
E.prototype = new D();

function f() {
        var o = new B();
        for (var i=0; i<10; i++)
                o[i] = i;

        var expected = '{"0":0,"1":1,"2":2,"3":3,"4":4,"5":5,"6":6,"7":7,"8":8,"9":9}';
//        print(JSON.stringify(o));
        if(JSON.stringify(o) != expected)
          $ERROR("#1 error");

        var o = new A();
        for (var i=0; i<10; i++)
                o[i] = i;

        if(JSON.stringify(o) != expected)
          $ERROR("#2 error");

        var o = new D();
        for (var i=0; i<10; i++)
                o[i] = i;
        if(JSON.stringify(o) != expected)
          $ERROR("#3 error");

        var o = new E();
        for (var i=0; i<10; i++)
                o[i] = i;
        if(JSON.stringify(o) != expected)
          $ERROR("#4 error");

}
f();

