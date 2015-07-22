#pragma once

#include <vector>
#include <string>
#include "assert.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class NPYBase {
   public:
       typedef enum { FLOAT, SHORT, DOUBLE, INT, UINT, CHAR, UCHAR, ULONGLONG} Type_t ;
       static const char* DEFAULT_PATH_TEMPLATE  ; 
   public:
        NPYBase(std::vector<int>& shape, unsigned char sizeoftype, Type_t type, std::string& metadata );

   public:
       // shape related
       std::vector<int>& getShapeVector();
       std::string  getItemShape(unsigned int ifr=1);
       unsigned int getLength();
       unsigned int getDimensions();
       unsigned int getShape(unsigned int dim);
       unsigned int getValueIndex(unsigned int i, unsigned int j, unsigned int k);
       unsigned int getNumValues(unsigned int from_dim=1);


   public:
       // depending on sizeoftype
       Type_t        getType();
       unsigned char getSizeOfType();
       unsigned int  getNumBytes(unsigned int from_dim=1);
       unsigned int  getByteIndex(unsigned int i, unsigned int j, unsigned int k);

   public:
       // OpenGL related
       void         setBufferId(int buffer_id);
       int          getBufferId();  // either -1 if not uploaded, or the OpenGL buffer Id
       void         setAux(void* aux);
       void*        getAux();
  
   public:
       // NumPy persistency
       static std::string path(const char* typ, const char* tag, const char* det);
       void setVerbose(bool verbose=true);

   public:
       // provided by subclass
       virtual void* getBytes() = 0 ;
       virtual void setQuad(unsigned int i, unsigned int j, glm::vec4&  vec ) = 0 ;
       virtual void setQuad(unsigned int i, unsigned int j, glm::ivec4& vec ) = 0 ;
       virtual glm::vec4  getQuad(unsigned int i, unsigned int j ) = 0 ; 
       virtual glm::ivec4 getQuadI(unsigned int i, unsigned int j ) = 0 ; 

       virtual void save(const char* path) = 0;
       virtual void save(const char* typ, const char* tag, const char* det) = 0;
       virtual void save(const char* tfmt, const char* targ, const char* tag, const char* det ) = 0;
 
    public:
       void Summary(const char* msg="NPYBase::Summary");
       void dump(const char* msg="NPYBase::dump");
       std::string description(const char* msg="NPYBase::description");

   protected:
       unsigned int       m_dim ; 
       unsigned int       m_len0 ; 
       unsigned int       m_len1 ; 
       unsigned int       m_len2 ; 
       unsigned char      m_sizeoftype ; 
       Type_t             m_type ; 
       int                m_buffer_id ; 
       void*              m_aux ; 
       bool               m_verbose ; 
 
   private:
       std::vector<int>   m_shape ; 
       std::string        m_metadata ; 

};


inline NPYBase::NPYBase(std::vector<int>& shape, unsigned char sizeoftype, Type_t type, std::string& metadata ) 
         :
         m_sizeoftype(sizeoftype),
         m_type(type),
         m_buffer_id(-1),
         m_aux(NULL),
         m_verbose(false),
         m_shape(shape),
         m_metadata(metadata)
{
    m_len0 = getShape(0);
    m_len1 = getShape(1);
    m_len2 = getShape(2);
    m_dim  = m_shape.size();
} 


// shape related

inline std::vector<int>& NPYBase::getShapeVector()
{
    return m_shape ; 
}
inline unsigned int NPYBase::getLength()
{
    return getShape(0);
}

inline unsigned int NPYBase::getDimensions()
{
    return m_shape.size();
}
inline unsigned int NPYBase::getShape(unsigned int n)
{
    return n < m_shape.size() ? m_shape[n] : -1 ;
}



// OpenGL related

inline void NPYBase::setBufferId(int buffer_id)
{
    m_buffer_id = buffer_id  ;
}
inline int NPYBase::getBufferId()
{
    return m_buffer_id ;
}

// used for CUDA OpenGL interop
inline void NPYBase::setAux(void* aux)
{
    m_aux = aux ; 
}
inline void* NPYBase::getAux()
{
    return m_aux ; 
}







inline void NPYBase::setVerbose(bool verbose)
{
    m_verbose = verbose ; 
}



inline unsigned int NPYBase::getValueIndex(unsigned int i, unsigned int j, unsigned int k)
{
    assert(m_dim == 3 ); 
    unsigned int nj = m_len1 ;
    unsigned int nk = m_len2 ;
    return  i*nj*nk + j*nk + k ;
}

inline unsigned int NPYBase::getNumValues(unsigned int from_dim)
{
    unsigned int nvals = 1 ; 
    for(unsigned int i=from_dim ; i < m_shape.size() ; i++) nvals *= m_shape[i] ;
    return nvals ;  
}


// depending on sizeoftype

inline unsigned char NPYBase::getSizeOfType()
{
    return m_sizeoftype;
}
inline NPYBase::Type_t NPYBase::getType()
{
    return m_type;
}



inline unsigned int NPYBase::getNumBytes(unsigned int from_dim)
{
    return m_sizeoftype*getNumValues(from_dim);
}
inline unsigned int NPYBase::getByteIndex(unsigned int i, unsigned int j, unsigned int k)
{
    return m_sizeoftype*getValueIndex(i,j,k);
}


