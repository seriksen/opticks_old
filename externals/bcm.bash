bcm-src(){      echo externals/bcm.bash ; }
bcm-source(){   echo ${BASH_SOURCE:-$(opticks-home)/$(bcm-src)} ; }
bcm-vi(){       vi $(bcm-source) ; }
bcm-usage(){ cat << EOU

BCM : Boost CMake Modules
===========================

This provides cmake modules that can be re-used by boost and other
dependencies. It provides modules to reduce the boilerplate for installing,
versioning, setting up package config, and creating tests.


Initially looked at:

* https://github.com/boost-cmake/bcm

But it seems development has moved to 

* https://github.com/BoostCMake/bcm

Docs:

* https://readthedocs.org/projects/bcm/
* http://bcm.readthedocs.io/en/latest/


Very clear explanation describing a standalone CMake setup for building boost_filesystem

* http://bcm.readthedocs.io/en/latest/src/Building.html#building-standalone-with-cmake


Discussion by the author : Paul Fultz II

* http://boost.2283326.n4.nabble.com/Boost-Cmake-Modules-td4690949.html


* https://github.com/pfultz2/cget
* http://pfultz2.com/blog/
* http://cget.readthedocs.io/en/latest/src/intro.html


How close is BCM to being endorsed by Boost ? Unclear 
-----------------------------------------------------------

* https://lists.boost.org/Archives/boost/2017/09/238679.php

2017-09-20

Hi, 
So the Daniel Pfeifer has agreed to be the review manager for the Boost Cmake 
Modules. I would like ask for an endorsement so we can schedule a formal 
review for BCM. 
Thanks, 
Paul 


* https://lists.boost.org/Archives/boost/2017/09/238696.php
* https://lists.boost.org/Archives/boost/2017/09/index.php


* https://groups.google.com/forum/#!topic/boost-developers-archive/kBiU5eli28U


bcm_auto_export
----------------

* https://github.com/boost-cmake/bcm/blob/master/share/bcm/cmake/BCMExport.cmake


Analysing project dependencies
-------------------------------



FUNCTIONS
-----------

bcm--
   clones bcm from github, configures, "builds" and installs the share/bcm/cmake/ 
   directory contents such as BCMDeploy.cmake 
   beneath opticks-prefix (typically /usr/local/opticks) 

   share/bcm/cmake/BCMDeploy.cmake



EOU
}

bcm-env(){  olocal- ; opticks- ; }
bcm-view-(){ ls -1 $(opticks-prefix)/share/bcm/cmake/* ; }
bcm-view(){ vim -R $($FUNCNAME-) ; }
bcm-info(){ cat << EOI

    url  : $(bcm-url)
    dist : $(bcm-dist)
    base : $(bcm-base)
    dir  : $(bcm-dir)
    bdir : $(bcm-bdir)
    idir : $(bcm-idir)


EOI
}



bcm-url(){ 
  case $NODE_TAG in 
      E) echo git@github.com:simoncblyth/bcm.git ;; 
      *) echo http://github.com/simoncblyth/bcm.git ;; 
  esac
}   

bcm-base(){   echo $(opticks-prefix)/externals/bcm ; }
bcm-prefix(){ echo $(opticks-prefix)/externals ; }
bcm-idir(){   echo $(bcm-prefix) ; }

bcm-dir(){  echo $(bcm-base)/bcm ; }
bcm-bdir(){ echo $(bcm-base)/bcm.build ; }

bcm-ecd(){  cd $(bcm-edir); }
bcm-cd(){   cd $(bcm-dir)/$1 ; }
bcm-bcd(){  cd $(bcm-bdir); }
bcm-icd(){  cd $(bcm-idir); }

bcm-ccd(){  cd $(bcm-base)/bcm/share/bcm/cmake ; }


bcm-get(){
   local iwd=$PWD
   local dir=$(dirname $(bcm-dir)) &&  mkdir -p $dir && cd $dir
   if [ ! -d "bcm" ]; then 
       git clone $(bcm-url)
   fi 
   cd $iwd
}

bcm-nuclear(){ cd $(opticks-prefix)/externals && rm -rf bcm ; }

bcm-wipe(){
  local bdir=$(bcm-bdir)
  rm -rf $bdir 
}

bcm-cmake(){
  local iwd=$PWD
  local bdir=$(bcm-bdir)
  mkdir -p $bdir
  bcm-bcd
  cmake $(bcm-dir) -DCMAKE_INSTALL_PREFIX=$(bcm-prefix) 
  cd $iwd
}

bcm-configure()
{
   bcm-wipe
   bcm-cmake $*
}

bcm-make(){
  local iwd=$PWD
  bcm-bcd
  cmake --build . --target ${1:-install}
  cd $iwd
}

bcm--(){
  bcm-get 
  bcm-cmake
  bcm-make install
}


bcm-deploy-review(){ cat << EOR

BCMPkgConfig
    defines target properties INTERFACE_DESCRIPTION, INTERFACE_URL, INTERFACE_PKG_CONFIG_REQUIRES, bcm_auto_pkgconfig 
    quite informative as serializes targets in familiar pc format   

BCMInstallTargets
    defines bcm_install_targets

bcm_deploy
    
    * TARGET_NAME = EXPORT_NAME ?  EXPORT_NAME : NAME 

    * creates alias lib eg Opticks::NPY for target NPY  


share/bcm/cmake/BCMDeploy.cmake (my fork)::

     01 
      2 include(BCMPkgConfig)
      3 include(BCMInstallTargets)
      4 include(BCMExport)
      5 
      6 function(bcm_deploy)
      7     set(options SKIP_HEADER_INSTALL)
      8     set(oneValueArgs NAMESPACE COMPATIBILITY)
      9     set(multiValueArgs TARGETS INCLUDE)
     10 
     11     cmake_parse_arguments(PARSE "${options}" "${oneValueArgs}" "${multiValueArgs}"  ${ARGN})
     12 
     13     if(PARSE_SKIP_HEADER_INSTALL)
     14         bcm_install_targets(TARGETS ${PARSE_TARGETS} INCLUDE ${PARSE_INCLUDE} SKIP_HEADER_INSTALL)
     15     else()
     16         bcm_install_targets(TARGETS ${PARSE_TARGETS} INCLUDE ${PARSE_INCLUDE} )
     17     endif()
     18 
     19     bcm_auto_pkgconfig(TARGET ${PARSE_TARGETS})
     20     bcm_auto_export(TARGETS ${PARSE_TARGETS} NAMESPACE ${PARSE_NAMESPACE} COMPATIBILITY ${PARSE_COMPATIBILITY})
     21     
     22     foreach(TARGET ${PARSE_TARGETS})
     23         get_target_property(TARGET_NAME ${TARGET} EXPORT_NAME)
     24         if(NOT TARGET_NAME)
     25             get_target_property(TARGET_NAME ${TARGET} NAME)
     26         endif()
     27         set(EXPORT_LIB_TARGET ${PARSE_NAMESPACE}${TARGET_NAME})
     28         if(NOT TARGET ${EXPORT_LIB_TARGET})
     29             add_library(${EXPORT_LIB_TARGET} ALIAS ${TARGET})
     30         endif()
     31         set_target_properties(${TARGET} PROPERTIES INTERFACE_FIND_PACKAGE_NAME ${PROJECT_NAME})
     32         if(COMMAND bcm_add_rpath)
     33             get_target_property(TARGET_TYPE ${TARGET} TYPE)
     34             if(NOT "${TARGET_TYPE}" STREQUAL "INTERFACE_LIBRARY")
     35                 bcm_add_rpath("$<TARGET_FILE_DIR:${TARGET}>")
     36             endif()
     37         endif()
     38         bcm_shadow_notify(${EXPORT_LIB_TARGET})
     39         bcm_shadow_notify(${TARGET})
     40     endforeach()
     41 
     42 endfunction()




EOR
}

bcm-install-targets-review(){ cat << EOR

bcm_install_targets
=====================

* does target_include_directories for INTERFACE only, thus 
  populating INTERFACE_INCLUDE_DIRECTORIES property of the target.
  This is only concerned with usage : not building  

* setup install of each of the INCLUDE dirs


EXPORT   
    name-of-export-file (no way to change that from bcm_deploy)


Original BCM had hardcoded assumptions of header layout with src headers
all in an include dir and installed headers all co-mingled.
My fork fixes these problems.

NB when using bcm_deploy no need for the below as thats done by bcm_install_targets::

    install(TARGETS ${name} DESTINATION ${CMAKE_INSTALL_LIBDIR})


share/bcm/cmake/BCMInstallTargets.cmake::

      01 include(GNUInstallDirs)
      2 
      3 function(bcm_install_targets)
      4     set(options SKIP_HEADER_INSTALL)
      5     set(oneValueArgs EXPORT)
      6     set(multiValueArgs TARGETS INCLUDE)
      7 
      8     cmake_parse_arguments(PARSE "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
      9 
     10     string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)
     11     set(EXPORT_FILE ${PROJECT_NAME_LOWER}-targets)
     12     if(PARSE_EXPORT)
     13         set(EXPORT_FILE ${PARSE_EXPORT})
     14     endif()
     15     
     16     set(BIN_INSTALL_DIR ${CMAKE_INSTALL_BINDIR})
     17     set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})
     18     set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})
     19     
     20     foreach(TARGET ${PARSE_TARGETS})
     21         foreach(INCLUDE ${PARSE_INCLUDE})
     22             get_filename_component(INCLUDE_PATH ${INCLUDE} ABSOLUTE)
     23             target_include_directories(${TARGET} INTERFACE $<BUILD_INTERFACE:${INCLUDE_PATH}>)
     24         endforeach()
     25         target_include_directories(${TARGET} INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${INCLUDE_INSTALL_DIR}>)
     26     endforeach()
     27         
     28         
     29     if(NOT PARSE_SKIP_HEADER_INSTALL)
     30         message(STATUS "bcm_install_targets.proceed-with-install ${INCLUDE_INSTALL_DIR}  ")
     31         foreach(INCLUDE ${PARSE_INCLUDE})
     32             install(DIRECTORY ${INCLUDE}/ DESTINATION ${INCLUDE_INSTALL_DIR})
     33         endforeach()
     34     endif() 
     35                 
     36     install(TARGETS ${PARSE_TARGETS}
     37             EXPORT ${EXPORT_FILE}
     38             RUNTIME DESTINATION ${BIN_INSTALL_DIR}
     39             LIBRARY DESTINATION ${LIB_INSTALL_DIR}
     40             ARCHIVE DESTINATION ${LIB_INSTALL_DIR})
     41 
     42 endfunction()
     43 





EOR
}



bcm-test-review(){ cat << EOR

bcm-test-review
===================

Looks to be tied to boost tests ?



EOR
}


