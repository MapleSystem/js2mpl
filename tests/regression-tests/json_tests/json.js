var str = '[';
for (var i = 0, sz = 21; i < sz; i++)
  str += '0,';
  str += '0]';

  var arr = JSON.parse(str);
  if (arr.length !== 22)
    $ERROR("failed");
for (i = 0; i < 21; i++)
   if (arr[i] != 0)
     $ERROR("elem set failed");
