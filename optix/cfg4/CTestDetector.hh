#pragma once

#include <map>
#include <string>

// ggeo-
class GCache ;
class GGeoTestConfig ; 
class GMaterial ;
class GCSG ; 

// cfg4-
class CMaker ; 
class CPropLib ; 

// g4-
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4VSolid;

//
// Hmm for fully controlled cfg4- testing need 
// to construct composite boolean G4 geometries for things like PMTs
//  
// Maybe GParts specs not appropriate level to start from for that ? 
// Need a higher level ? detdesc ?
//
// Parsing detdesc is a shortterm DYB specific kludge, the analytic geometry info 
// should eventually live withing the G4DAE exported file (GDML style). 
// Given this, the approach for making G4 PMTs can be a kludge too. 
// Does not need to be very general, probably will only every use for PMTs,
// as this approach too much effort for full geometries.
//  
// See:  
//       pmt-ecd
//           python detdesc parsing into parts buffer
//
//       GMaker::makeZSphereIntersect    
//           converts high level params (two sphere radii and z offsets) of convex lens
//           into parts  
//
//  Possible approach:
//       during the pmt- python partitioning to create the parts buffer 
//       write a sidecar buffer of high level params, that can be used for G4 boolean CSG
//       


#include "CDetector.hh"

class CTestDetector : public CDetector
{
 public:
    CTestDetector(GCache* cache, GGeoTestConfig* config);
  private:
    void init();
  public:
    virtual G4VPhysicalVolume* Construct();
    virtual ~CTestDetector();
  public:
    bool isPmtInBox();
    bool isBoxInBox();
  private:
    void makePMT(G4LogicalVolume* mother);
    void kludgePhotoCathode();
    G4LogicalVolume* makeLV(GCSG* csg, unsigned int i);

  private:
    GGeoTestConfig*    m_config ; 
    CMaker*            m_maker ; 

};



inline CTestDetector::CTestDetector(GCache* cache, GGeoTestConfig* config)
  : 
  CDetector(cache),
  m_config(config),
  m_maker(NULL)
{
    init();
}


inline CTestDetector::~CTestDetector()
{
}

