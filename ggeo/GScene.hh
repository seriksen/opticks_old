#pragma once

#include <map>

class GGeo ; 
class GSolid ; 
class GNode ; 
class GBndLib ; 
class GGeoLib ; 
class GMesh ; 

class NCSG ; 
class NScene ; 
struct nd ; 

template<class T> class NPY ;

#include "GGEO_API_EXPORT.hh"
#include "GGEO_HEAD.hh"

/*
GScene
========

Aiming to be a fully analytic replacement for the mainly triangulated 
GTreeCheck 

* to follow the same pattern as GTreeCheck need to 
  label the node tree with a ridx repeat index... 
  in order to use the GGeo meshing machinery

  GGeo/GGeoLib/GMergedMesh::create machinery 

* a global mm0 is needed for setting domains... when 
  operating purely instanced need to construct some 
  placeholder bbox so satisfy the global mesh 0 that 
  lots of things require



*/

class GGEO_API GScene 
{
    public:
        GScene(GGeo* ggeo, NScene* scene);
    private:
        void init();
    private:
        void modifyGeometry();
        void importMeshes(NScene* scene);
        GMesh* getMesh(unsigned mesh_idx);
        NCSG*  getCSG(unsigned mesh_idx);
    private:
        GSolid* createVolumeTree(NScene* scene);
        GSolid* createVolumeTree_r(nd* n);
        GSolid* createVolume(nd* n);
   private:
        void         createInstancedMergedMeshes(bool delta);
        void         makeMergedMeshAndInstancedBuffers() ; 
        void         makeInstancedBuffers(GMergedMesh* mergedmesh, unsigned ridx);

        NPY<float>*    makeInstanceTransformsBuffer(unsigned ridx);
        NPY<unsigned>* makeInstanceIdentityBuffer(unsigned ridx);
        NPY<unsigned>* makeAnalyticInstanceIdentityBuffer(unsigned ridx);
   private:
        GSolid*       getNode(unsigned node_idx);
    private:
        GGeo*    m_ggeo ; 
        GGeoLib* m_geolib ; 
        GBndLib* m_bndlib ; 
        NScene*  m_scene ; 
        GSolid*  m_root ; 

        std::map<unsigned, GMesh*>  m_meshes ; 
        std::map<unsigned, GSolid*> m_nodes ;  

};

#include "GGEO_TAIL.hh"

