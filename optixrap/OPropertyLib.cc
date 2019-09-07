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

#include "NPY.hpp"
#include "NGPU.hpp"
#include "OPropertyLib.hh"

#include "PLOG.hh"


OPropertyLib::OPropertyLib(optix::Context& ctx, const char* name) 
    : 
    m_context(ctx),
    m_name(name ? strdup(name) : NULL)
{
}

void OPropertyLib::dumpVals( float* vals, unsigned int n)
{
    for(unsigned int i=0 ; i < n ; i++)
    { 
        std::cout << std::setw(10) << vals[i]  ;
        if(i % 16 == 0 ) std::cout << std::endl ; 
    }
}

void OPropertyLib::upload(optix::Buffer& optixBuffer, NPY<float>* buffer)
{
    unsigned int numBytes = buffer->getNumBytes(0) ;
    void* data = buffer->getBytes();
    memcpy( optixBuffer->map(), data, numBytes );
    optixBuffer->unmap(); 

    NGPU::GetInstance()->add(numBytes, m_name, "OPropLib", "OScene" );   

}

//
// NB this requires the memory layout of the optixBuffer needed for the texture of (nx,ny) shape
//    matches that of the NPY<float> buffer
//   
//    this was working for NPY<float> of shape    (128, 4, 39, 4)    
//         going into texture<float4>   nx:39  ny:128*4 boundaries*species
//
//    but for (128, 4, 39, 8)  it did not working 
//    as depends on the 39 being the last dimension of the buffer 
//    excluding the 4 that disappears as payload
//
//    maybe use
//             (128, 4, 39*2, 4 )
//    
//


