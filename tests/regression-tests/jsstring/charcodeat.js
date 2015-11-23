var a = new String("aaabbcfdg");
if (a.charCodeAt(5) !== 99) {
  $ERROR('#2: String.charCodeAt(pos) failed');
}
/*
if (a.charCodeAt(-1) !== Number.NaN)) { 
  $ERROR('#2: String.charCodeAt(pos) failed');
}*/
