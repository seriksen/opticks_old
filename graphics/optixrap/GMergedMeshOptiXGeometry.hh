#pragma once

#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_aabb_namespace.h>

#include "OptiXGeometry.hh"

/*

Migration to the flattened GMergedMesh 
=======================================

Motivation:
-----------

* single geometry instance in recommended as fastest OptiX geometry handling technique
* potentially can persist the buffers rather easily and use them as a geocache
  to avoid parsing at initialization, enabling a fast start 

Progress:
----------

* geometry creation DONE : using convertDrawableInstance and convertDrawable to 
  pump the flattened GMergedMesh to OptiX using a single geometry instance
  with multiple materials

* IN PROGRESS : flattening the substance library into the WavelengthBuffer
 
  Formerly each GSubstance has 1-1 relationship with optix::Material
  which has one texture sampler. Although clean, its kinda pointless
  having order 58 materials which only differ by the textures they contain.

  Trying to move to single material operation with properties using 
  the substanceBuffer to get the substanceIndex which is used
  for wavelength lookups into the waveLengthBuffer.

     material["wavelength_texture"]->setTextureSampler(sampler);

*/


class GMergedMesh ; 
class GBoundaryLib ; 
class GBuffer ; 

// canonical usage from OptiXEngine::initGeometry
//
// TODO: rename to ?MeshGeometry? as handle multiple GMergedMesh 
//       with instancing support 
//

class GMergedMeshOptiXGeometry  : public OptiXGeometry 
{
public:
    GMergedMeshOptiXGeometry(GMergedMesh* mergedmesh, GBoundaryLib* lib); //, OptiXEngine* engine); 

public:
    void convert();
    //optix::TextureSampler makeColorSampler(unsigned int nx);
    optix::TextureSampler makeWavelengthSampler(GBuffer* wavelengthBuffer);
    optix::TextureSampler makeReemissionSampler(GBuffer* reemissionBuffer);
    optix::float4         getDomain();
    optix::float4         getDomainReciprocal();

    template <typename T>
    optix::Buffer createInputBuffer(GBuffer* buf, RTformat format, unsigned int fold=1);

private:
    optix::GeometryInstance makeGeometryInstance(GMergedMesh* mergedmesh);
    optix::Geometry         makeGeometry(GMergedMesh* drawable);
    optix::Material         makeMaterial();

public:
    optix::float3  getMin();
    optix::float3  getMax();

private:
    GMergedMesh* m_mergedmesh ; 
    GBoundaryLib* m_boundarylib ; 
  //  OptiXEngine*  m_engine ; 

};


inline GMergedMeshOptiXGeometry::GMergedMeshOptiXGeometry(GMergedMesh* mergedmesh, GBoundaryLib* lib) //, OptiXEngine* engine)
           : 
           OptiXGeometry(),
           m_mergedmesh(mergedmesh),
           m_boundarylib(lib)
           //m_engine(engine)
{
}


