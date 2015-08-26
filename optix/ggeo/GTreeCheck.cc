#include  "GTreeCheck.hh"
#include "GGeo.hh"
#include "GSolid.hh"
#include "GMatrix.hh"
#include "GBuffer.hh"
#include "Counts.hpp"

#include <iomanip>

#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL
// trace/debug/info/warning/error/fatal

void GTreeCheck::dumpTree(const char* msg)
{
    LOG(info) << msg ; 
    dumpTraverse(m_root, 0);
}
void GTreeCheck::dumpTraverse( GNode* node, unsigned int depth )
{
    GSolid* solid = dynamic_cast<GSolid*>(node) ;
    unsigned int ridx = node->getRepeatIndex();   
    printf(" %7d : %3d : %3d : %s \n", m_dump_count, depth, ridx, node->getName());
    m_dump_count++ ; 
    for(unsigned int i = 0; i < node->getNumChildren(); i++) dumpTraverse(node->getChild(i), depth + 1 );
}


void GTreeCheck::init()
{
    m_root = m_ggeo->getSolid(0);
    m_digest_count = new Counts<unsigned int>("progenyDigest");
}

void GTreeCheck::traverse()
{
    traverse(m_root, 0);

    m_digest_count->sort(false);
    //m_digest_count->dump();

    // minrep 120 removes repeats from headonPMT, calibration sources and RPC leaving just PMTs 
    findRepeatCandidates(120); 
    dumpRepeatCandidates();
}

void GTreeCheck::traverse( GNode* node, unsigned int depth)
{
    GSolid* solid = dynamic_cast<GSolid*>(node) ;

    //bool selected = solid->isSelected();
    //if(selected)
    {
        GMatrixF* gtransform = solid->getTransform();
        GMatrixF* ltransform = solid->getLevelTransform();
        GMatrixF* ctransform = solid->calculateTransform();

        float delta = gtransform->largestDiff(*ctransform);

        std::string& pdig = node->getProgenyDigest();
        unsigned int nprogeny = node->getProgenyCount() ;
        
        m_digest_count->add(pdig.c_str());

        if(nprogeny > 0 ) 
            LOG(debug) 
              << "GTreeCheck::traverse " 
              << " count "     << std::setw(6) << m_count
              << " #progeny "  << std::setw(6) << nprogeny 
              << " pdig "      << std::setw(32) << pdig 
              << " delta*1e6 " << std::setprecision(6) << std::fixed << delta*1e6 
              << " name " << node->getName() 
              ;

        assert(delta < 1e-6) ;
        m_count++ ; 
    }

    for(unsigned int i = 0; i < node->getNumChildren(); i++) traverse(node->getChild(i), depth + 1 );
}

void GTreeCheck::dumpRepeatCandidates()
{
    LOG(info) << "GTreeCheck::dumpRepeatCandidates " ;
    for(unsigned int i=0 ; i < m_repeat_candidates.size() ; i++) dumpRepeatCandidate(i) ;
}





void GTreeCheck::dumpRepeatCandidate(unsigned int index, bool verbose)
{
    std::string pdig = m_repeat_candidates[index];
    unsigned int ndig = m_digest_count->getCount(pdig.c_str());

    GNode* first = m_root->findProgenyDigest(pdig) ; // first node that matches the progeny digest
    std::vector<GNode*> placements = m_root->findAllProgenyDigest(pdig);
    std::cout  
                  << " pdig "  << std::setw(32) << pdig  
                  << " ndig "  << std::setw(6) << std::dec << ndig
                  << " nprog " <<  std::setw(6) << std::dec << first->getProgenyCount() 
                  << " placements " << std::setw(6) << placements.size()
                  << " n "          <<  first->getName() 
                  << std::endl 
                  ;  

    assert(placements.size() == ndig ); // restricting traverse to just selected causes this to fail
    if(verbose)
    {
        for(unsigned int i=0 ; i < placements.size() ; i++)
        {
            GNode* place = placements[i] ;
            GMatrix<float>* t = place->getTransform();
            std::cout 
                   << " t " << t->brief() 
                   << std::endl 
                   ;  
        }
    }
}

GNode* GTreeCheck::getRepeatExample(unsigned int ridx)
{
    assert(ridx >= 1); // ridx is a 1-based index
    if(ridx > m_repeat_candidates.size()) return NULL ; 
    std::string pdig = m_repeat_candidates[ridx-1];
    std::vector<GNode*> placements = m_root->findAllProgenyDigest(pdig);
    GNode* node = m_root->findProgenyDigest(pdig) ; // first node that matches the progeny digest
    assert(placements[0] == node);
    return node ; 
}

GBuffer* GTreeCheck::makeTransformsBuffer(unsigned int ridx) 
{
    assert(ridx >= 1); // ridx is a 1-based index
    if(ridx > m_repeat_candidates.size()) return NULL ; 

    std::string pdig = m_repeat_candidates[ridx-1];
    std::vector<GNode*> placements = m_root->findAllProgenyDigest(pdig);

    unsigned int num = placements.size() ; 
    unsigned int numElements = 16 ; 
    unsigned int size = sizeof(float)*numElements;
    float* transforms = new float[num*numElements];

    for(unsigned int i=0 ; i < placements.size() ; i++)
    {
        GNode* place = placements[i] ;
        GMatrix<float>* t = place->getTransform();
        t->copyTo(transforms + numElements*i); 
    } 

    LOG(info) << "GTreeCheck::makeTransformsBuffer " 
              << " ridx " << ridx 
              << " pdig " << pdig 
              << " num " << num 
              << " size " << size 
              ;

    GBuffer* buffer = new GBuffer( size*num, (void*)transforms, size, numElements ); 
    return buffer ;
}


void GTreeCheck::findRepeatCandidates(unsigned int minrep)
{
    for(unsigned int i=0 ; i < m_digest_count->size() ; i++)
    {
        std::pair<std::string,unsigned int>&  kv = m_digest_count->get(i) ;

        std::string& pdig = kv.first ; 
        unsigned int ndig = kv.second ;                 // number of occurences of the progeny digest 
        GNode* node = m_root->findProgenyDigest(pdig) ; // first node that matches the progeny digest


        //if( ndig > minrep )
        if( ndig > minrep && node->getProgenyCount() > 0 )
        {
            LOG(info) 
                  << "GTreeCheck::findRepeatCandidates "
                  << " pdig "  << std::setw(32) << pdig  
                  << " ndig "  << std::setw(6) << ndig
                  << " nprog " <<  std::setw(6) << node->getProgenyCount() 
                  << " n "     <<  node->getName() 
                  ;  

            m_repeat_candidates.push_back(pdig);
        }   
    }

    // erase repeats that are enclosed within other repeats 
    // ie that have an ancestor which is also a repeat candidate

    m_repeat_candidates.erase(
         std::remove_if(m_repeat_candidates.begin(), m_repeat_candidates.end(), *this ),
         m_repeat_candidates.end()
    ); 
    

}





bool GTreeCheck::operator()(const std::string& dig)  
{
    bool cr = isContainedRepeat(dig, 3);
 
    if(cr) LOG(info) 
                  << "GTreeCheck::operator() "
                  << " pdig "  << std::setw(32) << dig  
                  << " disallowd as isContainedRepeat "
                  ;

    return cr ;  
} 

bool GTreeCheck::isContainedRepeat( const std::string& pdig, unsigned int levels ) const 
{
    // for the first node that matches the *pdig* progeny digest
    // look back *levels* ancestors to see if any of the immediate ancestors 
    // are also repeat candidates, if they are then this is a contained repeat
    // and is thus disallowed in favor of the ancestor that contains it 

    GNode* node = m_root->findProgenyDigest(pdig) ; 
    std::vector<GNode*>& ancestors = node->getAncestors();
    unsigned int asize = ancestors.size(); 

    for(unsigned int i=0 ; i < std::min(levels, asize) ; i++)
    {
        GNode* a = ancestors[asize - 1 - i] ;
        std::string& adig = a->getProgenyDigest();
        if(std::find(m_repeat_candidates.begin(), m_repeat_candidates.end(), adig ) != m_repeat_candidates.end())
        { 
            return true ;
        }
    }
    return false ; 
} 



unsigned int GTreeCheck::getRepeatIndex(const std::string& pdig )
{
     unsigned int index(0);
     std::vector<std::string>::iterator it = std::find(m_repeat_candidates.begin(), m_repeat_candidates.end(), pdig );
     if(it != m_repeat_candidates.end())
     {
         index = std::distance(m_repeat_candidates.begin(), it ) + 1;  // 1-based index
         LOG(debug)<<"GTreeCheck::getRepeatIndex " 
                  << std::setw(32) << pdig 
                  << " index " << index 
                  ;
     }
     return index ; 
}



void GTreeCheck::labelTree()
{
    m_labels = 0 ; 

    for(unsigned int i=0 ; i < m_repeat_candidates.size() ; i++)
    {
         std::string pdig = m_repeat_candidates[i];
         unsigned int ridx = getRepeatIndex(pdig);
         assert(ridx == i + 1 );
         std::vector<GNode*> placements = m_root->findAllProgenyDigest(pdig);

         // recursive labelling starting from the placements
         for(unsigned int p=0 ; p < placements.size() ; p++)
         {
             labelTree(placements[p], ridx);
         }
    }

    LOG(info)<<"GTreeCheck::labelTree count of non-zero setRepeatIndex " << m_labels ; 
}

void GTreeCheck::labelTree( GNode* node, unsigned int ridx)
{
    node->setRepeatIndex(ridx);
    if(ridx > 0)
    {
         LOG(debug)<<"GTreeCheck::labelTree "
                  << " ridx " << std::setw(5) << ridx
                  << " n " << node->getName()
                  ;
         m_labels++ ; 
    }
    for(unsigned int i = 0; i < node->getNumChildren(); i++) labelTree(node->getChild(i), ridx );
}






