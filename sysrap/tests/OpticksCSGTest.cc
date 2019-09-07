/*
 * Copyright (c) 2019 Opticks Team. All Rights Reserved.
 *
 * This file is part of Opticks
 * (see https://bitbucket.org/simoncblyth/opticks).
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */


#include <cstring>
#include <iostream>
#include <iomanip>

#include "SYSRAP_LOG.hh"
#include "PLOG.hh"

#include "OpticksCSG.h"

int main(int argc, char** argv)
{
    PLOG_(argc, argv);
    SYSRAP_LOG__ ; 

    for(unsigned i=0 ; i < 100 ; i++)
    {
        OpticksCSG_t type = (OpticksCSG_t)i ; 
        if(!CSGExists(type)) continue ; 

        const char*  name = CSGName( type );

        std::cout 
                   << " type " << std::setw(3) << type
                   << " name " << std::setw(20) << name
                   << std::endl ; 


       /*
        char csgChar = CSGChar( csgName );

        OpticksCSG_t csgFlag2 = CSGFlag(csgChar);
        assert( csgFlag2 == csgFlag );

        const char* csgName2 = CSGChar2Name( csgChar );

        assert(strcmp(csgName, csgName2) == 0);
        assert( csgName == csgName2 );

        std::cout 
                   << " csgFlag " << std::setw(3) << csgFlag 
                   << " csgName " << std::setw(20) << csgName
                   << " csgChar " << std::setw(3) << csgChar
                   << " csgFlag2 " << std::setw(3) << csgFlag2
                   << " csgName2 " << std::setw(20) << csgName2
                   << std::endl ; 

        */

    }


    //int rc = SSys::run("tpmt.py");

    return 0 ; 
} 

