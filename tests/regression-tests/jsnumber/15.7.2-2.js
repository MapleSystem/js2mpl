// CHECK#6
if ((new Number('string')).constructor !== Number.prototype.constructor){
  $ERROR('#6: (new Number("string")).constructor!== Number.prototype.constructor. Actual: ' + (new Number('string')).constructor);
}
if (typeof (new Number('string')) !== "object"){
  $ERROR('#6: typeof (new Number("string")) !== "object". Actual: ' + typeof (new Number('string')));
}
if (String((new Number('string')).valueOf()) !== 'NaN'){
  $ERROR('#6: (new Number("string")).valueOf() !== Number.NaN. Actual: ' + (new Number('string')).valueOf());
}


// CHECK#7
if ((new Number(new String())).constructor !== Number.prototype.constructor){
  $ERROR('#7: (new Number(new String())).constructor !== Number.prototype.constructor. Actual: ' + (new Number(new String())).constructor);
}
if (typeof (new Number(new String())) !== "object"){
  $ERROR('#7: typeof (new Number(new String())) !== "object". Actual: ' + typeof (new Number(new String())));
}
if ((new Number(new String())).valueOf() !== 0){
  $ERROR('#7:(new Number(new String())).valueOf() !== 0. Actual: ' + (new Number(new String())).valueOf());
}


// CHECK#8
if ((new Number('')).constructor !== Number.prototype.constructor){
  $ERROR('#8: (new Number("")).constructor !== Number.prototype.constructor. Actual: ' + (new Number('')).constructor);
}
if (typeof (new Number('')) !== "object"){
  $ERROR('#8: typeof (new Number("")) !== "object". Actual: ' + typeof (new Number('')));
}
if ((new Number('')).valueOf() !== 0){
  $ERROR('#8: (new Number("")).valueOf() !== 0. Actual: ' + (new Number('')).valueOf());
}


// CHECK#9
if ((new Number(Number.POSITIVE_INFINITY)).constructor !== Number.prototype.constructor){
  $ERROR('#9: (new Number(Number.POSITIVE_INFINITY)).constructor !== Number.prototype.constructor. Actual: ' + (new Number(Number.POSITIVE_INFINITY)).constructor);
}
if (typeof (new Number(Number.POSITIVE_INFINITY)) !== "object"){
  $ERROR('#9: typeof (new Number(Number.POSITIVE_INFINITY)) !== "object". Actual: ' + typeof (new Number(Number.POSITIVE_INFINITY)));
}
if ((new Number(Number.POSITIVE_INFINITY)).valueOf() !== Number.POSITIVE_INFINITY){
  $ERROR('#9: (new Number(Number.POSITIVE_INFINITY)).valueOf() !== Number.POSITIVE_INFINITY. Actual: ' + (new Number(Number.POSITIVE_INFINITY)).valueOf());
}


// CHECK#10
if ((new Number(Number.NEGATIVE_INFINITY)).constructor !== Number.prototype.constructor){
  $ERROR('#10: (new Number(Number.NEGATIVE_INFINITY)).constructor !== Number.prototype.constructor. Actual: ' + (new Number(Number.NEGATIVE_INFINITY)).constructor);
}
if (typeof (new Number(Number.NEGATIVE_INFINITY)) !== "object"){
  $ERROR('#10: typeof (new Number(Number.NEGATIVE_INFINITY)) !== "object". Actual: ' + typeof (new Number(Number.NEGATIVE_INFINITY)));
}
if ((new Number(Number.NEGATIVE_INFINITY)).valueOf() !== Number.NEGATIVE_INFINITY){
  $ERROR('#10: (new Number(Number.NEGATIVE_INFINITY)).valueOf() !== Number.NEGATIVE_INFINITY. Actual: ' + (new Number(Number.NEGATIVE_INFINITY)).valueOf());
}
