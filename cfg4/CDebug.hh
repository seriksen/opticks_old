#pragma once

#include <map>
#include <string>
#include <vector>

class G4StepPoint ; 

class Opticks ; 

struct CPhoton ; 
struct CG4Ctx ; 
class CRecorder ; 
class CRec ; 
class CG4 ; 

#include "CFG4_API_EXPORT.hh"

#include "CFG4_PUSH.hh"
#include "CBoundaryProcess.hh"
#include "CStage.hh"
#include "CFG4_POP.hh"

#include "CFG4_HEAD.hh"

class CFG4_API CDebug {
    public:
        CDebug(CG4* g4, const CPhoton& photon, CRecorder* recorder);

        void setMaterialBridge(CMaterialBridge* material_bridge) ;
        void postTrack();
        bool hasIssue();

        bool isSelected(); 
        bool isHistorySelected(); 
        bool isMaterialSelected(); 



    public:

#ifdef USE_CUSTOM_BOUNDARY
        void Collect(const G4StepPoint* point, Ds::DsG4OpBoundaryProcessStatus boundary_status, const CPhoton& photon );
        void dump_point(const G4ThreeVector& origin, unsigned index, const G4StepPoint* point, Ds::DsG4OpBoundaryProcessStatus boundary_status, unsigned flag, const char* matname );
#else
        void Collect(const G4StepPoint* point, G4OpBoundaryProcessStatus boundary_status, const CPhoton& photon );
        void dump_point(const G4ThreeVector& origin, unsigned index, const G4StepPoint* point, G4OpBoundaryProcessStatus boundary_status, unsigned flag, const char* matname );
#endif
        void Clear();


        // reporting
        void report(const char* msg="CDebug::report");
        void addSeqhisMismatch(unsigned long long rdr, unsigned long long rec);
        void addSeqmatMismatch(unsigned long long rdr, unsigned long long rec);
        void addDebugPhoton(int photon_id);

        void dump(const char* msg="CDebug::dump");
        void dump_brief(const char* msg="CDebug::dump_brief");
        void dump_sequence(const char* msg="CDebug::dump_sequence");
        void dump_points(const char* msg="CDebug::dump_points");
        void dumpStepVelocity(const char* msg="CDebug::dumpStepVelocity");


         std::string desc() const ;

    private:
        CG4*     m_g4 ; 
        CG4Ctx&  m_ctx ;  

        Opticks*         m_ok ; 
        int              m_verbosity ; 
        CRecorder*       m_recorder ; 
        CRec*            m_crec ; 
        CMaterialBridge* m_material_bridge  ; 
        const CPhoton&   m_photon ; 


        unsigned long long m_seqhis_select ; 
        unsigned long long m_seqmat_select ; 

        unsigned long long m_dbgseqhis ;
        unsigned long long m_dbgseqmat ;
        bool               m_dbgflags ;

        bool               m_posttrack_dbgzero ;
        bool               m_posttrack_dbgseqhis ;
        bool               m_posttrack_dbgseqmat ;
        bool               m_posttrack_dump ;



        std::vector<const G4StepPoint*>         m_points ; 
        std::vector<unsigned>                   m_flags ; 
        std::vector<unsigned>                   m_materials ; 
        std::vector<double>                     m_times  ; 

        std::vector<unsigned long long>         m_seqhis_dbg  ; 
        std::vector<unsigned long long>         m_seqmat_dbg  ; 
        std::vector<unsigned>                   m_mskhis_dbg  ; 
        std::vector<std::pair<unsigned long long, unsigned long long> > m_seqhis_mismatch ; 
        std::vector<std::pair<unsigned long long, unsigned long long> > m_seqmat_mismatch ; 
        std::vector<int> m_debug_photon ; 


#ifdef USE_CUSTOM_BOUNDARY
        std::vector<Ds::DsG4OpBoundaryProcessStatus>  m_bndstats ; 
#else
        std::vector<G4OpBoundaryProcessStatus>  m_bndstats ; 
#endif




};

#include "CFG4_TAIL.hh"
 
