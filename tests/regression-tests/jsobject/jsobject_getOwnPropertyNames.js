/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------


a = [0, 1, 2];

names = Object.getOwnPropertyNames(a).sort();
expected = ["0", "1", "2", "length"].sort();

if(names.toString() !== expected.toString())
  $ERROR("error getOwnPropertyNames");
if (names.indexOf("length") < 0)
  $ERROR("error length");
if (names.indexOf("0") < 0)
  $ERROR("error 0");
if (names.indexOf("1") < 0)
  $ERROR("error 1");
if (names.indexOf("2") < 0)
  $ERROR("error 2");