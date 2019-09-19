/*
 * Copyright (c) 2019 Opticks Team. All Rights Reserved.
 *
 * This file is part of Opticks
 * (see https://bitbucket.org/simoncblyth/opticks).
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */

#include <csignal>
#include <ostream>
#include <fstream>
#include <iomanip>

#include "BStr.hh"
#include "BFile.hh"
#include "Opticks.hh"

#include "NCSG.hpp"

#include "GMesh.hh"
#include "GMeshLib.hh"
#include "GItemList.hh"

#include "PLOG.hh"

const plog::Severity GMeshLib::LEVEL = debug ; 

const unsigned GMeshLib::MAX_MESH = 300 ;   // <-- hmm 500 too large ? it means a lot of filesystem checking 
/**
TODO: be more clever about the introspection to avoid the filesystem checking for every index
**/



const char* GMeshLib::GMESHLIB = "GMeshLib" ; 
const char* GMeshLib::GMESHLIB_LIST = "GMeshLib" ; 
const char* GMeshLib::GMESHLIB_NCSG = "GMeshLibNCSG" ; 


GMeshLib::GMeshLib(Opticks* ok ) 
    :
    m_ok(ok),
    m_direct(ok->isDirect()),
    m_reldir(GMESHLIB),
    m_reldir_solids(GMESHLIB_NCSG),
    m_meshnames(new GItemList(GMESHLIB_LIST, "GItemList"))
{
}

GMeshLib* GMeshLib::Load(Opticks* ok)
{
    GMeshLib* meshlib = new GMeshLib(ok);
    meshlib->loadFromCache(); 
    return meshlib ; 
}

void GMeshLib::loadFromCache()
{
    const char* idpath = m_ok->getIdPath() ;

    m_meshnames = GItemList::Load(idpath, GMESHLIB_LIST, "GItemList" ) ;
    assert(m_meshnames);

    loadMeshes(idpath);
    loadAltReferences();  
}

void GMeshLib::save() 
{
    addAltMeshes(); 

    const char* idpath = m_ok->getIdPath() ;

    assert( m_meshnames ); 
    m_meshnames->save(idpath );

    saveAltReferences();  

    saveMeshes(idpath);
    saveMeshUsage(idpath);
}


/**
GMeshLib::addAltMeshes
------------------------

Alternative meshes associated with meshes in the 
library via setAlt are added to the library in a
deferred manner in order not to influence the indices 
of the standard non-alt meshes.

Note that all GMesh instances including the alt 
are expected to be distinct. 

This gets invoked by GMeshLib::save

**/

void GMeshLib::addAltMeshes()
{
    std::vector<unsigned> indices_with_alt ; 
    getMeshIndicesWithAlt(indices_with_alt) ; 
 
    LOG(info) 
        << " num_indices_with_alt " << indices_with_alt.size()
        ;
 
    for(unsigned i=0 ; i < indices_with_alt.size() ; i++)
    {
        unsigned index = indices_with_alt[i] ; 
        GMesh* mesh = getMeshSimple(index); 
        const GMesh* altmesh = mesh->getAlt() ; 
        assert(altmesh);
        LOG(info) << " index with alt " << index ;  
        bool alt = true ; 
        add(altmesh, alt ); 
    }
 
    dump("addAltMeshes"); 
}

const std::vector<const NCSG*>& GMeshLib::getSolids() const { return m_solids ; }
const std::vector<const GMesh*>& GMeshLib::getMeshes() const { return m_meshes ; }

 

/**
GMeshLib::saveAltReferences
--------------------------------

**/

void GMeshLib::saveAltReferences() 
{
    for(unsigned i=0 ; i < m_meshes.size() ; i++ )
    {
        const GMesh* mesh = m_meshes[i]; 
        const NCSG* solid = m_solids[i];  

        int index = findMeshIndex(mesh); 
        assert( unsigned(index) == i );  // not expecting same GMesh instance more than once

        const GMesh* altmesh = mesh->getAlt(); 
        if(altmesh == NULL) continue ; 

        const NCSG* altsolid = altmesh->getCSG();
        assert( altsolid ); 

        int altindex = findMeshIndex(altmesh);    
        int altindex2 = findSolidIndex(altsolid);    
        assert( altindex == altindex2 ); 

        assert( altindex != -1 && " alt mesh not present in the lib "); 

        // make alt relationship symmetric
        const_cast<NCSG*>(solid)->set_altindex( altindex ); 
        const_cast<NCSG*>(altsolid)->set_altindex( index ); 

    }
}


/**
GMeshLib::loadAltReferences
-----------------------------


**/


void GMeshLib::loadAltReferences() 
{
    for(unsigned i=0 ; i < m_meshes.size() ; i++ )
    {
        const GMesh* mesh = m_meshes[i] ; 
        const NCSG* solid = i < m_solids.size() ? m_solids[i] : NULL ;  
        assert( mesh->getCSG() == solid ); 
        if(solid == NULL) continue ;  

        int altindex = solid->get_altindex();  
        if(altindex == -1) continue ; 
     
        assert( unsigned(altindex) < m_meshes.size() ); 
        const GMesh* alt = m_meshes[altindex] ; 
        const_cast<GMesh*>(mesh)->setAlt(alt) ;            
        LOG(LEVEL) 
            << " mesh.i " << i 
            << " altindex " << altindex ; 
            ;
    }
}

int GMeshLib::findMeshIndex( const GMesh* mesh ) const 
{
    typedef std::vector<const GMesh*> VM ; 
    VM::const_iterator it = std::find( m_meshes.begin(), m_meshes.end(), mesh );
    return it ==  m_meshes.end() ? -1 : std::distance( m_meshes.begin(), it ) ; 
}
int GMeshLib::findSolidIndex( const NCSG* solid ) const 
{
    typedef std::vector<const NCSG*> VS ; 
    VS::const_iterator it = std::find( m_solids.begin(), m_solids.end(), solid ); 
    return it ==  m_solids.end() ? -1 : std::distance( m_solids.begin(), it ) ; 
}


const char* GMeshLib::getMeshName(unsigned aindex) const 
{
    return m_meshnames->getKey(aindex); 
}

void GMeshLib::dump(const char* msg) const
{
    unsigned num_mesh = m_meshnames->getNumKeys();
    LOG(info) << msg 
              << " meshnames " << num_mesh 
              << " meshes " << m_meshes.size() 
              ; 

    for(unsigned i=0 ; i < num_mesh ; i++)
    {
        const char* name = getMeshName(i); 
        assert(name);
        bool startswith(false); 
        int aidx = getMeshIndexWithName(name, startswith);  // hmm this is lib index not mesh index

        const GMesh* mesh = getMeshSimple(i);
        assert( mesh ); 
        unsigned midx = mesh->getIndex();  

        assert( strcmp(mesh->getName(), name) == 0);

        std::cout 
               << " i " << std::setw(3) << i  
               << " aidx " << std::setw(3) << aidx  
               << " midx " << std::setw(3) << midx  
               << " name " << std::setw(50) << name 
               << " mesh " << mesh->desc()
               << std::endl 
               ;
    }
}


unsigned GMeshLib::getNumMeshes() const 
{
    return m_meshes.size();
}

unsigned GMeshLib::getNumSolids() const 
{
    return m_solids.size();
}



const NCSG* GMeshLib::getSolidWithIndex(unsigned aindex) const  // gets the solid with the index
{
    const NCSG* solid = NULL ; 
    for(unsigned i=0 ; i < m_solids.size() ; i++ )
    { 
        if(m_solids[i]->getIndex() == aindex )
        {
            solid = m_solids[i] ; 
            break ; 
        }
    }
    return solid ;
}  

const GMesh* GMeshLib::getMeshWithIndex(unsigned aindex) const // gets the mesh with the index
{
    const GMesh* mesh = NULL ; 
    for(unsigned i=0 ; i < m_meshes.size() ; i++ )
    { 
        if(m_meshes[i]->getIndex() == aindex )
        {
            mesh = m_meshes[i] ; 
            break ; 
        }
    }
    return mesh ;
}  


const GMesh* GMeshLib::getMeshWithName(const char* name, bool startswith) const 
{
    int idx = getMeshIndexWithName(name, startswith);
    return idx == -1 ? NULL : getMeshWithIndex(idx);
}


int GMeshLib::getMeshIndexWithName(const char* name, bool startswith)  const 
{
    int aindex = startswith ? m_meshnames->findIndexWithKeyStarting(name) : m_meshnames->findIndex(name) ;   
    return aindex ; 
}



GMesh* GMeshLib::getMeshSimple(unsigned index) const 
{
    assert( index < m_meshes.size() ); 
    const GMesh* mesh_ = m_meshes[index] ; 
    GMesh* mesh = const_cast<GMesh*>(mesh_);   // hmm rethink needed ?

    bool index_match = mesh->getIndex() == index ; 
    if(!index_match)
       LOG(error) 
           << " mesh indices do not match " 
           << " m_meshes index " << index
           << " mesh.index " << mesh->getIndex()
           ; 

    //assert( index_match ); 
    return mesh ; 
}  

const GMesh* GMeshLib::getAltMesh(unsigned index) const 
{
    const GMesh* mesh = getMeshSimple(index); 
    return mesh->getAlt();  
}


void GMeshLib::getMeshIndicesWithAlt(std::vector<unsigned>& indices) const 
{
    for(unsigned i=0 ; i < m_meshes.size() ; i++ )
    {
        const GMesh* mesh = m_meshes[i] ; 
        const GMesh* alt = mesh->getAlt(); 
        if(alt) indices.push_back(i); 
    }
}


/**
GMeshLib::add
----------------

Invoked via GGeo::add from X4PhysicalVolume::convertSolids_r as each distinct 
solid is encountered in the recursive traverse.

**/

void GMeshLib::add(const GMesh* mesh, bool alt )
{
    const char* name = mesh->getName();
    unsigned index = mesh->getIndex();
    assert(name) ; 

    m_meshnames->add(name); 

    LOG(debug) 
        << " index " << std::setw(4) << index 
        << " name " << name 
        ;


    if( alt == false )
    {
        bool inorder = index == m_meshes.size()  ; 
        if(!inorder )
        {
            LOG(fatal) 
                << " non-alt meshes are required to be added in index order "
                << " index " << index
                << " m_meshes.size() " << m_meshes.size()
                ; 
        }
        assert( inorder ) ; 
    }


    m_meshes.push_back(mesh);

    const NCSG* solid = mesh->getCSG(); 
    if(m_direct) assert(solid) ;                
    m_solids.push_back(solid); 

    //std::raise(SIGINT); 
}


void GMeshLib::replace(unsigned index, GMesh* replacement )
{
    // replace the name too ?

    assert( index < m_meshes.size() ); 
    m_meshes[index] = replacement ; 

    const NCSG* solid = replacement->getCSG(); 
    if(m_direct) assert(solid) ;                

    assert( index < m_solids.size() ); 
    m_solids[index] = solid  ; 
}



void GMeshLib::removeDirs(const char* idpath ) const 
{
   for(unsigned int idx=0 ; idx < MAX_MESH ; ++idx)
   {   
        const char* sidx = BStr::itoa(idx);
        if(BFile::ExistsDir(idpath, m_reldir, sidx))
        { 
            BFile::RemoveDir(idpath, m_reldir, sidx); 
        }
        if(BFile::ExistsDir(idpath, m_reldir_solids, sidx))
        { 
            BFile::RemoveDir(idpath, m_reldir_solids, sidx); 
        }
   } 
}

/**
GMeshLib::loadMeshes
----------------------

In addition to GMesh instances this also loads the corresponding NCSG 
analytic solids and associates them with the GMesh instances.

**/

void GMeshLib::loadMeshes(const char* idpath )
{
   LOG(LEVEL) << "idpath "  << idpath ;  

   // TODO: read the directory instead of just checking existance of MAX_MESH paths ?
   //       (or use the index ?)

   for(unsigned int idx=0 ; idx < MAX_MESH ; ++idx)
   {   
        const char* sidx = BStr::itoa(idx);

        std::string meshdir_ = BFile::FormPath(idpath, m_reldir, sidx);
        const char* meshdir = meshdir_.c_str() ;

        std::string soliddir_ = BFile::FormPath(idpath, m_reldir_solids, sidx);
        const char* soliddir = soliddir_.c_str() ;

        bool meshdir_exists = BFile::ExistsDir(meshdir) ; 
        bool soliddir_exists = BFile::ExistsDir(soliddir) ; 

        if(meshdir_exists)
        {   
            if( m_direct )
            {
                assert( soliddir_exists && "GMeshLib persisted GMesh are expected to have paired GMeshLibNCSG dirs"); 
            }

            LOG(debug) 
                << " meshdir " << meshdir 
                << " meshdir_exists " << meshdir_exists 
                << " soliddir " << soliddir 
                << " soliddir_exists " << soliddir_exists 
                ; 

            GMesh* mesh = GMesh::load( meshdir );
            NCSG* solid = NCSG::Load( soliddir );  
            mesh->setCSG(solid); 

            const char* name = getMeshName(idx);
            if(name == NULL)
                LOG(fatal) 
                    << " no name for mesh idx " << idx 
                    ; 

            assert(name);

            mesh->setIndex(idx);
            mesh->setName(strdup(name));

            m_meshes.push_back(mesh);
            m_solids.push_back(solid);
        }
        else
        {
            LOG(debug)
                << " MISSING meshdir "  
                << " idx " << idx 
                << " meshdir " << meshdir 
                ;
        }
   }
   LOG(LEVEL) 
       << " loaded "  
       << " meshes "  << m_meshes.size()
       << " solids "  << m_solids.size()
       ;
}


/**
GMeshLib::saveMeshes
---------------------

Note that cannot assert the existance of associated NCSG solids
as they are not present in legacy workflow, eg as exercised by "op.sh -G".

In the legacy route the analytic solid info is handled precache via NScene/GScene
and placed into GParts associated with the GMergedLib preventing 
the need for the deferred GParts creation that is used 
in the direct workflow.

* notes/issues/geocache-create-reversion-reported-by-sam.rst
* notes/issies/plan-removal-of-legacy-geometry-workflow-packages-and-externals.rst

To see which GMergedMesh/GParts/GPts are being loaded turn up the 
logging levels in GGeoLib::

   GGeoLib=ERROR OKTest --xanalytic --gltf 1


**/

void GMeshLib::saveMeshes(const char* idpath) const 
{
    removeDirs(idpath); // clean old meshes to avoid duplication when repeat counts go down 

    for(unsigned i=0 ; i < m_meshes.size() ; i++)
    {
        unsigned idx = i ;               // <-- with the "alt" mesh index doesnt always match library index 
        const GMesh* mesh = m_meshes[idx] ; 
        const char* sidx = BStr::itoa(idx);

        mesh->save(idpath, m_reldir, sidx); 

        const NCSG* solid = mesh->getCSG(); 
        if(!solid) continue ; 
        //assert(solid);   <-- tripped by legacy route, 

        solid->savesrc(idpath, m_reldir_solids, sidx); 
    }

    // meshindex persisted first, up in GMeshLib::save
}



void GMeshLib::countMeshUsage(unsigned int meshIndex, unsigned int nodeIndex)
{
     // called during GGeo creation from: void AssimpGGeo::convertStructure(GGeo* gg)
     //printf("GMeshLib::countMeshUsage %d %d %s %s \n", meshIndex, nodeIndex, lv, pv);
     m_mesh_usage[meshIndex] += 1 ; 
     m_mesh_nodes[meshIndex].push_back(nodeIndex); 
}


std::map<unsigned int, unsigned int>& GMeshLib::getMeshUsage()
{
    return m_mesh_usage ; 
}
std::map<unsigned int, std::vector<unsigned int> >& GMeshLib::getMeshNodes()
{
    return m_mesh_nodes ; 
}

void GMeshLib::reportMeshUsage_(std::ostream& out) const 
{
     typedef std::map<unsigned int, unsigned int>::const_iterator MUUI ; 
     out << " meshIndex, nvert, nface, nodeCount, nodeCount*nvert, nodeCount*nface, meshName " << std::endl ; 

     unsigned tnode(0) ; 
     unsigned tvert(0) ; 
     unsigned tface(0) ; 

     for(MUUI it=m_mesh_usage.begin() ; it != m_mesh_usage.end() ; it++)
     {
         unsigned int meshIndex = it->first ; 
         unsigned int nodeCount = it->second ; 
 
         const GMesh* mesh = getMeshWithIndex(meshIndex);
         const char* meshName = mesh->getName() ; 
         unsigned int nvert = mesh->getNumVertices() ; 
         unsigned int nface = mesh->getNumFaces() ; 

         //printf("  %4d (v%5d f%5d) : %6d : %7d : %7d : %s \n", meshIndex, nvert, nface, nodeCount, nodeCount*nvert, nodeCount*nface, meshName);

         out << "  " << std::setw(4) << meshIndex 
             << " ("
             << " v" << std::setw(5) << nvert 
             << " f" << std::setw(5) << nface
             << " )"
             << " : " << std::setw(7) << nodeCount
             << " : " << std::setw(10) << nodeCount*nvert
             << " : " << std::setw(10) << nodeCount*nface
             << " : " << meshName 
             << std::endl ; 


         tnode += nodeCount ; 
         tvert += nodeCount*nvert ; 
         tface += nodeCount*nface ; 
     }

     //printf(" tnode : %7d \n", tnode);
     //printf(" tvert : %7d \n", tvert);
     //printf(" tface : %7d \n", tface);

     out << " tot "
         << " node : " << std::setw(7) << tnode
         << " vert : " << std::setw(7) << tvert
         << " face : " << std::setw(7) << tface
         << std::endl ;
}

void GMeshLib::reportMeshUsage(const char* msg) const 
{
    std::ostream& out = std::cout ; 
    out << msg << std::endl ; 
    reportMeshUsage_(out); 
}

void GMeshLib::writeMeshUsage(const char* path) const 
{
    LOG(LEVEL) << " write to " << path ; 
    std::ofstream out(path); 
    out << "GMeshLib::writeMeshUsage"  << std::endl ; 
    reportMeshUsage_(out); 
}

void GMeshLib::saveMeshUsage(const char* idpath) const 
{
    std::string path = BFile::FormPath(idpath, m_reldir, "MeshUsage.txt") ; 
    writeMeshUsage(path.c_str()); 
}


