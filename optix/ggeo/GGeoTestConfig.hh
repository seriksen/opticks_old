#pragma once

#include <cstddef>
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

struct NSlice ; 

class GGeoTestConfig {
    public:
       typedef enum { 
                      MODE, 
                      FRAME, 
                      BOUNDARY, 
                      PARAMETERS, 
                      SHAPE, 
                      SLICE, 
                      ANALYTIC, 
                      DEBUG,
                      UNRECOGNIZED } Arg_t ;

       typedef std::pair<std::string,std::string> KV ; 
       static const char* DEFAULT_CONFIG ; 
    public:
       static const char* MODE_; 
       static const char* FRAME_ ; 
       static const char* BOUNDARY_ ; 
       static const char* PARAMETERS_ ; 
       static const char* SHAPE_ ; 
       static const char* SLICE_ ; 
       static const char* ANALYTIC_ ; 
       static const char* DEBUG_ ; 
    public:
       GGeoTestConfig(const char* config);
    private:
       void init(const char* config);
       void configure(const char* config);
       Arg_t getArg(const char* k);
       void set(Arg_t arg, const char* s);
    private:
       void setMode(const char* s);
       void setFrame(const char* s);
       void setSlice(const char* s);
       void setAnalytic(const char* s);
       void setDebug(const char* s);
    private:
       void addShape(const char* s);
       void addBoundary(const char* s);
       void addParameters(const char* s);
    public:
       const char* getBoundary(unsigned int i);
       glm::vec4 getParameters(unsigned int i);
       char      getShape(unsigned int i);

       NSlice*   getSlice();
       bool      getAnalytic();
       const char* getMode();
       unsigned int getNumElements();

       std::vector<std::pair<std::string, std::string> >& getCfg();
       void dump(const char* msg="GGeoTestConfig::dump");
   private:
       unsigned int getNumBoundaries();
       unsigned int getNumParameters();
       unsigned int getNumShapes();
   private:
       const char*  m_config ; 
       const char*  m_mode ; 
       NSlice*      m_slice ; 
       glm::ivec4   m_frame ;
       glm::ivec4   m_analytic ;
       glm::vec4    m_debug ;
       std::vector<std::string> m_shapes ; 
       std::vector<std::string> m_boundaries ; 
       std::vector<glm::vec4>   m_parameters ; 
       std::vector<KV> m_cfg ; 
};

inline GGeoTestConfig::GGeoTestConfig(const char* config) 
    : 
    m_config(NULL),
    m_mode(NULL),
    m_slice(NULL),
    m_frame(0,0,0,0),
    m_analytic(0,0,0,0),
    m_debug(1.f,0.f,0.f,0.f)
{
    init(config);
}

inline std::vector<std::pair<std::string, std::string> >& GGeoTestConfig::getCfg()
{
    return m_cfg ; 
}
inline NSlice* GGeoTestConfig::getSlice()
{
    return m_slice ; 
}
inline unsigned int GGeoTestConfig::getNumBoundaries()
{
    return m_boundaries.size();
}
inline unsigned int GGeoTestConfig::getNumParameters()
{
    return m_parameters.size() ; 
}
inline unsigned int GGeoTestConfig::getNumShapes()
{
    return m_shapes.size() ; 
}

inline bool GGeoTestConfig::getAnalytic()
{
    bool analytic = m_analytic.x > 0 ;
    return analytic ; 
}
inline const char* GGeoTestConfig::getMode()
{
    return m_mode ; 
}


