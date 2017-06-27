#!/bin/bash

# Build with gcc/g++
JS2MPLROOT=`pwd`
MAPLEROOT=$JS2MPLROOT/../mapleall

BUILD_CMAKE_DIR=build_cmake

function do_build()
{
  local BUILD_DIR=$1
  local CMAKE_OPTIONS=$2
  local nthrs=$3
  mkdir -p $BUILD_DIR
  cd $BUILD_DIR
  cmake $JS2MPLROOT $CMAKE_OPTIONS
  make -j $nthrs
  cd $JS2MPLROOT
}

function do_install()
{
  flavor=$1
  arch=$2
  srcdir=$3
  tgtdir=
  if [ $arch = 64 ];
  then
    tgtdir=build${arch}/$flavor
  else
    tgtdir=build/$flavor
  fi

  /bin/mkdir -p $tgtdir
  /bin/cp $srcdir/bin/js2mpl ${tgtdir}
}

nthrs=4
if [ "x$1" = "x-j" ];
then
  nthrs=$2
fi

gnu_targets="32"

GNU_BUILD=$BUILD_CMAKE_DIR/gnu/build
for arch in $gnu_targets;
do
  builddir=$GNU_BUILD
  if [ $arch = 64 ];
  then
    builddir="${builddir}${arch}"
  fi
  echo "Build mozjs"
  (cd ../mozjs; make -j $nthrs > /dev/null )
  echo "Build js2mpl ${arch}"
  do_build $builddir   "-DDEX=0 -DHOST_ARCH=${arch}" $nthrs
  echo "Install js2mpl ${arch}"
  do_install gnu $arch $builddir
done
