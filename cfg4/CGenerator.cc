#include "Opticks.hh"
#include "OpticksHub.hh"
#include "OpticksCfg.hh"
#include "CG4.hh"

#include "NGLM.hpp"
#include "NPY.hpp"
#include "TorchStepNPY.hpp"
#include "NGunConfig.hpp"

#include "CTorchSource.hh"
#include "CGunSource.hh"

#include "CDetector.hh"
#include "CGenerator.hh"

#include "PLOG.hh"


CGenerator::CGenerator(OpticksHub* hub, CG4* g4)
    :
    m_hub(hub),
    m_ok(m_hub->getOpticks()),
    m_cfg(m_ok->getCfg()),
    m_g4(g4),
    m_source(NULL),
    m_num_g4evt(1),
    m_photons_per_g4evt(0),
    m_gensteps(NULL),
    m_dynamic(true)
{
    init();
}


void CGenerator::init()
{
    unsigned code = m_ok->getSourceCode();
    if(     code == G4GUN) setSource(makeG4GunSource());
    else if(code == TORCH) setSource(makeTorchSource());
    else                   assert(0);
}


void CGenerator::setSource(CSource* source)
{
    m_source = source ; 

   
   // CRecorder* recorder = m_g4->getRecorder(); 
   // m_source->setRecorder(recorder);  
   // SEEMS NOT USED ???  PROBABLY FOR THE AS YET UNIMPLEMENTED PRIMARY RECORDING 
   // CAUSES INIT ORDER ISSUE TOO, AS NEED Generator 
   //
}
CSource* CGenerator::getSource()
{
    return m_source ; 
}


void CGenerator::setNumG4Event(unsigned num)
{
    m_num_g4evt = num ;
}
unsigned CGenerator::getNumG4Event()
{
    return m_num_g4evt ;  
}

void CGenerator::setNumPhotonsPerG4Event(unsigned num)
{
    m_photons_per_g4evt = num ;
}
unsigned CGenerator::getNumPhotonsPerG4Event()
{
    return m_photons_per_g4evt ;  
}

void CGenerator::setGensteps(NPY<float>* gensteps)
{
    m_gensteps = gensteps ; 
}
NPY<float>* CGenerator::getGensteps()
{
    return m_gensteps ; 
}
void CGenerator::setDynamic(bool dynamic)
{
   m_dynamic = dynamic ; 
}
bool CGenerator::isDynamic()
{
    return m_dynamic ; 
}

CSource* CGenerator::makeTorchSource()
{
    LOG(info) << "CGenerator::makeTorchSource " ; 

    TorchStepNPY* torch = m_hub->getTorchstep();

    setGensteps( torch->getNPY() );  // sets the number of photons and preps buffers (unallocated)
    setDynamic(false);
    setNumG4Event( torch->getNumG4Event()); 
    setNumPhotonsPerG4Event( torch->getNumPhotonsPerG4Event()); 

    int verbosity = m_cfg->hasOpt("torchdbg") ? 10 : 0 ; 
    CSource* source  = static_cast<CSource*>(new CTorchSource( torch, verbosity)); 
    return source ; 
}

CSource* CGenerator::makeG4GunSource()
{
    LOG(info) << "CGenerator::makeG4GunSource " ; 
    NGunConfig* gc = new NGunConfig();
    gc->parse(m_cfg->getG4GunConfig());

    CDetector* detector = m_g4->getDetector();

    unsigned int frameIndex = gc->getFrame() ;
    unsigned int numTransforms = detector->getNumGlobalTransforms() ;

    if(frameIndex < numTransforms )
    {
        const char* pvname = detector->getPVName(frameIndex);
        LOG(info) << "CGenerator::makeG4GunSource "
                       << " frameIndex " << frameIndex 
                       << " numTransforms " << numTransforms 
                       << " pvname " << pvname 
                       ;

        glm::mat4 frame = detector->getGlobalTransform( frameIndex );
        gc->setFrameTransform(frame) ;
    }
    else
    {
        LOG(fatal) << "CGenerator::makeG4GunSource gun config frameIndex not in detector"
                   << " frameIndex " << frameIndex
                   << " numTransforms " << numTransforms
                          ;
         assert(0);
    }  

    setDynamic(true);
    setNumG4Event(gc->getNumber()); 
    setNumPhotonsPerG4Event(0); 

    int verbosity = m_cfg->hasOpt("g4gundbg") ? 10 : 0 ; 
    CGunSource* gun = new CGunSource(verbosity) ;
    gun->configure(gc);      

    CSource* source  = static_cast<CSource*>(gun);
    return source ; 
}

