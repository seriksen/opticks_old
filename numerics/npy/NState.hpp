#pragma once

#include <map>
#include <cstring>
#include <string>
#include <cstdlib>
#include <vector>

//
// *NState* 
//       focusses on saving and restoring state of a collection of objects
//       jumping between states is handled at a higher level by
//       oglrap-/Bookmarks
//
// Main problem is the volume of state , impractical to include everything
// need to pick 
//
// Single canonical current NState instance is instanciated in Opticks::configure
// from App::configure and lodged into App::m_state
//
// Multiple NState instances corresponding to each ini files such as
//  ~/.opticks/rainbow/State/001.ini are managed by oglrap-/Bookmarks
//
// NState holds state strings for a collection of NConfigurable objects, grouped
// by prefix strings such as "camera" "clipper" "scene":: 
//
//        simon:State blyth$ cat 001.ini 
//        [camera]
//        far=6000.0000
//        near=120.0000
//        scale=120.0000
//        zoom=1.0000
//        [clipper]
//        cutnormal=1.0000,0.0000,0.0000
//        cutplane=1.0000,0.0000,0.0000,1.0000
//        cutpoint=0.0000,0.0000,0.0000
//        [scene]
//        target=0
//   
//

class NConfigurable ; 

class NState {
   public:
       static NState* load(const char* dir, unsigned int num);
   public:
       NState(const char* dir="/tmp", const char* name="state");
       const char* getDir();

       void addConfigurable(NConfigurable* configurable);
       void setVerbose(bool verbose=true);

       static std::string FormName(unsigned int num); // eg 001 002
       void setName(const char* name);
       void setName(unsigned int num);
       const char* getName();

       void Summary(const char* msg="NState::Summary");
       std::string description(const char* msg="NState::description");
   public:
       void roundtrip();
       void save();
       int load();

       void collect(); // collect state from configurables into m_kv
       void apply();   // apply state from m_kv to configurables
       void update();  // the state_string for GUI presentation

       const std::string& getStateString(bool update=false);
   private:
       void init();
       NConfigurable* getConfigurable(const char* prefix); 
       std::string get(const char* key);
       void set(const char* key, const char* val);
       std::string getFileName();
       std::string formKey(const char* prefix, const char* tag);
       unsigned int splitKey(std::vector<std::string>& prefix_tag, const char* key);

       void apply(const char* k, const char* v);
       unsigned int collect(NConfigurable* configurable);
       void setNumChanges(unsigned int num_changes);
       unsigned int getNumChanges();
       std::string stateString();
   private:
       bool                                  m_verbose ; 
       const char*                           m_dir ; 
       const char*                           m_name ; 
       unsigned int                          m_num_changes ; 
       std::map<std::string, std::string>    m_kv ; 
       std::map<std::string, NConfigurable*>  m_configurables ; 
       std::string                           m_state_string ; 

};

inline NState::NState(const char* dir, const char* name) 
    :
    m_verbose(false),
    m_dir(strdup(dir)),
    m_name(strdup(name)),
    m_num_changes(0)
{
    init();
}


inline void NState::setVerbose(bool verbose)
{
    m_verbose = verbose ; 
}

inline void NState::setName(const char* name)
{
    free((void*)m_name);
    m_name = strdup(name);
}

inline const char* NState::getName()
{
   return m_name ; 
}
inline const char* NState::getDir()
{
   return m_dir ; 
}


inline void NState::setNumChanges(unsigned int num_changes)
{
    m_num_changes = num_changes ; 
}
inline unsigned int NState::getNumChanges()
{
    return m_num_changes ; 
}
