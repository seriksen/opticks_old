#!/bin/bash -l

opticks-

sdir=$(pwd)
name=$(basename $sdir) 
bdir=/tmp/$USER/$name/build 

rm   -rf $bdir
mkdir -p $bdir 
cd $bdir 
pwd 

cmake $sdir \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_INSTALL_PREFIX=$(opticks-prefix) \
      -DCMAKE_MODULE_PATH=$(opticks-home)/cmake/Modules \
      -DCMAKE_PREFIX_PATH=$(opticks-prefix)/externals

make
make install   


