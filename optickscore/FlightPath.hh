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

#pragma once

#include <string>

class SCtrl ; 


// npy-
template<typename T> class NPY ; 

// opticks-
class InterpolatedView ; 

/**
FlightPath
============

Canonical m_flighpath instance is resident of OpticksHub
and is instanciated in OpticksHub::configureState following m_bookmarks.

Note that FlightPath can in principal be used in a pure compute 
manner with no OpenGL involvement, eg for making pure raytrace movies
on headless nodes without OpenGL capability.


**/

#include "OKCORE_API_EXPORT.hh"
#include "OKCORE_HEAD.hh"

class OKCORE_API FlightPath {
public:
    static const char* FILENAME ; 
    FlightPath(const char* dir);
    std::string description(const char* msg="FlightPath");
    void Summary(const char* msg="FlightPath::Summary");
public:
    unsigned getNumViews() const ;
    void setCtrl(SCtrl* ctrl); 
public:
    void setVerbose(bool verbose=true);
    void setInterpolatedViewPeriod(unsigned int ivperiod); 
    void refreshInterpolatedView();
    InterpolatedView* getInterpolatedView();
private:
    void load();
    InterpolatedView* makeInterpolatedView();
public:
    int* getIVPeriodPtr();
private:
    const char*                          m_flightpathdir ; 
    NPY<float>*                          m_flightpath ;  
    InterpolatedView*                    m_view ;  
    bool                                 m_verbose ; 
    int                                  m_ivperiod ; 
    SCtrl*                               m_ctrl ; 

};


#include "OKCORE_TAIL.hh"

