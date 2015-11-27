/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor: Bob Clary
 */

//-----------------------------------------------------------------------------
var BUGNUMBER = 424954;
var summary = 'Do not crash with [].concat(null)';

print ('BUGNUMBER: ' + BUGNUMBER);
print(summary);
 
[].concat(null);