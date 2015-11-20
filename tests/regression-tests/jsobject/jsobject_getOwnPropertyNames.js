/*
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/licenses/publicdomain/
 * Contributor:
 *   Jeff Walden <jwalden+code@mit.edu>
 */

//-----------------------------------------------------------------------------


a = [0, 1, 2];

names = Object.getOwnPropertyNames(a);
//expected = ["0", "1", "2", "length"];

if (names[0] !== "length")
  $ERROR("error length");
if (names[1] !== "0")
  $ERROR("error 0");
if (names[2] !== "1")
  $ERROR("error 1");
if (names[3] !== "2")
  $ERROR("error 2");



