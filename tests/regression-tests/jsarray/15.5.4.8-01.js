/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

//-----------------------------------------------------------------------------
var BUGNUMBER = 480096;
var summary = 'Array.lastIndexOf';
var actual = '';
var expect = '';
print(BUGNUMBER);
print(summary);

  actual = 0;
  actual += [2, 3,, 4, 5, 6].lastIndexOf();
  actual += Array.prototype.lastIndexOf.call([2, 3,, 4, 5, 6]);
  actual += Array.prototype.lastIndexOf.apply([2, 3,, 4, 5, 6], [, -4]);
  actual += Array.prototype.lastIndexOf.apply([2, 3,, 4, 5, 6], [undefined, -4]);
  actual += Array.prototype.lastIndexOf.apply([2, 3,, 4, 5, 6], [undefined, -5]);
if (actual !== -5){
    $ERROR('test failed');
}