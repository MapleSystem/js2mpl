// CHECK#4
var c=0;
try{
  try{
    c = 1;
    throw "ex1";
  }
  catch(er1){
    c += 10;
    try{
      c += 100;
      throw "ex2";
    }
    finally{
      c += 1000;
      throw "ex3";
    }
  }
  finally{
    c += 10000;
  }
}
catch(er2){
  if (er2!=="ex3") $ERROR('check1: expecting "ex3", but get '+er2);
}
if (c!==11111) $ERROR('check2: "finally" block must be evaluated');
