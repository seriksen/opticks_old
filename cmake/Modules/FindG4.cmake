
set(G4_MODULE "${CMAKE_CURRENT_LIST_FILE}")
set(G4_PREFIX "${CMAKE_INSTALL_PREFIX}/externals")

find_package(Geant4 CONFIG QUIET)   

set(G4_FOUND ${Geant4_FOUND})

# parse header to yield version integer at configure time
if(G4_FOUND)
   set(_dirs ${Geant4_INCLUDE_DIRS}) 
   list(GET _dirs 0 _firstdir)
   file(READ "${_firstdir}/G4Version.hh" _contents)
   string(REGEX REPLACE "\n" ";" _contents "${_contents}")
   foreach(_line ${_contents})
        if (_line MATCHES "#define G4VERSION_NUMBER[ ]+([0-9]+)$")
            set(Geant4_VERSION_INTEGER ${CMAKE_MATCH_1})
            #message(STATUS "FindG4.cmake:_line ${_line} ===> ${CMAKE_MATCH_1} ") 
        endif()
   endforeach()
else()
   message(STATUS "FindG4.cmake:Geant4 NOT-FOUND ")
endif()


if(Geant4_FOUND)
  set(G4_DIR         ${Geant4_DIR})
  set(G4_VERSION     ${Geant4_VERSION})
  set(G4_VERSION_INTEGER ${Geant4_VERSION_INTEGER})
  set(G4_INCLUDE_DIR ${Geant4_INCLUDE_DIR})
  set(G4_INCLUDE_DIRS ${Geant4_INCLUDE_DIRS})
  set(G4_LIBRARIES   ${Geant4_LIBRARIES})
  set(G4_DEFINITIONS ${Geant4_DEFINITIONS})

  set(_targets)
  foreach(_lib ${Geant4_LIBRARIES})
     set(_loc "${_lib}-NOTFOUND" ) # https://cmake.org/pipermail/cmake/2007-February/012966.html     
     find_library( _loc 
          NAMES ${_lib}
          PATHS ${G4_PREFIX}/lib )

     if(_loc)
         set(_tgt "Opticks::${_lib}")
         #message(STATUS "${_tgt} ${_loc} ") 
         add_library(${_tgt}  UNKNOWN IMPORTED) 
         set_target_properties(${_tgt} PROPERTIES IMPORTED_LOCATION "${_loc}")
         list(APPEND _targets ${_tgt})
     else()
         message(FATAL_ERROR "failed to locate expected lib ${_lib}")
     endif()
  endforeach()  

  string(REGEX REPLACE "-D" "" _defs "${Geant4_DEFINITIONS}")
  #message(STATUS "_defs ${_defs} ") 

  add_library(Opticks::G4 INTERFACE IMPORTED)

  set_target_properties(Opticks::G4  PROPERTIES INTERFACE_FIND_PACKAGE_NAME "G4 MODULE REQUIRED")

  ## Above target_properties INTERFACE_FIND_PACKAGE_NAME kludge tees up the arguments 
  ## to find_dependency in BCM generated exports such as /usr/local/opticks-cmake-overhaul/lib/cmake/useg4/useg4-config.cmake
  ## so downstream targets will automatically do the required find_dependency, see::
  ##  
  ##     examples/UseG4  
  ##     examples/UseUseG4  
  ##     examples/UseBoost
  ##     examples/UseUseBoost
  ##
  ## NB INTERFACE_FIND_PACKAGE_NAME is a BCM defined property, not a standard one, see bcm-

  target_include_directories(Opticks::G4 INTERFACE "${Geant4_INCLUDE_DIRS}" )
  target_link_libraries(Opticks::G4 INTERFACE "${_targets}" ) 
  target_compile_definitions(Opticks::G4 INTERFACE "${_defs}" )

  # https://cmake.org/cmake/help/v3.3/prop_tgt/INTERFACE_LINK_LIBRARIES.html
  # https://cmake.org/cmake/help/v3.3/prop_tgt/INTERFACE_COMPILE_DEFINITIONS.html

endif()


if(G4_VERBOSE)
  message(STATUS "G4_MODULE          : ${G4_MODULE} " )
  message(STATUS "G4_PREFIX          : ${G4_PREFIX} " )
  message(STATUS "G4_FOUND           : ${G4_FOUND} " )
  message(STATUS "G4_DIR             : ${G4_DIR} " )
  message(STATUS "G4_VERSION         : ${G4_VERSION} " )
  message(STATUS "G4_VERSION_INTEGER : ${G4_VERSION_INTEGER} " )
  message(STATUS "G4_INCLUDE_DIR     : ${G4_INCLUDE_DIR} " )
  message(STATUS "G4_INCLUDE_DIRS    : ${G4_INCLUDE_DIRS} " )
  message(STATUS "G4_LIBRARIES       : ${G4_LIBRARIES} " )
  message(STATUS "G4_DEFINITIONS     : ${G4_DEFINITIONS} " )
endif()

