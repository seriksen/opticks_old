#include "Camera.hh"

// npy-
#include "GLMPrint.hpp"
#include "GLMFormat.hpp"
#include "NLog.hpp"

#include "stdio.h"
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtx/transform.hpp>
#include <boost/lexical_cast.hpp>


const char* Camera::PREFIX = "camera" ;
const char* Camera::getPrefix()
{
   return PREFIX ; 
}


const char* Camera::PRINT    = "print" ;

const char* Camera::NEAR     = "near" ;
const char* Camera::FAR      = "far" ;
const char* Camera::ZOOM     = "zoom" ;
const char* Camera::SCALE     = "scale" ;

const char* Camera::PARALLEL = "parallel" ;


bool Camera::accepts(const char* name)
{
    return 
          strcmp(name, NEAR) == 0  ||
          strcmp(name, FAR ) == 0  || 
          strcmp(name, SCALE ) == 0  || 
          strcmp(name, ZOOM ) == 0  ;
}  



std::vector<std::string> Camera::getTags()
{
    std::vector<std::string> tags ;
    tags.push_back(NEAR);
    tags.push_back(FAR);
    tags.push_back(ZOOM);
    tags.push_back(SCALE);
    return tags ; 
}

std::string Camera::get(const char* name)
{
    float v(0.f) ; 

    if(     strcmp(name,NEAR)==0)     v = getNear();
    else if(strcmp(name,FAR)== 0 )    v = getFar();
    else if(strcmp(name,ZOOM)== 0 )   v = getZoom();
    else if(strcmp(name,SCALE)== 0 )  v = getScale();
    else
         printf("Camera::get bad name %s\n", name);

    return gformat(v);
}

void Camera::set(const char* name, std::string& s)
{
    float v = gfloat_(s); 

    if(     strcmp(name,NEAR)==0)    setNear(v);
    else if(strcmp(name,FAR)== 0 )   setFar(v);
    else if(strcmp(name,ZOOM)== 0 )  setZoom(v);
    else if(strcmp(name,SCALE)== 0 ) setScale(v);
    else
         printf("Camera::set bad name %s\n", name);
}


void Camera::configure(const char* name, const char* val_)
{
    std::string val(val_);
    configure(name, gfloat_(val));
}

void Camera::configure(const char* name, float value)
{
    if( strcmp(name, ZOOM) ==  0)      setZoom(value);
    else if( strcmp(name, SCALE) ==  0)     setScale(value);
    else if( strcmp(name, NEAR) ==  0)      setNear(value);
    else if( strcmp(name, FAR) ==  0)       setFar(value);
    else if( strcmp(name, PARALLEL) ==  0)  setParallel( value==0.f ? false : true );
    else
        printf("Camera::configure ignoring unknown parameter %s : %10.3f \n", name, value); 
}
 





void Camera::configureS(const char* name, std::vector<std::string> values)
{
}

void Camera::configureI(const char* name, std::vector<int> values)
{
    if( strcmp(name, PRINT) ==  0)  Print("liveline --print");
}

void Camera::configureF(const char* name, std::vector<float> values)
{
     if(values.empty())
     {
         printf("Camera::parameter_set %s no values \n", name);
     }
     else         
     {
         float vlast = values.back() ;

#ifdef VERBOSE
         LOG(info) << "Camera::configureF"
                   << " name " << name
                   << " vals " << values.size()
                   ;

         for(size_t i=0 ; i < values.size() ; i++ ) printf("%10.3f ", values[i]);
         printf(" : vlast %10.3f \n", vlast );
#endif
         configure(name, vlast);  
     }
}
 



void Camera::Print(const char* msg)
{
    printf("%s parallel %d  near %10.3f far %10.3f zoom %10.3f scale %10.3f \n", msg, m_parallel, m_near, m_far, m_zoom, getScale() );
}


void Camera::Summary(const char* msg)
{
    printf("%s  parallel %d \n", msg, m_parallel );
    printf(" width %5d height %5d  aspect %10.3f \n", m_size[0], m_size[1], getAspect() );
    printf(" near %10.3f  clip %10.3f %10.3f \n", m_near, m_nearclip[0], m_nearclip[1] );
    printf(" far  %10.3f  clip %10.3f %10.3f \n", m_far , m_farclip[0], m_farclip[1] );
    printf(" scale %10.3f  clip %10.3f %10.3f \n", m_scale, m_scaleclip[0], m_scaleclip[1] );
    printf(" zoom %10.3f  clip %10.3f %10.3f \n", m_zoom, m_zoomclip[0], m_zoomclip[1] );
    printf(" top %10.3f bot %10.3f left %10.3f right %10.3f tan(yfov/2) %10.3f \n", getTop(), getBottom(), getLeft(), getRight(), getTanYfov() );

    glm::mat4 projection = getProjection();    
    print(projection, "projection");

    glm::mat4 perspective = getPerspective();    
    print(perspective, "perspective");

    glm::mat4 ortho = getOrtho();    
    print(ortho, "ortho");

    glm::mat4 frustum = getFrustum();    
    print(frustum, "frustum");
}     


glm::mat4 Camera::getProjection()
{
    return m_parallel ? getOrtho() : getFrustum() ; 
}


void Camera::fillZProjection(glm::vec4& zProj)
{
    glm::mat4 proj = getProjection() ;
    zProj.x = proj[0][2] ; 
    zProj.y = proj[1][2] ; 
    zProj.z = proj[2][2] ; 
    zProj.w = proj[3][2] ; 
}

glm::mat4 Camera::getPerspective()
{
    return glm::perspective(getYfov(), getAspect(), getNear(), getFar());
}
glm::mat4 Camera::getOrtho()
{
    return glm::ortho( getLeft(), getRight(), getBottom(), getTop(), getNear(), getFar() );
}
glm::mat4 Camera::getFrustum()
{
    return glm::frustum( getLeft(), getRight(), getBottom(), getTop(), getNear(), getFar() );
}



