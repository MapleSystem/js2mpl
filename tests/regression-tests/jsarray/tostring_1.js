// |reftest| skip -- obsolete test
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          tostring_1.js
   ECMA Section:       Array.toString()
   Description:

   This checks the ToString value of Array objects under JavaScript 1.2.

   Author:             christine@netscape.com
   Date:               12 november 1997
*/

var SECTION = "JS1_2";
var TITLE   = "Array.toString()";

print( SECTION + " "+ TITLE);

var a = new Array();
if(a.toString() !== "")
  $ERROR("#1: var a = new Array(); a.toString()", "", a.toString());

a[0] = void 0;
if(a.toString() !== "")
  $ERROR("#2: a[0] = void 0; a.toString()", "", a.toString());

if(a.length !== 1)
  $ERROR("#2: a.length", 1, a.length);

a[1] = void 0;
if(a.toString() !== ",")
  $ERROR("#3: a[1] = void 0; a.toString()", ",", a.toString());

a[1] = "hi";
if(a.toString() !== ",hi" )
  $ERROR("#4: a[1] = \"hi\"; a.toString()", ",hi", a.toString());

a[2] = void 0;
if(a.toString() !== ",hi," )
  $ERROR("#5: a[2] = void 0; a.toString()", ",hi,", a.toString());

var b = new Array(10);
var bstring = "";
for ( blen=0; blen<9; blen++) {
  bstring += ",";
}

if(b.toString() !== bstring )
  $ERROR("#6: var b = new Array(1000); b.toString()", bstring, b.toString());

if(b.length !== 10 )
  $ERROR("#6: var b = new Array(1000); b.toString()", 10, b.length);