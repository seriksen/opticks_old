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


#include "X4PhysicalVolume.hh"
#include "X4Sample.hh"

#include "OPTICKS_LOG.hh"
#include "Opticks.hh"

class GGeo ; 


int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv);

    char c = argc > 1 ? *argv[1] : 'o' ;  

    G4VPhysicalVolume* top = X4Sample::Sample(c) ; 

    GGeo* ggeo = X4PhysicalVolume::Convert(top) ;   
    assert(ggeo);  

    Opticks* ok = Opticks::GetInstance();
    ok->Summary();

    return 0 ; 
}


