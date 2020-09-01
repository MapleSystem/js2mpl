var x = Array.prototype.concat(Object.freeze([{}]));
print(x[0]);
if (JSON.stringify(x[0]) != "{}")
  $ERROR("error")

