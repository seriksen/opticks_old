#ifndef LXeGeneralPhysics_h
#define LXeGeneralPhysics_h 1

#include "globals.hh"
#include "G4ios.hh"

#include "G4VPhysicsConstructor.hh"

class LXeGeneralPhysics : public G4VPhysicsConstructor
{
  public:

    LXeGeneralPhysics(const G4String& name = "general");
    virtual ~LXeGeneralPhysics();

    // This method will be invoked in the Construct() method.
    // each particle type will be instantiated
    virtual void ConstructParticle();
 
    // This method will be invoked in the Construct() method.
    // each physics process will be instantiated and
    // registered to the process manager of each particle type
    virtual void ConstructProcess();

};

#endif
