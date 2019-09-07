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

#include "BOpticksKey.hh"
#include "OPTICKS_LOG.hh"

int main( int argc, char** argv )
{
    OPTICKS_LOG_COLOR__(argc, argv ); 

    const char* spec = "X4PhysicalVolumeTest.X4PhysicalVolume.World.3ad454e0990085f20c4689fce16c0819" ; 

    BOpticksKey::SetKey(spec); 

    BOpticksKey* key = BOpticksKey::GetKey(); 
    LOG(info) << key->desc() ; 


    const char* exename = PLOG::instance->args.exename() ; 
    std::cout << " exename " << exename << std::endl ; 


    return 0 ; 
}
