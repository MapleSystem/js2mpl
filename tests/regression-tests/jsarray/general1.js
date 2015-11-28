/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   Filename:     general1.js
   Description:  'This tests out some of the functionality on methods on the Array objects'

   Author:       Nick Lerissa
   Date:         Fri Feb 13 09:58:28 PST 1998
*/

var SECTION = 'As described in Netscape doc "Whats new in JavaScript 1.2"';
var VERSION = 'no version';
var TITLE = 'String:push,unshift,shift';

print('Executing script: general1.js');
print( SECTION + " "+ TITLE);

var array1 = [];

array1.push(123);            //array1 = [123]
array1.push("dog");          //array1 = [123,dog]
array1.push(-99);            //array1 = [123,dog,-99]
array1.push("cat");          //array1 = [123,dog,-99,cat]

if(array1.pop() !== 'cat')
  $ERROR("#1: array1.pop()", array1.pop(),'cat');

//array1 = [123,dog,-99]
array1.push("mouse");        //array1 = [123,dog,-99,mouse]
if(array1.shift() !== 123)
  $ERROR("#2: array1.shift()", array1.shift(),123);
  
//array1 = [dog,-99,mouse]
array1.unshift(96);          //array1 = [96,dog,-99,mouse]
if(String([96,"dog",-99,"mouse"]) !== String(array1))
  $ERROR("@3: array1.unshift(96)", String([96,"dog",-99,"mouse"]), String(array1));

if(array1.length !== 4)
  $ERROR("#4: array1.length", array1.length,4);

array1.shift();              //array1 = [dog,-99,mouse]
array1.shift();              //array1 = [-99,mouse]
array1.shift();              //array1 = [mouse]

if(array1.shift() !== "mouse")
  $ERROR("#5: array1.shift()", array1.shift(),"mouse");

if(array1.shift() !== undefined)
  $ERROR("#6: array1.shift()", array1.shift(),"undefined");