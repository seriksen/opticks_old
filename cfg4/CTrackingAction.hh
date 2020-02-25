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

/**
CTrackingAction : G4 to CG4 interface for tracks
==================================================

* CG4Ctx::setTrack at pre 
* CG4::preTrack CG4::postTrack for optical 

**/

#include <string>
#include "G4TrackStatus.hh"
#include "G4UserTrackingAction.hh"
#include "plog/Severity.h"
#include "globals.hh"

class G4Event ; 

struct CG4Ctx ; 

class Opticks ; 
class CG4 ; 

#include "CFG4_API_EXPORT.hh"
#include "CFG4_HEAD.hh"

class CFG4_API CTrackingAction : public G4UserTrackingAction
{
    static const plog::Severity LEVEL ; 
    friend class CG4 ;  
  public:
    virtual ~CTrackingAction();
  private:
    CTrackingAction(CG4* g4);
    void postinitialize();
  public:
    std::string brief(); 
 public:
    virtual void  PreUserTrackingAction(const G4Track* track);
    virtual void PostUserTrackingAction(const G4Track* track);
  private:
    void setTrack(const G4Track* track);
    void dump(const char* msg );
  private:
    CG4*                  m_g4 ; 
    CG4Ctx&               m_ctx ; 
    Opticks*              m_ok ; 

};

#include "CFG4_TAIL.hh"

