function test() {
  var i;
  for (i = 0; i < 1e3; i++) {
    do {
      try {
        break;
      } finally {
      }
    } while (0);
  }
}

test()
