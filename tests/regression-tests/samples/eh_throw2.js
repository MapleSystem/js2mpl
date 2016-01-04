// CHECK#4
try{
  try{
    throw "ex0";
  }
  finally{
    throw "ex1";
  }
}
catch(err){
  if (err!=="ex1") $ERROR('failed: except "ex1" but get: ', err );
}
