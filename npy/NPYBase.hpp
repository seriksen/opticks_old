#pragma once

#include <vector>
#include <string>
#include <cassert>

#include "NPY_FLAGS.hh"

// dont include NGLM.hpp here as it causes problem for thrustrap-
//#include "NGLM.hpp"
#include <glm/fwd.hpp>


class NMeta ; 
class NLookup ; 
class NPYSpec ; 
#include "NPY_API_EXPORT.hh"

/**
NPYBase
==========

Load/Make return NPYBase for generic handling, but 
they use type specialized NPY<T> buffers internally.
dynamic_cast<NPY<T>*>(buf) them when needed.

**/

class NPY_API NPYBase {
   public:
       typedef enum { FLOAT, SHORT, DOUBLE, INT, UINT, CHAR, UCHAR, ULONGLONG} Type_t ;
   public:
       static const char* ArrayContentVersion ; 
       static NPYBase* Load( const char* path, Type_t type );
       static NPYBase* Make( unsigned ni, const NPYSpec* itemspec, bool zero );
   private:
       static bool NPDump ; 
   public:
       static void SetNPDump(bool npdump); 
       static bool IsNPDump(); 
   public:

       static const char* FLOAT_ ; 
       static const char* SHORT_ ; 
       static const char* DOUBLE_ ; 
       static const char* INT_ ; 
       static const char* UINT_ ; 
       static const char* CHAR_ ; 
       static const char* UCHAR_ ; 
       static const char* ULONGLONG_ ; 

       static const char* TypeName(Type_t type);

       static bool GLOBAL_VERBOSE ; 
       static int checkNumItems(NPYBase* data);
   public:
        NPYBase(const std::vector<int>& shape, unsigned char sizeoftype, Type_t type, std::string& metadata, bool has_data);
        virtual ~NPYBase();
        void setHasData(bool has_data=true);
        bool hasData() const ;
        static void transfer(NPYBase* dst, NPYBase* src); 
   public:
        const char* getName() const ; 
        void setName(const char* name); 
   public:
       // shape related
       const NPYSpec* getShapeSpec() const ;
       const NPYSpec* getItemSpec() const ;
       const std::vector<int>& getShapeVector() const ;

       bool hasSameShape(NPYBase* other, unsigned fromdim=0) const ;
       bool hasShape(int ni, int nj=0, int nk=0, int nl=0, int nm=0) const ; // -1 for anything 
       bool hasItemShape(int nj, int nk=0, int nl=0, int nm=0) const ;
  
       static bool HasShape( NPYBase* a, int ni, int nj=0, int nk=0, int nl=0, int nm=0) ; // -1 for anything 

       bool hasShapeSpec(const NPYSpec* spec) const ; 
       bool hasItemSpec(const NPYSpec* spec) const ; 

       virtual void zero() = 0 ; 

       std::string  getItemShape(unsigned int ifr=1) const ;
       std::string  getDigestString()  ;
       std::string  getItemDigestString(unsigned i);

       static std::string  getDigestString(void* bytes, unsigned int nbytes) ; 
       bool isEqualTo(void* bytes, unsigned int nbytes) ;
       bool isEqualTo(NPYBase* other) ;
       unsigned int getNumQuads() const ;  // product of all dimensions excluding the last, which must be 4 
       unsigned int getNumItems(int ifr=0, int ito=1) const;  // default ifr/ito=0/1 is size of 1st dimension
       unsigned int getNumElements() const ;   // size of last dimension
       unsigned int getDimensions() const ;
       std::string  getShapeString(unsigned int ifr=0) const ;
       unsigned int getShape(int dim) const ;
       unsigned int getValueIndex(unsigned i, unsigned j, unsigned k, unsigned l=0, unsigned m=0) const ;
       unsigned int getNumValues(unsigned int from_dim=0) const ;

       NMeta*        getParameters() const ;

       template <typename T> void setParameter(const char* key, T value);
       template <typename T> T getParameter(const char* key, const char* fallback) const ;

       void setMeta(NMeta* meta); 
       template <typename T> void setMeta(const char* key, T value);
       template <typename T> T getMeta(const char* key, const char* fallback) const ;
       int getArrayContentVersion() const ;
       void setArrayContentVersion(int acv);

   public:
       void saveMeta( const char* path, const char* ext=".json") const ; 
       static NMeta* LoadMeta( const char* path, const char* ext=".json"); 

   public:
       // depending on sizeoftype
       Type_t        getType() const ;
       bool          isIntegerType() const ;
       bool          isFloatType() const ;
       unsigned char getSizeOfType() const ;
       unsigned int  getNumBytes(unsigned int from_dim=0) const ;
       unsigned int  getByteIndex(unsigned i, unsigned j, unsigned k, unsigned l=0, unsigned m=0) const ;
   public:
       void reshape(int ni, unsigned nj=0, unsigned nk=0, unsigned nl=0, unsigned nm=0);
   private:
       void init();
       void updateDimensions();
   public:
       // OpenGL related
       void         setBufferId(int buffer_id);
       int          getBufferId() const ;  // either -1 if not uploaded, or the OpenGL buffer Id
       bool         isComputeBuffer() const ;
       bool         isInteropBuffer() const ;

       void         setBufferTarget(int buffer_target);
       int          getBufferTarget() const ;  // -1 if unset

       void         setBufferControl(unsigned long long  buffer_control);
       unsigned long long getBufferControl() const ;
       unsigned long long* getBufferControlPtr()  ;

       void         setActionControl(unsigned long long  action_control);
       void         addActionControl(unsigned long long  action_control);
       unsigned long long  getActionControl() const ;
       unsigned long long* getActionControlPtr() ;

       void     setLookup(NLookup* lookup);   // needed for legacy gensteps
       NLookup* getLookup() const ;

       void         setBufferSpec(const NPYSpec* spec);
       const NPYSpec* getBufferSpec() const ;
       const char*  getBufferName() const ;

       void         setAux(void* aux);
       void*        getAux() const ;


       void         setDynamic(bool dynamic=true);
       bool         isDynamic() const ;    // used by oglrap-/Rdr::upload

       bool isGenstepTranslated() const ;
       void setGenstepTranslated(bool flag=true);

       unsigned getNumHit() const ;
       void setNumHit(unsigned num_hit);
   private:
       void         setBufferName(const char* name);
   public:
       // NumPy static persistency path/dir providers moved to brap-/BOpticksEvent

       void setVerbose(bool verbose=true);
       void setAllowPrealloc(bool allow=true); 
       static void setGlobalVerbose(bool verbose=true);

   public:
       // provided by subclass
       virtual void read(void* ptr) = 0;
       virtual void* getBytes() = 0 ;

       virtual void setQuad(const glm::vec4& vec, unsigned int i, unsigned int j, unsigned int k) = 0 ;
       virtual void setQuad(const glm::ivec4& vec, unsigned int i, unsigned int j, unsigned int k) = 0 ;

       virtual glm::vec4  getQuad(unsigned int i, unsigned int j, unsigned int k ) const  = 0 ; 
       virtual glm::ivec4 getQuadI(unsigned int i, unsigned int j, unsigned int k ) const = 0 ; 

       virtual void save(const char* path) = 0;
       virtual void save(const char* dir, const char* name) = 0;
   public:
       // from BOpticksEvent
       virtual void save(const char* pfx, const char* tfmt, const char* targ, const char* tag, const char* det ) = 0;
 
    public:
       void Summary(const char* msg="NPYBase::Summary") const ;
       std::string description(const char* msg="NPYBase::description") const ;

   //protected:
    public:
       void setNumItems(unsigned int ni);

   protected:
       std::vector<int>   m_shape ; 
       unsigned char      m_sizeoftype ; 
       Type_t             m_type ; 
       std::string        m_metadata ; 
       bool               m_has_data ;
       unsigned int       m_ni ; 
       unsigned int       m_nj ; 
       unsigned int       m_nk ; 
       unsigned int       m_nl ; 
       unsigned int       m_nm ;
       unsigned int       m_dim ; 
       const NPYSpec*     m_shape_spec ; 
       const NPYSpec*     m_item_spec ; 
       const NPYSpec*     m_buffer_spec ; 
       int                m_buffer_id ; 
       int                m_buffer_target ; 
       unsigned long long m_buffer_control ; 
       const char*        m_buffer_name ; 
       unsigned long long m_action_control ; 
       void*              m_aux ; 
       bool               m_verbose ; 
       bool               m_allow_prealloc ; 
   private:
       bool               m_dynamic ;
       NLookup*           m_lookup ;   // only needed for legacy gensteps 

#ifdef OLD_PARAMETESR
       X_BParameters*     m_parameters ;  // for keeping notes, especially for gensteps
#else
       NMeta*             m_parameters ;  // for keeping notes, especially for gensteps
#endif

       NMeta*             m_meta ; 

       const char*        m_name ; 

};


