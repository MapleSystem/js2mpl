/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:          instanceof-003.js
   ECMA Section:
   Description:        http://bugzilla.mozilla.org/show_bug.cgi?id=7635

   js> function Foo() {}
   js> theproto = {};
   [object Object]
   js> Foo.prototype = theproto
   [object Object]
   js> theproto instanceof Foo
   true

   I think this should be 'false'


   Author:             christine@netscape.com
   Date:               12 november 1997

   Modified to conform to ECMA3
   https://bugzilla.mozilla.org/show_bug.cgi?id=281606
*/
function Foo() {};
theproto = {};
Foo.prototype = theproto;
//CHECK#1
if (theproto instanceof Foo){
	$ERROR('#1 function Foo() = {}; theproto = {}; Foo.prototype = theproto;theproto instanceof Foo === false');
}