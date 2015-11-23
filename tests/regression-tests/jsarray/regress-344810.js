/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 348810;
var summary = 'Do not crash when sorting an array of holes';

print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);

var a = Array(1);
a.sort();