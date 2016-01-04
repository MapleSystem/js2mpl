// CHECK#4
try{
  try{
    throw "ex1";
  }
  finally{
  }
}
catch(err){
  if (err!=="ex1") $ERROR('failed: except "ex1" but get: ', err );
}
