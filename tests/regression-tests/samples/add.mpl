flavor 1
srclang 2
id 65535
numfuncs 2
entryfunc &main
func &main () i32
func &Add (var %_this dynany, var %par1 dynany, var %par2 dynany) dynany
var $Add_obj_ dynany
var $v1 dynany
var $const_chars_6 <[15] u8> = [0x0, 0x0, 0xb, 0x0, ' ', 'a', 'd', 'd', ':', ' ', 'p', 'a', 's', 's', 0xa]
var $const_chars_8 <[35] u8> = [0x0, 0x0, 0x1f, 0x0, 't', 'e', 's', 't', ' ', 'f', 'a', 'i', 'l', 'e', 'd', ' ', 'v', '1', ' ', 'e', 'x', 'p', 'e', 'c', 't', ' ', '3', ' ', 'b', 'u', 't', ' ', 'g', 'e', 't']
var $const_chars_9 <[5] u8> = [0x0, 0x0, 0x1, 0x0, 0xa]
func &Add (var %_this dynany, var %par1 dynany, var %par2 dynany) dynany {
  var %sum dynany
  var %temp_var_4 dynany
  #LINE 3:   var sum;
  #LINE 4:   sum = par1 + par2;
  intrinsiccallassigned JSOP_ADD (dread dynany %par1, dread dynany %par2) { dassign %temp_var_4 0 }
  dassign %sum 0 (dread dynany %temp_var_4)
  #LINE 5:   return(sum)
  return (dread dynany %sum)
  return (constval i32 0x0)
}
func &main () i32 {
  var %temp_var_3 dynany
  var %temp_var_5 dynany
  var %print dynany
  var %temp_var_7 dynany
  var %$ERROR dynany
  var %temp_var_10 dynany
  intrinsiccallassigned JS_INIT_CONTEXT () {}
  #LINE 1: function Add(par1, par2)
  intrinsiccallassigned JS_NEW_FUNCTION (addroffunc ptr &Add, constval i32 0x0, constval u32 0x20204) { dassign %temp_var_3 0 }
  dassign $Add_obj_ 0 (dread dynany %temp_var_3)
  #LINE 8: var v1 = Add(1, 2);
  callassigned &Add (constval dynundef 0x0, constval dyni32 0x400000001, constval dyni32 0x400000002) { dassign %temp_var_5 0 }
  dassign $v1 0 (dread dynany %temp_var_5)
  #LINE 10: if (v1 ==3 ){
  brfalse @label1 (eq u1 dynany (dread dynany $v1, constval dyni32 0x400000003))
  #LINE 11:   print(" add: pass\n");
  intrinsiccallassigned JS_PRINT (cvt dynstr simplestr (addrof simplestr $const_chars_6)) { dassign %temp_var_7 0 }
  goto @label2
  #LINE 13:   $ERROR("test failed v1 expect 3 but get",  v1, "\n");
@label1   intrinsiccallassigned JS_ERROR (
    cvt dynstr simplestr (addrof simplestr $const_chars_8),
    dread dynany $v1,
    cvt dynstr simplestr (addrof simplestr $const_chars_9)) { dassign %temp_var_10 0 }
@label2   return (constval i32 0x0)
}
