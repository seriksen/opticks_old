#include <sstream>

#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4LogicalVolume.hh"

#include "BStr.hh"

#include "GVector.hh"
#include "GProperty.hh"
#include "GPropertyMap.hh"
#include "GOpticalSurface.hh"
#include "GSurfaceLib.hh"
#include "GSurLib.hh"
#include "GSur.hh"

#include "CMPT.hh"
#include "CSurLib.hh"
#include "CDetector.hh"

#include "PLOG.hh"



G4OpticalSurfaceModel Model(unsigned model_)
{
   // materials/include/G4OpticalSurface.hh
    G4OpticalSurfaceModel model = unified ;
    switch(model_)
    {
       case 0:model = glisur   ; break; 
       case 1:model = unified  ; break; 
       case 2:model = LUT      ; break; 
       case 3:model = dichroic ; break; 
    }
    return model ; 
}   
G4OpticalSurfaceFinish Finish(unsigned finish_)
{
   // materials/include/G4OpticalSurface.hh
    G4OpticalSurfaceFinish finish = polished ;
    switch(finish_)
    {
        case 0:finish = polished ;break;
        case 3:finish = ground   ;break;
    }
    return finish ; 
}
G4SurfaceType Type(unsigned type_)
{
    // materials/include/G4SurfaceProperty.hh
    G4SurfaceType type = dielectric_dielectric ;
    switch(type_)
    {
        case 0:type = dielectric_metal      ;break;
        case 1:type = dielectric_dielectric ;break;
    }
    return type ; 
}





CSurLib::CSurLib(GSurLib* surlib) 
   :
   m_surlib(surlib),
   m_surfacelib(surlib->getSurfaceLib()),
   m_detector(NULL)
{
}


void CSurLib::setDetector(CDetector* detector)
{
    assert(m_detector == NULL);
    m_detector = detector ; 
}


std::string CSurLib::brief()
{
    std::stringstream ss; 

    ss << "CSurLib " 
       << " num CSur " << m_surlib->getNumSur()
       << " --> "
       << " numBorderSurface " << m_border.size() 
       << " numSkinSurface " << m_skin.size() 
       ;

    return ss.str();
}

// called by CGeometry::init for CGDMLDetector case
void CSurLib::convert(CDetector* detector)
{
    setDetector(detector);
    unsigned numSur = m_surlib->getNumSur();
    LOG(info) << "CSurLib::convert  numSur " << numSur  ;   
    for(unsigned i=0 ; i < numSur ; i++)
    {   
        GSur* sur = m_surlib->getSur(i);
        G4OpticalSurface* os = makeOpticalSurface(sur);

        if(sur->isBorder()) 
        {
             unsigned nvp = sur->getNumVolumePair();
             for(unsigned ivp=0 ; ivp < nvp ; ivp++) 
             {
                 G4LogicalBorderSurface* lbs = makeBorderSurface(sur, ivp, os);
                 m_border.push_back(lbs);
             }
        }
        else if(sur->isSkin())
        {
             unsigned nlv = sur->getNumLV();
             for(unsigned ilv=0 ; ilv < nlv ; ilv++) 
             {
                 G4LogicalSkinSurface* lss = makeSkinSurface(sur, ilv, os);
                 m_skin.push_back(lss);
             }
        } 
        else
        {
              LOG(warning) << "CSurLib::convert SKIPPED GSur index " << i << " : " << sur->brief() ;  
        }
    }   
    LOG(info) << brief();
}


G4OpticalSurface* CSurLib::makeOpticalSurface(GSur* sur)
{
    GPropertyMap<float>* pmap = sur->getPMap();
    GOpticalSurface* os_ = pmap->getOpticalSurface();
    G4OpticalSurface* os = new G4OpticalSurface(os_->getName());
    guint4 optical = os_->getOptical();

    os->SetModel(Model(1));
    os->SetFinish(Finish(optical.z));
    os->SetType(Type(optical.y));

    G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
    os->SetMaterialPropertiesTable(mpt);

    addProperties(mpt,  pmap);

    return os ; 
}


G4LogicalBorderSurface* CSurLib::makeBorderSurface(GSur* sur, unsigned ivp, G4OpticalSurface* os)
{
    GPropertyMap<float>* pmap = sur->getPMap();
    const char* name = pmap->getName() ;
    guint4 pair = sur->getVolumePair(ivp);
    unsigned ipv1 = pair.x ; 
    unsigned ipv2 = pair.y ; 
    assert(pair.w == ivp);

    const G4VPhysicalVolume* pv1 = m_detector->getPV(ipv1);    
    const G4VPhysicalVolume* pv2 = m_detector->getPV(ipv2);    

    G4LogicalBorderSurface* lbs = new G4LogicalBorderSurface(name,
             const_cast<G4VPhysicalVolume*>(pv1),
             const_cast<G4VPhysicalVolume*>(pv2),
             os);

    return lbs ; 
}

G4LogicalSkinSurface* CSurLib::makeSkinSurface(GSur* sur, unsigned ilv, G4OpticalSurface* os)
{
    GPropertyMap<float>* pmap = sur->getPMap();
    const char* name = pmap->getName() ;
    const char* daelvn = sur->getLV(ilv);       // assuming LV identity is 1-to-1 with name 

    char* lvn = BStr::DAEIdToG4(daelvn);
    
    const G4LogicalVolume* lv = m_detector->getLV(lvn);

    //if(!lv) 
/*
        LOG(fatal) << "CSurLib::makeSkinSurface"
                   << " ilv " << std::setw(5) << ilv
                   << " name " << std::setw(35) << name
                   << " lvn " << std::setw(35) << lvn 
                   << " lv " << ( lv ? lv->GetName() : "NULL" )
                   ;
*/

    G4LogicalSkinSurface* lss = new G4LogicalSkinSurface(name, const_cast<G4LogicalVolume*>(lv), os );
    return lss ;
}



void CSurLib::addProperties(G4MaterialPropertiesTable* mpt_, GPropertyMap<float>* pmap)
{
    /**
    Property values hail from GSurfaceLib::createStandardSurface  
    which did the preparations for the Opticks texture, so 
    there should be little to do here other than translate into 
    EFFICIENCY and REFLECTIVITY ?
    **/

    CMPT mpt(mpt_);

    GOpticalSurface* os_ = pmap->getOpticalSurface();

    //unsigned   nprop = pmap->getNumProperties();
    //const char* name = pmap->getShortName();
    //LOG(info) << "CSurLib::addProperties " << name ;  

    GProperty<float>* detect = pmap->getProperty(GSurfaceLib::detect);
    GProperty<float>* absorb = pmap->getProperty(GSurfaceLib::absorb);
    GProperty<float>* specular = pmap->getProperty(GSurfaceLib::reflect_specular);
    GProperty<float>* diffuse = pmap->getProperty(GSurfaceLib::reflect_diffuse);

    bool is_sensor = pmap->isSensor();   // ?? always false 
    bool is_specular = os_->isSpecular();

    bool detect_zero = detect->isZero();
    bool absorb_zero = absorb->isZero();
    bool specular_zero = specular->isZero();
    bool diffuse_zero = diffuse->isZero();

    bool spline = false ; 

    std::stringstream ss, tt ; 

    if(!detect_zero) ss << " sensor " ;
    mpt.addProperty("EFFICIENCY",   detect, spline );


    // Opticks distinguishes specular from diffuse by putting 
    // the REFLECTIVITY prop in either 
    // reflect_specular or reflect_diffuse slot 
    // so reverse that here...

    if(specular_zero && diffuse_zero )
    {
        ss << " zerorefl " ;
        mpt.addProperty("REFLECTIVITY", specular , spline );
    }
    else if(specular_zero && !diffuse_zero )
    {
        ss << " diffuse " ;
        mpt.addProperty("REFLECTIVITY", diffuse , spline );
    }
    else if(!specular_zero && diffuse_zero )
    {
        ss << " specular " ;
        mpt.addProperty("REFLECTIVITY", specular , spline );
    }
    else
    {
         assert(0);
    }

    if(detect_zero) tt << "detect_zero " ;
    if(absorb_zero) tt << "absorb_zero " ;
    if(specular_zero) tt << "specular_zero " ;
    if(diffuse_zero) tt << "diffuse_zero " ;

    if(is_sensor) tt << " is_sensor " ; 
    if(is_specular) tt << " is_specular " ; 


/*
    LOG(info) 
              << " name " << std::setw(35) << name
              << " nprop " << std::setw(4) << nprop
              << std::setw(30) << ss.str() 
              << std::setw(50) << tt.str() 
              ;


    LOG(info) << mpt.description("MPT:");

*/

}



