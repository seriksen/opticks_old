#pragma once

class GMesh ;
class GBoundary ; 
class GSensor ; 

#include "GNode.hh"
#include "GMatrix.hh"

//  hmm the difference between the models is focussed in here 
//   chroma.geometry.Solid is all about splaying things across all the triangles
//  relationship between how many materials for each mesh is up for grabs
//

class GSolid : public GNode {
  public:
      GSolid( unsigned int index, GMatrixF* transform, GMesh* mesh,  GBoundary* boundary, GSensor* sensor);
      virtual ~GSolid();

  public:
     void setSelected(bool selected);
     bool isSelected();

  public:
     void setBoundary(GBoundary* boundary);
     void setSensor(GSensor* sensor);
  public:
     // need to resort to names for debugging IAV top lid issue
     void setPVName(const char* pvname);
     void setLVName(const char* lvname);
     const char* getPVName();
     const char* getLVName();

  public:
     GBoundary* getBoundary();
     GSensor*   getSensor();

  public: 
      void Summary(const char* msg="GSolid::Summary");
 
  private:
      GBoundary*        m_boundary ; 
      GSensor*          m_sensor ; 
      bool              m_selected ;
      const char*       m_pvname ; 
      const char*       m_lvname ; 

};

inline GSolid::GSolid( unsigned int index, GMatrixF* transform, GMesh* mesh, GBoundary* boundary, GSensor* sensor)
         : 
         GNode(index, transform, mesh ),
         m_boundary(boundary),
         m_sensor(sensor),
         m_selected(true),
         m_pvname(NULL),
         m_lvname(NULL)
{
}

inline GSolid::~GSolid()
{
}

inline GBoundary* GSolid::getBoundary()
{
    return m_boundary ; 
}
inline GSensor* GSolid::getSensor()
{
    return m_sensor ; 
}

inline void GSolid::setSelected(bool selected)
{
    m_selected = selected ; 
}
inline bool GSolid::isSelected()
{
   return m_selected ; 
}

inline void GSolid::setPVName(const char* pvname)
{
    m_pvname = strdup(pvname);
}
inline void GSolid::setLVName(const char* lvname)
{
    m_lvname = strdup(lvname);
}

inline const char* GSolid::getPVName()
{
    return m_pvname ; 
}
inline const char* GSolid::getLVName()
{
    return m_lvname ; 
}




