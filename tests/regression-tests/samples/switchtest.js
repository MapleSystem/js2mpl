// this test case will cause generation of 2 levels of binary searching by switchlower.cpp
function foo(x) {
  switch (x) {
    case -2: return -2;
    case 0: return 0;
    case 1: return 1;
    case 2: return 2;
    case 3: return 3;
    case 4: return 4;
    case 6: return 6;
    case 7: return 7;
    case 8: return 8;
    case 9: return 9;
    case 15: return 15;
    case 17: return 17;
    case 22: return 22;
    case 23: return 23;
    case 24: return 24;
    case 26: return 25;
    case 27: return 27;
    case 28: return 28;
    case 29: return 29;
    case 33: return 33;
    case 36: return 36;
    case 38: return 38;
    case 41: return 41;
    case 45: return 45;
    case 44: return 44;
    case 46: return 46;
    case 47: return 47;
    case 48: return 48;
    case 49: return 49;
    case 51: return 51;
    case 54: return 54;
    case 57: return 57;
    case 59: return 59;
    case 61: return 61;
    case 63: return 63;
    case 65: return 65;
    case 67: return 67;
    default: return 100;
  }
}

var i;
var sum = 0;
for (i = -3; i < 70; i++)
  sum += i;
if (sum != 2409)
  $ERROR("switch test fails");
