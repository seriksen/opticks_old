// adapted from /Developer/OptiX/SDK/sutil/PlyLoader.cpp

#include "G4DAELoader.hh"

#include <optixu/optixu.h>
#include <optixu/optixpp_namespace.h>
#include <optixu/optixu_math_namespace.h>
#include <optixu/optixu_math_stream_namespace.h>

#include <iostream>
#include <cstring> //memcpy
#include <algorithm>

#include "AssimpCommon.hh"
#include "AssimpGeometry.hh"

#ifdef DEPRECATED
#include "AssimpOptiXGeometry.hh"
#endif

#include "AssimpGGeo.hh"
#include "GGeoOptiXGeometry.hh"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"

#include <string>

// npy-
#include "stringutil.hpp"

std::string getExtension( const std::string& filename )
{
    std::string::size_type extension_index = filename.find_last_of( "." );
    return extension_index != std::string::npos ?
           filename.substr( extension_index+1 ) : 
           std::string();
}

const char* G4DAELoader::identityFilename( char* arg )
{
     char* query = getenv("RAYTRACE_QUERY");  // expedient kludge
     return AssimpGeometry::identityFilename(arg, query);
}

G4DAELoader::G4DAELoader( const char*   filename,
                      optix::Context       context,
                      optix::GeometryGroup geometry_group,
                      optix::Material      material,
                      const char* ASBuilder,
                      const char* ASTraverser,
                      const char* ASRefine,
                      bool large_geom )
: m_filename( strdup(filename) ),
  m_context( context ),
  m_geometry_group( geometry_group ),
  m_material( material ),
  m_large_geom( large_geom),
  m_ASBuilder  (ASBuilder),
  m_ASTraverser(ASTraverser),
  m_ASRefine   (ASRefine),
  m_ggeo(NULL)
{
  // Error checking on context and geometrygroup done in ModelLoader

  assert(material.get()); // MUST DEFINE MATERIAL ELSEWHERE
}

G4DAELoader::~G4DAELoader()
{
   free((void*)m_filename);
}



GGeo* G4DAELoader::getGGeo()
{
   return m_ggeo ; 
}

void G4DAELoader::load()
{
  load( optix::Matrix4x4::identity() );
}


void G4DAELoader::load( const optix::Matrix4x4& transform )
{
  char* query = getenv("RAYTRACE_QUERY");     
  printf("G4DAELoader::load query %s \n", query );
  printf("G4DAELoader::load filename %s \n", m_filename );

  // geometry selection query string is encoded into m_filename
  // the digest of the query is removed from m_filename 
  // into order to get the real path to load 
  //
  // This trickery is used in order for separate 
  // optix accelcache to be layed down for 
  // different geometry selections 

  char* path = new char[strlen(m_filename)]; 
  removeField( path , m_filename, '.' , -2 );

  printf("G4DAELoader::load path     %s \n", path );

  AssimpGeometry ageo(path);
  ageo.import();
  AssimpSelection* selection = ageo.select(query);


  const char* ggctrl = getenv("RAYTRACE_GGCTRL");
  if(ggctrl) 
  {
      printf("G4DAELoader::load with GGeo intermediary using AssimpGGeo + GGeoOptixGeometry %s \n", ggctrl );

      AssimpGGeo agg(ageo.getTree(), selection); 
      m_ggeo = agg.convert(ggctrl);

      GGeoOptiXGeometry geom(m_ggeo);
      
      geom.setGeometryGroup(m_geometry_group);
      geom.setContext(m_context);   
      geom.setOverrideMaterial(m_material);  

      geom.convert(); 
      geom.setupAcceleration();

      m_aabb = geom.getAabb();
  }
  else
  {
      printf("G4DAELoader::load with AssimpOptixGeometry IS DEPRECATED\n");
#ifdef DEPRECATED
      AssimpOptiXGeometry geom(&ageo, selection);

      geom.setGeometryGroup(m_geometry_group);
      geom.setContext(m_context);  
      geom.setOverrideMaterial(m_material);  

      geom.convert(); 
      geom.setupAcceleration();

      m_aabb = geom.getAabb();
#endif
  }

  
}




bool G4DAELoader::isMyFile( const char* filename )
{
  std::string fn(filename); 
  return getExtension( fn ) == "dae";
}

