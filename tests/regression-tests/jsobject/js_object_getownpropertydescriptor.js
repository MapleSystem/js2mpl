  var o = {foo:1};
  var desc = Object.getOwnPropertyDescriptor(o,"foo");
  if(desc.value !== 1)
    $ERROR("getOwnPropertyDescriptor failed 1");
  if(desc.set !== undefined)
    $ERROR("getOwnPropertyDescriptor failed 2");
  if(desc.get !== undefined)
    $ERROR("getOwnPropertyDescriptor failed 3");
  if(desc.writable !== true)
    $ERROR("getOwnPropertyDescriptor failed 4");
  if(desc.enumerable !== true)
    $ERROR("getOwnPropertyDescriptor failed 5");
  if(desc.configurable !== true) {
    print(desc.configureable);
    $ERROR("getOwnPropertyDescriptor failed 6");
  }

  
