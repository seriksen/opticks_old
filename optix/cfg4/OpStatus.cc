#include "OpStatus.hh"

#include <sstream>
#include "NLog.hpp"

std::string OpStepString(const G4StepStatus status)
{
    std::stringstream ss ;
    std::string s ; 
    switch(status)
    {
        case fWorldBoundary:          s="WorldBoundary"          ;break; 
        case fGeomBoundary:           s="GeomBoundary"           ;break; 
        case fAtRestDoItProc:         s="AtRestDoItProc"         ;break; 
        case fAlongStepDoItProc:      s="AlongStepDoItProc"      ;break; 
        case fPostStepDoItProc:       s="PostStepDoItProc"       ;break; 
        case fUserDefinedLimit:       s="UserDefinedLimit"       ;break; 
        case fExclusivelyForcedProc:  s="ExclusivelyForcedProc"  ;break; 
        case fUndefined:              s="Undefined"              ;break; 
        default:                      s="G4StepStatus-ERROR"     ;break;
    }
    ss << s ; 
    return ss.str() ;
}

std::string OpFlagSequenceString(const unsigned long long seqhis)
{
    std::stringstream ss ;
    assert(sizeof(unsigned long long)*8 == 16*4);
    for(unsigned int i=0 ; i < 16 ; i++)
    {
        unsigned long long f = (seqhis >> i*4) & 0xF ; 
        if(f == 0) 
        {
            ss << "." ;
        }
        else
        { 
            unsigned int flg = 0x1 << (f - 1) ; 
            ss << OpFlagString(flg);
            ss << " " ;
        }
    }
    return ss.str();
}

std::string OpFlagString(const unsigned int flag)
{
    std::stringstream ss ; 
    std::string s ; 
    switch(flag)
    {
        case 0:                s="ZERO";break;
        case CERENKOV:         s="CERENKOV";break;
        case SCINTILLATION:    s="SCINTILLATION" ;break; 
        case MISS:             s="MISS" ;break; 
        case BULK_ABSORB:      s="BULK_ABSORB" ;break; 
        case BULK_REEMIT:      s="BULK_REEMIT" ;break; 
        case BULK_SCATTER:     s="BULK_SCATTER" ;break; 
        case SURFACE_DETECT:   s="SURFACE_DETECT" ;break; 
        case SURFACE_ABSORB:   s="SURFACE_ABSORB" ;break; 
        case SURFACE_DREFLECT: s="SURFACE_DREFLECT" ;break; 
        case SURFACE_SREFLECT: s="SURFACE_SREFLECT" ;break; 
        case BOUNDARY_REFLECT: s="BOUNDARY_REFLECT" ;break; 
        case BOUNDARY_TRANSMIT:s="BOUNDARY_TRANSMIT" ;break; 
        case TORCH:            s="TORCH" ;break; 
        case NAN_ABORT:        s="NAN_ABORT" ;break; 
        default:
                  LOG(warning) << "OpFlagString bad flag [" << flag << "]" << std::hex << flag << std::dec ;             
                  s="ERROR" ;break; 
    }
    ss << s ; 
    return ss.str();
}


std::string OpBoundaryAbbrevString(const G4OpBoundaryProcessStatus status)
{
    std::stringstream ss ; 
    std::string s ; 
    switch(status)
    {
        case Undefined:s="Und";break;
        case Transmission:s="Tra";break;
        case FresnelRefraction:s="FrT";break;
        case FresnelReflection:s="FrR";break;
        case TotalInternalReflection:s="TIR";break;
        case LambertianReflection:s="LaR";break; 
        case LobeReflection:s="LoR";break; 
        case SpikeReflection:s="SpR";break; 
        case BackScattering:s="BkS";break;
        case Absorption:s="Abs";break; 
        case Detection:s="Det";break;
        case NotAtBoundary:s="NAB";break;
        case SameMaterial:s="SMA";break; 
        case StepTooSmall:s="STS";break;
        case NoRINDEX:s="NRI";break;
        case PolishedLumirrorAirReflection:s="PolishedLumirrorAirReflection";break;
        case PolishedLumirrorGlueReflection:s="PolishedLumirrorGlueReflection";break;
        case PolishedAirReflection:s="PolishedAirReflection";break;
        case PolishedTeflonAirReflection:s="PolishedTeflonAirReflection";break;
        case PolishedTiOAirReflection:s="PolishedTiOAirReflection";break;
        case PolishedTyvekAirReflection:s="PolishedTyvekAirReflection";break;
        case PolishedVM2000AirReflection:s="PolishedVM2000AirReflection";break;
        case PolishedVM2000GlueReflection:s="PolishedVM2000GlueReflection";break;
        case EtchedLumirrorAirReflection:s="EtchedLumirrorAirReflection";break;
        case EtchedLumirrorGlueReflection:s="EtchedLumirrorGlueReflection";break;
        case EtchedAirReflection:s="EtchedAirReflection";break;
        case EtchedTeflonAirReflection:s="EtchedTeflonAirReflection";break;
        case EtchedTiOAirReflection:s="EtchedTiOAirReflection";break;
        case EtchedTyvekAirReflection:s="EtchedTyvekAirReflection";break;
        case EtchedVM2000AirReflection:s="EtchedVM2000AirReflection";break;
        case EtchedVM2000GlueReflection:s="EtchedVM2000GlueReflection";break;
        case GroundLumirrorAirReflection:s="GroundLumirrorAirReflection";break;
        case GroundLumirrorGlueReflection:s="GroundLumirrorGlueReflection";break;
        case GroundAirReflection:s="GroundAirReflection";break;
        case GroundTeflonAirReflection:s="GroundTeflonAirReflection";break;
        case GroundTiOAirReflection:s="GroundTiOAirReflection";break;
        case GroundTyvekAirReflection:s="GroundTyvekAirReflection";break;
        case GroundVM2000AirReflection:s="GroundVM2000AirReflection";break;
        case GroundVM2000GlueReflection:s="GroundVM2000GlueReflection";break;
        case Dichroic:s="Dichroic";break;
    }
    ss << s ; 
    return ss.str();
}




std::string OpBoundaryString(const G4OpBoundaryProcessStatus status)
{
    std::stringstream ss ; 
    std::string s ; 
    switch(status)
    {
        case Undefined:s="Undefined";break;
        case Transmission:s="Transmission";break;
        case FresnelRefraction:s="FresnelRefraction";break;
        case FresnelReflection:s="FresnelReflection";break;
        case TotalInternalReflection:s="TotalInternalReflection";break;
        case LambertianReflection:s="LambertianReflection";break; 
        case LobeReflection:s="LobeReflection";break; 
        case SpikeReflection:s="SpikeReflection:";break; 
        case BackScattering:s="BackScattering";break;
        case Absorption:s="Absorption";break; 
        case Detection:s="Detection";break;
        case NotAtBoundary:s="NotAtBoundary";break;
        case SameMaterial:s="SameMaterial";break; 
        case StepTooSmall:s="StepTooSmall";break;
        case NoRINDEX:s="NoRINDEX";break;
        case PolishedLumirrorAirReflection:s="PolishedLumirrorAirReflection";break;
        case PolishedLumirrorGlueReflection:s="PolishedLumirrorGlueReflection";break;
        case PolishedAirReflection:s="PolishedAirReflection";break;
        case PolishedTeflonAirReflection:s="PolishedTeflonAirReflection";break;
        case PolishedTiOAirReflection:s="PolishedTiOAirReflection";break;
        case PolishedTyvekAirReflection:s="PolishedTyvekAirReflection";break;
        case PolishedVM2000AirReflection:s="PolishedVM2000AirReflection";break;
        case PolishedVM2000GlueReflection:s="PolishedVM2000GlueReflection";break;
        case EtchedLumirrorAirReflection:s="EtchedLumirrorAirReflection";break;
        case EtchedLumirrorGlueReflection:s="EtchedLumirrorGlueReflection";break;
        case EtchedAirReflection:s="EtchedAirReflection";break;
        case EtchedTeflonAirReflection:s="EtchedTeflonAirReflection";break;
        case EtchedTiOAirReflection:s="EtchedTiOAirReflection";break;
        case EtchedTyvekAirReflection:s="EtchedTyvekAirReflection";break;
        case EtchedVM2000AirReflection:s="EtchedVM2000AirReflection";break;
        case EtchedVM2000GlueReflection:s="EtchedVM2000GlueReflection";break;
        case GroundLumirrorAirReflection:s="GroundLumirrorAirReflection";break;
        case GroundLumirrorGlueReflection:s="GroundLumirrorGlueReflection";break;
        case GroundAirReflection:s="GroundAirReflection";break;
        case GroundTeflonAirReflection:s="GroundTeflonAirReflection";break;
        case GroundTiOAirReflection:s="GroundTiOAirReflection";break;
        case GroundTyvekAirReflection:s="GroundTyvekAirReflection";break;
        case GroundVM2000AirReflection:s="GroundVM2000AirReflection";break;
        case GroundVM2000GlueReflection:s="GroundVM2000GlueReflection";break;
        case Dichroic:s="Dichroic";break;
    }
    ss << s ; 
    return ss.str();
}





unsigned int OpBoundaryFlag(const G4OpBoundaryProcessStatus status)
{
    unsigned int flag = 0 ; 
    switch(status)
    {
        case FresnelRefraction:flag=BOUNDARY_TRANSMIT;break;
        case FresnelReflection:flag=BOUNDARY_REFLECT;break;
        case StepTooSmall:flag=NAN_ABORT;break;

        case Undefined:
        case Transmission:
        case TotalInternalReflection:
        case LambertianReflection:
        case LobeReflection:
        case SpikeReflection:
        case BackScattering:
        case Absorption:
        case Detection:
        case NotAtBoundary:
        case SameMaterial:
        case NoRINDEX:
        case PolishedLumirrorAirReflection:
        case PolishedLumirrorGlueReflection:
        case PolishedAirReflection:
        case PolishedTeflonAirReflection:
        case PolishedTiOAirReflection:
        case PolishedTyvekAirReflection:
        case PolishedVM2000AirReflection:
        case PolishedVM2000GlueReflection:
        case EtchedLumirrorAirReflection:
        case EtchedLumirrorGlueReflection:
        case EtchedAirReflection:
        case EtchedTeflonAirReflection:
        case EtchedTiOAirReflection:
        case EtchedTyvekAirReflection:
        case EtchedVM2000AirReflection:
        case EtchedVM2000GlueReflection:
        case GroundLumirrorAirReflection:
        case GroundLumirrorGlueReflection:
        case GroundAirReflection:
        case GroundTeflonAirReflection:
        case GroundTiOAirReflection:
        case GroundTyvekAirReflection:
        case GroundVM2000AirReflection:
        case GroundVM2000GlueReflection:
        case Dichroic:
                      flag=0;break;
    }
    return flag ; 
}


