
/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 300858;
var summary = 'Do not crash when sorting array with holes';

print(summary);
var arry = [];
arry[6]  = 'six';
arry[8]  = 'eight';
arry[9] = 'twentyone';
//print(arry.toString())
//print(arry.length)
arry.sort();
//print(arry.toString())
//print(arry.length)
