#pragma once

#include <string>
#include "G4Types.hh"

class OpticksHub ; 
class NLookup ; 
template <typename T> class NPY ;

/**
CCollector
==================== 

Gensteps have an item shape of 6*4 (ie 6 quads) the 
first 3 quads are common between scintillation and
Cerenkov but the last 3 differ.

Furthermore the precise details of which quanties are included 
in the last three quads depends on the inner photon loop 
implementation details of the scintillation and Cerenkov processes.
They need to be setup whilst developing a corresponding GPU/OptiX 
implementation of the inner photon loop and doing 
equivalence comparisons.

Each implementation will need slightly different genstep and OptiX port.

Effectively the genstep can be regarded as the "stack" just 
prior to the photon loop.
 
**/

#include "CFG4_API_EXPORT.hh"

class CFG4_API CCollector 
{
         friend class OKG4Mgr ; 
         friend class CG4 ; 
   public:
         static CCollector* Instance();
   public:
         CCollector(OpticksHub* hub);
   public:
         NPY<float>*  getGensteps();
   public:
         std::string description();
         void Summary(const char* msg="CCollector::Summary");
         int translate(int acode);
   private:
         void setGensteps(NPY<float>* gs);
         void consistencyCheck();
         void postinit();
   public:
         void collectScintillationStep(
            G4int                id, 
            G4int                parentId,
            G4int                materialId,
            G4int                numPhotons,
            
            G4double             x0_x,  
            G4double             x0_y,  
            G4double             x0_z,  
            G4double             t0, 

            G4double             deltaPosition_x, 
            G4double             deltaPosition_y, 
            G4double             deltaPosition_z, 
            G4double             stepLength, 

            G4int                pdgCode, 
            G4double             pdgCharge, 
            G4double             weight, 
            G4double             meanVelocity, 

            G4int                scntId,
            G4double             slowerRatio,
            G4double             slowTimeConstant,
            G4double             slowerTimeConstant,

            G4double             scintillationTime,
            G4double             scintillationIntegrationMax,
            G4double             spare1=0,
            G4double             spare2=0
        );
   public:
         void collectCerenkovStep(
            G4int                id, 
            G4int                parentId,
            G4int                materialId,
            G4int                numPhotons,
            
            G4double             x0_x,  
            G4double             x0_y,  
            G4double             x0_z,  
            G4double             t0, 

            G4double             deltaPosition_x, 
            G4double             deltaPosition_y, 
            G4double             deltaPosition_z, 
            G4double             stepLength, 

            G4int                pdgCode, 
            G4double             pdgCharge, 
            G4double             weight, 
            G4double             meanVelocity, 

            G4double             betaInverse,
            G4double             pmin,
            G4double             pmax,
            G4double             maxCos,

            G4double             maxSin2,
            G4double             meanNumberOfPhotons1,
            G4double             meanNumberOfPhotons2,
            G4double             spare2=0
        );


   private:
         static CCollector* INSTANCE ;      
   private:
         OpticksHub*  m_hub ; 
         NLookup*     m_lookup ; 
         NPY<float>*  m_onestep ;
         float*       m_values ;  
         NPY<float>*  m_genstep ;
         unsigned     m_scintillation_count ; 
         unsigned     m_cerenkov_count ; 


};