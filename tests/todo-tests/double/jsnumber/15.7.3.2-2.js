/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.3.2-2.js
   ECMA Section:       15.7.3.2 Number.MAX_VALUE
   Description:        All value properties of the Number object should have
   the attributes [DontEnum, DontDelete, ReadOnly]

   this test checks the DontDelete attribute of Number.MAX_VALUE

   Author:             christine@netscape.com
   Date:               16 september 1997
*/

var SECTION = "15.7.3.2-2";
var VERSION = "ECMA_1";
var TITLE =  "Number.MAX_VALUE:  DontDelete Attribute";

print( SECTION + " "+ TITLE);

// CHECK#1
delete( Number.MAX_VALUE );
if (Number.MAX_VALUE !== 1.7976931348623157e308){
  $ERROR('#1: Number.MAX_VALUE !== 1.7976931348623157e308. Actual: ' + Number.MAX_VALUE);
}

// CHECK#2
var a = delete( Number.MAX_VALUE );
if (a !== false){
  $ERROR('#2: delete( Number.MAX_VALUE) !== false. Actual: ' + a);
}