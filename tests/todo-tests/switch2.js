function foo(a)
{
  var day;
  switch(a){
    case "SUN":
	day = "Sunday";
        break;
    case "MON":
        day = "Monday";
        break;
    case "TUE":
        day = "Tuesday";
        break;
    case 3:
        day = 3;
        break;
    default:
        day = "Wrong day";
  }
  return day;
}

var name = foo("SUN");
print(name);
name = foo("3");
print(name);
name = foo(3);
print(name);
