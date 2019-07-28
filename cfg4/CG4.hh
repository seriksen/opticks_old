#pragma once

#include <string>
#include <map>

// g4-
class G4RunManager ; 
class G4VisManager ; 
class G4UImanager ; 
class G4UIExecutive ; 
class G4VUserDetectorConstruction ;

class SLog ; 

// npy-
template <typename T> class NPY ; 

// cfg4-


class CRayTracer ; 
class CPhysics ; 
class CGeometry ; 
class CMaterialLib ; 
class CDetector ; 
class CMaterialBridge ; 
class CSurfaceBridge ; 
class CGenerator ; 

class CGenstepCollector ; 
class CPrimaryCollector ; 
class CRecorder ; 
class CStepRec ; 
class CRandomEngine ; 
class CRandomListener ; 

class CRunAction ; 
class CEventAction ; 
class CPrimaryGeneratorAction ;
class CTrackingAction ; 
class CSteppingAction ; 
class CSensitiveDetector ; 

class OpticksHub ; 
class OpticksRun ; 
class OpticksEvent ; 
class Opticks ; 
template <typename T> class OpticksCfg ;

/**

CG4
====

Canonical instance m_g4 is resident of OKG4Mgr and is instanciated 
with it for non "--load" option running.

Prime method CG4::propagate is invoked from OKG4Mgr::propagate


Whats the difference between CRecorder/m_recorder and CStepRec/m_steprec ?
-------------------------------------------------------------------------------

CStepRec 
   records non-optical particle steps into the m_nopstep buffer of the
   OpticksEvent set by CStepRec::initEvent
    
CRecorder
   records optical photon steps and photon tracks

CStepRec is beautifully simple, CRecorder is horribly complicated in comparison


Workflow overview
-------------------

Traditional GPU Opticks simulation workflow:

* gensteps (Cerenkov/Scintillation) harvested from Geant4
  and persisted into OpticksEvent

* gensteps seeded onto GPU using Thrust, summation over photons 
  to generate per step provide photon and record buffer 
  dimensions up frount 

* Cerenkov/Scintillation on GPU generation and propagation      
  populate the pre-sized GPU record buffer 

This works because all gensteps are available before doing 
any optical simulation. BUT when operating on CPU doing the 
non-optical and optical simulation together, do not know the 
photon counts ahead of time.

**/

#define CG4UniformRand(file, line) CG4::INSTANCE->flat_instrumented((file), (line))

#include "plog/Severity.h"
#include "CG4Ctx.hh"

#include "CFG4_API_EXPORT.hh"

class CFG4_API CG4 
{


        friend class CGeometry ;  // for setUserInitialization
   public:
        static const plog::Severity LEVEL ; 
        static CG4* INSTANCE ; 
   public:
        CG4(OpticksHub* hub);
        void interactive();
        void cleanup();
        bool isDynamic(); // true for G4GUN without gensteps ahead of time, false for TORCH with gensteps ahead of time
   public:
        NPY<float>* propagate();
   private:
        void postinitialize();
        void postpropagate();
   public:
        int getPrintIndex() const ;

        void postStep();
        void preTrack();
        void postTrack();
   public:
        const std::map<std::string, unsigned>& getMaterialMap() const ;        
   private:
        int preinit();
        void init();
        void initialize();
        void setUserInitialization(G4VUserDetectorConstruction* detector);
        void execute(const char* path);
        void initEvent(OpticksEvent* evt);
        void snap();
   public:
       // from CRecorder
        unsigned long long getSeqHis() const ;
        unsigned long long getSeqMat() const ;
   public:
        Opticks*         getOpticks() const ;
        OpticksHub*      getHub() const ;
        OpticksRun*      getRun() const;
        CRandomEngine*   getRandomEngine() const ; 
        CGenerator*      getGenerator() const ;
        CRecorder*       getRecorder() const ;
        CStepRec*        getStepRec() const ;
        CGeometry*       getGeometry() const ;
        CMaterialBridge* getMaterialBridge() const ;
        CSurfaceBridge*  getSurfaceBridge() const ;
        CMaterialLib*    getMaterialLib() const ;
        CDetector*       getDetector() const ;
        double           getCtxRecordFraction() const ;  // ctx is updated at setTrackOptical
   public:
        CEventAction*       getEventAction() const ;
        CSteppingAction*    getSteppingAction() const ;
        CTrackingAction*    getTrackingAction() const ;
        CSensitiveDetector* getSensitiveDetector() const ;
   public:
        NPY<float>*      getGensteps() const ;
   public:
        double           flat_instrumented(const char* file, int line); 
        CG4Ctx&          getCtx();

   private:
        SLog*                 m_log ; 
        OpticksHub*           m_hub ; 
        Opticks*              m_ok ; 
        int                   m_preinit ; 
        OpticksRun*           m_run ; 
        OpticksCfg<Opticks>*  m_cfg ; 

        CG4Ctx                m_ctx ;       

        CRandomEngine*        m_engine ; 
        //CRandomListener*        m_engine ; 

        CPhysics*             m_physics ; 
        G4RunManager*         m_runManager ; 
        CSensitiveDetector*   m_sd ; 
        CGeometry*            m_geometry ; 
        bool                  m_hookup ; 
        CMaterialLib*         m_mlib ; 
        CDetector*            m_detector ; 
        CGenerator*           m_generator ; 
        bool                  m_dynamic ; 
   private:
        CGenstepCollector*           m_collector ; 
        CPrimaryCollector*    m_primary_collector ; 
        CRecorder*            m_recorder ; 
        CStepRec*             m_steprec ; 
   private:
        G4VisManager*         m_visManager ; 
        G4UImanager*          m_uiManager ; 
        G4UIExecutive*        m_ui ; 
   private:

        CPrimaryGeneratorAction*       m_pga ; 
        CSteppingAction*               m_sa ;
        CTrackingAction*               m_ta ;
        CRunAction*                    m_ra ;
        CEventAction*                  m_ea ;
        CRayTracer*                    m_rt ; 

        bool                           m_initialized ; 
};

