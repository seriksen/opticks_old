#pragma once

#include <cassert>

#include "NNode.hpp"
#include "NQuad.hpp"
#include "NGLM.hpp"
#include "NZSphere.h"

struct nbbox ; 

#include "NPY_API_EXPORT.hh"

struct NPY_API nzsphere : nnode {

    float operator()(float x, float y, float z) const ;

    nbbox bbox() const ;

    glm::vec3 gseedcenter() ;
    glm::vec3 gseeddir();

    void pdump(const char* msg="nzsphere::pdump", int verbosity=1);

    float zmax() const { return center.z + zdelta.y ; }
    float zmin() const { return center.z + zdelta.x ; }
    unsigned flags() const { return param2.u.x ; }

 
    glm::vec3 center ; 
    glm::vec2 zdelta ;   // z range relative to center.z, ie -radius, radius would correspond to full sphere
    float     radius ; 

};

inline NPY_API void init_zsphere(nzsphere& s, const nquad& param, const nquad& param1, const nquad& param2)
{
    s.param = param ; 
    s.param1 = param1 ; 
    s.param2 = param2 ; 

    s.center.x = param.f.x ; 
    s.center.y = param.f.y ; 
    s.center.z = param.f.z ;
    s.radius   = param.f.w ;  

    s.zdelta.x = param1.f.x  ;
    s.zdelta.y = param1.f.y  ;

    assert( s.zdelta.y > s.zdelta.x );
}

inline NPY_API nzsphere make_zsphere(const nquad& param, const nquad& param1, const nquad& param2)
{
    nzsphere n ; 
    nnode::Init(n,CSG_ZSPHERE) ; 
    init_zsphere(n, param, param1, param2 );
    return n ; 
}

inline NPY_API nzsphere make_zsphere(float x, float y, float z, float radius, float zdelta_min, float zdelta_max, unsigned flags=ZSPHERE_PCAP|ZSPHERE_QCAP)
{
    nquad p0, p1, p2  ; 

    p0.f = {x,y,z,radius} ;
    p1.f = {zdelta_min, zdelta_max, 0,0} ;
    p2.u = {flags, 0,0,0};

    return make_zsphere(p0, p1, p2);
}
