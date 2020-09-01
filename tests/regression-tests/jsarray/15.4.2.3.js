
// CHECK#1
if ((new   Array() +'') !== ""){
  $ERROR('#1: new   Array() +"" === "". Actual: ' + (new Array()) +"");
}

// CHECK#2
if (typeof new Array() !== "object"){
  $ERROR('#2: typeof new Array() === "object". Actual: ' + typeof new Array());
}

// CHECK#3
var arr = new Array();
arr.getClass = Object.prototype.toString;
if (arr.getClass() !== "[object Array]"){
  $ERROR('#3: var arr = new Array(); arr.getClass = Object.prototype.toString; arr.getClass(). Actual: ' + arr.getClass());
}

// CHECK#4
if ((new Array()).length !== 0){
  $ERROR('#4: (new Array()).length === 0. Actual: ' + (new Array()).length);
}

// CHECK#5
if ((new Array()).toString !== Array.prototype.toString){
  $ERROR('#5: ((new Array()).toString == Array.prototype.toString) === true. Actual: ' + (new Array()).toString == Array.prototype.toString);
}

// CHECK#6
if ((new Array()).join !== Array.prototype.join){
  $ERROR('#6: ((new Array()).join  == Array.prototype.join) === true. Actual: ' + (new Array()).join  == Array.prototype.join);
}

// CHECK#7
if ((new Array()).reverse !== Array.prototype.reverse){
  $ERROR('#7: ((new Array()).reverse == Array.prototype.reverse) === true. Actual: ' + (new Array()).reverse == Array.prototype.reverse);
}

// CHECK#8
if ((new Array()).sort !== Array.prototype.sort){
  $ERROR('#8: ((new Array()).sort  == Array.prototype.sort) === true. Actual: ' + (new Array()).sort  == Array.prototype.sort);
}