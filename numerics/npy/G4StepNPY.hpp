#pragma once

#include "assert.h"
#include "uif.h"
#include <iostream>
#include <set>

#include "NPY.hpp"
class Lookup ; 

//
// hmm CerenkovStep and ScintillationStep have same shapes but different meanings see
//     /usr/local/env/chroma_env/src/chroma/chroma/cuda/cerenkov.h
//     /usr/local/env/chroma_env/src/chroma/chroma/cuda/scintillation.h
//
//  but whats needed for visualization should be in the same locations ?
//
//
// resist temptation to use inheritance here, 
// it causes much grief for little benefit 
// instead if needed use "friend class" status to 
// give G4StepNPY access to innards of NPY
//
 
class G4StepNPY {
   public:  
        typedef std::set<unsigned int> Set_t ; 
   public:  
       G4StepNPY(NPY<float>* npy); // weak reference to NPY* only
       NPY<float>* getNPY();

   public:  
       void setLookup(Lookup* lookup);
       Lookup* getLookup();
       void applyLookup(unsigned int jj, unsigned int kk);
       void dump(const char* msg);
       void dumpLines(const char* msg);

   public:  
       int getStepId(unsigned int i=0);
       bool isCerenkovStep(unsigned int i=0);
       bool isScintillationStep(unsigned int i=0);

  private:
       // the heart of the lookup:  int bcode = m_lookup->a2b(acode) ;
       bool applyLookup(unsigned int index);

  private:
        NPY<float>*  m_npy ; 
        Lookup*  m_lookup ; 
        Set_t    m_lines ;
 
};

inline NPY<float>* G4StepNPY::getNPY()
{
    return m_npy ; 
}
inline void G4StepNPY::setLookup(Lookup* lookup)
{
    m_lookup = lookup ;
} 
inline Lookup* G4StepNPY::getLookup()
{
    return m_lookup ;
} 

inline G4StepNPY::G4StepNPY(NPY<float>* npy) 
       :  
       m_npy(npy),
       m_lookup(NULL)
{
}


