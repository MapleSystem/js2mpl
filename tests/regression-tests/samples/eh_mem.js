function test() {
  var i;
  for (i = 0; i < 1e3; i++) {
    try {
      continue;
    } finally {
    }
  }
}

test()
