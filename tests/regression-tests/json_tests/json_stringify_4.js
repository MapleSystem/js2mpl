JSON.stringify(new Boolean(false), function(k, v) { 
   if (typeof(v) != "object")
     $ERROR("#1:error");
});

if (Boolean.prototype.hasOwnProperty('toJSON') != false)
  $ERROR("#2:error");

Object.prototype.toJSON = function() { return 2; };
if (JSON.stringify(new Boolean(true)) != "2")
  $ERROR("#3:error");
