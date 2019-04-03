// TEST=CCerenkovGeneratorTest om-t

#include "OPTICKS_LOG.hh"

#include "SSys.hh"
#include "NPY.hpp"
#include "GGeo.hh"
#include "GBndLib.hh"

#include "OpticksGenstep.hh"
#include "CCerenkovGenerator.hh"
#include "C4PhotonCollector.hh"

#include "Opticks.hh"
#include "OpticksMode.hh"
#include "OpticksHub.hh"
#include "CMaterialLib.hh"
#include "CAlignEngine.hh"

/**
CCerenkovGeneratorTest
========================

1. loads gensteps from a direct geocache identified by OPTICKS_KEY envvar 
2. CPU generates photons from the gensteps and saves them back into the 
   geocache at tests/CCerenkovGeneratorTest/so.npy


Example usage:: 

    ckm-gentest () 
    { 
        OPTICKS_KEY=$(ckm-key) $(ckm-dbg) CCerenkovGeneratorTest --natural --envkey
    }

    [blyth@localhost cfg4]$ ckm-key
    CerenkovMinimal.X4PhysicalVolume.World.792496b5e2cc08bdf5258cc12e63de9f

See ckm-vi for comparisons

**/

int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv); 

    Opticks ok(argc, argv);
    ok.setModeOverride( OpticksMode::CFG4_MODE );  // override the typical COMPUTE/INTEROP , how used ?

    OpticksHub hub(&ok) ; 
    CMaterialLib* clib = new CMaterialLib(&hub);
    clib->convert();

    // CMaterialLib::convert creates G4Materials which occupy the G4MaterialsTable 
    // that CCerenkovGenerator::GetRINDEX uses
    //
    // Note that this is loading the entire geometry just to get at the refractive index.
    // If this turns out to be inconvenient could implement finer gained persisting.
    // 
    //
    // below needs to be done after Opticks::configure for setup of the event spec

    NPY<float>* np = ok.hasKey() ? ok.loadDirectGenstep() : NULL ;   
    if(np == NULL) return 0 ; 

    OpticksGenstep* gs = new OpticksGenstep(np) ; 
    unsigned modulo = 1000 ; 
    unsigned margin = 10 ;   
    gs->dump( modulo, margin ) ; 


    // KEYDIR is an internal "envvar", see boosttrap/BFile.cc,  
    // that gets resolved to BResource::Get("idpath"). 
    // The simstreamdir is used for RNG logging and storing the 
    // generated photons.

    const char* simstreamdir = "$KEYDIR/tests/CCerenkovGeneratorTest" ;
    CAlignEngine::Initialize( simstreamdir );  

    unsigned idx = 0 ;  
    G4VParticleChange* pc = CCerenkovGenerator::GeneratePhotonsFromGenstep(gs,idx) ;

    C4PhotonCollector* collector = new C4PhotonCollector ; 
    collector->collectSecondaryPhotons( pc, idx ); 

    NPYBase::SetNPDump(true);
    collector->savePhotons(simstreamdir, "so.npy") ; 
    NPYBase::SetNPDump(false);

    LOG(info) << collector->desc() ;

    //SSys::npdump( ph_path, "np.float32", "", "suppress=True") ;  

    return 0 ; 
}

