#!/bin/bash -l

sdir=$(pwd)
name=$(basename $sdir)
bdir=/tmp/$USER/opticks/examples/$name/build 
idir=$HOME
# /tmp is not same on GPU cluster gateway and nodes, so cannot install there 

pfx=$(opticks-release-prefix)
echo pfx $pfx

rm -rf $bdir 
if [ ! -d "$bdir" ]; then 
   mkdir -p $bdir && cd $bdir 
   cmake3 $sdir \
     -DCMAKE_BUILD_TYPE=Debug \
     -DCMAKE_PREFIX_PATH="$pfx/externals;$pfx" \
     -DCMAKE_MODULE_PATH=$pfx/cmake/Modules \
     -DCMAKE_INSTALL_PREFIX=$idir \
     -DGeant4_DIR=$(opticks-envg4-Geant4_DIR)
else
   cd $bdir 
fi 

pwd
make
rc=$?
[ "$rc" != "0" ] && exit $rc 

make install   


cat << EOX

# how to run example

unset OPTICKS_GEOCACHE_PREFIX 

# for simple geometry tests must unset the geocache prefix
# which will cause the default to be used ~/.opticks
# otherwise will see permission errors on attempting 
# to write into the shared cache

export PATH=$HOME/lib:$PATH
exe=$(which $name)
echo $exe

$exe

EOX




