#include "AssimpRegistry.hh"
#include "AssimpNode.hh"
#include <cassert>

#include "BLog.hh"
// trace/debug/info/warning/error/fatal


AssimpRegistry::AssimpRegistry()
{
}
AssimpRegistry::~AssimpRegistry()
{
}

void AssimpRegistry::add(AssimpNode* node)
{
   std::size_t digest = node->getDigest();
   AssimpNode* prior = lookup(digest);
   assert(!prior);
   m_registry[digest] = node ;
}

AssimpNode* AssimpRegistry::lookup(std::size_t digest)
{
   return m_registry.find(digest) != m_registry.end() ? m_registry[digest] : NULL ; 
}  

void AssimpRegistry::summary(const char* msg)
{
   LOG(info) << msg 
             << " size " << m_registry.size()
             ;
}


