// CHECK#3
try{
  throw "ex1";
}
catch(er1){
  if (er1!=="ex1") $ERROR('#3.1: Exception ==="ex1". Actual:  Exception ==='+ er1  );
}
finally{
  try{
    throw "ex2";
  }
  catch(er1){
    if (er1==="ex1") $ERROR('#3.2: Exception !=="ex1". Actual: catch previous embedded exception');
    if (er1!=="ex2") $ERROR('#3.3: Exception ==="ex2". Actual:  Exception ==='+ er1  );
  }
}
