/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          15.7.3.4-2.js
   ECMA Section:       15.7.3.4 Number.NaN
   Description:        All value properties of the Number object should have
   the attributes [DontEnum, DontDelete, ReadOnly]

   this test checks the DontDelete attribute of Number.NaN

   Author:             christine@netscape.com
   Date:               16 september 1997
*/


var SECTION = "15.7.3.4-2";
var VERSION = "ECMA_1";
var TITLE   = "Number.NaN";

print( SECTION + " "+ TITLE);

// CHECK#1
delete( Number.NaN );
if (String(Number.NaN) !== 'NaN'){
  $ERROR('#1: Number.NaN !== NaN. Actual: ' + Number.NaN);
}

// CHECK#2
if (delete( Number.NaN) !== false){
  $ERROR('#2: delete( Number.NaN) !== false. Actual: ' + delete( Number.NaN));
}