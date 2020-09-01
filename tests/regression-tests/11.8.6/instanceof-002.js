/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */


/**
   File Name:
   ECMA Section:
   Description:        Call Objects



   Author:             christine@netscape.com
   Date:               12 november 1997
*/
var b = new Boolean();
//CHECK#1
if (!(b instanceof Boolean)){
	$ERROR('#1 var b = new Boolean(); b instanceof Boolean === true');
}

//CHECK#1
if (!(b instanceof Object)){
	$ERROR('#1 var b = new Boolean(); b instanceof Object === true');
}

//CHECK#1
if (b instanceof Array){
	$ERROR('#1 var b = new Boolean(); b instanceof Array === false');
}

//CHECK#1
if (true instanceof Array){
	$ERROR('#1 true instanceof Boolean === false');
}

//CHECK#1
if (!(Boolean instanceof Object)){
	$ERROR('#1 Boolean instanceof Objectt === true');
}