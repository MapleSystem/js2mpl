// CHECK#7
var c7=0;
try{
  try{
    throw "ex1";
  }
  finally{
    try{
      c7=1;
      throw "ex2";
    }
    catch(er1){
      if (er1!=="ex2") $ERROR('#7.1: Exception === "ex2". Actual:  Exception ==='+er1);
      if (er1==="ex1") $ERROR('#7.2: Exception !=="ex2". Actual: catch previous catched exception');
      c7++;
    }
    finally{
      c7*=2;
    }
  }
}
catch(er1){
  if (er1!=="ex1") $ERROR('#7.3: Exception === "ex1". Actual:  Exception ==='+er1);
}
if (c7!==4) $ERROR('#7.4: "finally" block must be evaluated');

