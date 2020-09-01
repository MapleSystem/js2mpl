var v=0;
try{
  try{
    throw "ex1";
  }
  finally{
    try{
      v=1;
      throw "ex2";
    }
    catch(er1){
      if (er1==="ex1") $ERROR('#1: catch previous catched exception');
      if (er1!=="ex2") $ERROR('#2: expect exception "ex2". but get '+er1);
      v++;
    }
    finally{
      v*=2;
    }
  }
}
catch(er2){
  if (er2==="ex2") $ERROR('#3: original catched exception lost');
  if (er2!=="ex1") $ERROR('#4: expect exception "ex1". but get '+er2);
}
if (v!==4) $ERROR('#4: finally block must be evaluated');

