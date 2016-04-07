// memleak.js
function trigger(obj) {
  if (obj.epoch == "aaaaa")
    return;
  $ERROR("error 1");
}

function aa1() {
    for (var i = 0; i < 5; i++) {
    trigger({epoch:"aaaaa"});
  }
}

for (var i = 0; i < 5; i++) {
  aa1();
}


