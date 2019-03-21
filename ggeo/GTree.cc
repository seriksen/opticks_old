#include "NSensor.hpp"
#include "NPY.hpp"

#include "GNode.hh"
#include "GVolume.hh"
#include "GTree.hh"

#include "PLOG.hh"

const plog::Severity GTree::LEVEL = debug ; 


NPY<float>* GTree::makeInstanceTransformsBuffer(const std::vector<GNode*>& placements)
{
    // collecting transforms from GNode instances into a buffer
    // getPlacement for ridx=0 just returns m_root (which always has identity transform)
    // for ridx > 0 returns all GNode instances 

    unsigned int ni = placements.size(); 
    NPY<float>* buf = NPY<float>::make(0, 4, 4);
    for(unsigned int i=0 ; i < ni ; i++)
    {
        GNode* place = placements[i] ;
        GMatrix<float>* t = place->getTransform();
        buf->add(t->getPointer(), 4*4*sizeof(float) );
    } 
    assert(buf->getNumItems() == ni);
    return buf ; 
}


NPY<unsigned int>* GTree::makeAnalyticInstanceIdentityBuffer(const std::vector<GNode*>& placements) 
{
    // collect identity information for each of the repeated nodes (or subtrees)
    // eg PMT sensor index

    unsigned int numInstances = placements.size() ;

    NPY<unsigned int>* buf = NPY<unsigned int>::make(numInstances, 1, 4); // huh non-analytic uses (-1,4)
    buf->zero(); 

    // NB the differences:
    //
    //    analytic
    //         identity buffer has numInstances items (ie one entry for each repeated instance)
    //
    //    triangulated:  
    //         identity buffer has numInstances*numVolumes items (ie one entry for every volume of every instance)
    //         ... downstream this gets repeated further to every triangle
    //

    std::vector<GNode*>& progeny0 = placements[0]->getProgeny();
    unsigned numProgeny0 = placements[0]->getLastProgenyCount();
    assert( progeny0.size() == numProgeny0 );


    unsigned int numVolumes  = numProgeny0 + 1 ; 

    // observe that each instance has only one sensor, so not need 
    // to repeat over the number of volumes just one entry per instance

    LOG(LEVEL) 
              << " numPlacements " << numInstances
              << " numProgeny0 " << numProgeny0      
              << " numVolumes " << numVolumes      
              ;

    for(unsigned int i=0 ; i < numInstances ; i++) // over instances of the same geometry
    {
        GNode* base = placements[i] ;
        unsigned ridx = base->getRepeatIndex();

        std::vector<GNode*>& progeny = base->getProgeny();
        unsigned numProgeny = base->getLastProgenyCount();
        assert( numProgeny == numProgeny0 );      // repeated geometry for the instances, so the progeny counts must match 

        bool progeny_match = progeny.size() == numProgeny ;

        //if(ridx > 0)
        {
            if(!progeny_match)
               LOG(fatal) 
                          << " progeny MISMATCH "
                          << " progeny.size() " << progeny.size() 
                          << " numProgeny " << numProgeny
                          << " numInstances " << numInstances
                          << " numVolumes " << numVolumes
                          << " i " << i 
                          << " ridx " << ridx
                          ;

            assert( progeny_match );
        }

      
        NSensor* sensor = NULL ;  
        for(unsigned int s=0 ; s < numVolumes ; s++ )  // loop over volumes looking for a sensor
        {
            GNode* node = s == 0 ? base : progeny[s-1] ; 
            GVolume* volume = dynamic_cast<GVolume*>(node) ;
            NSensor* ss = volume->getSensor();
            //assert(ss); dont have JUNO sensor info

            unsigned int sid = ss && ss->isCathode() ? ss->getId() : 0 ;

            if(sid > 0)
            LOG(debug) 
                      << " s " << std::setw(3) << s 
                      << " sid " << std::setw(10) << std::hex << sid << std::dec 
                      << " ss " << (ss ? ss->description() : "NULL" )
                      ;

            if(sid > 0 && numInstances > 1)  // formerly && ridx > 0 
            {
                assert(sensor == NULL && "not expecting more than one sensor volume with non-zero id within an instance of repeated geometry");
                sensor = ss ; 
            }
        }

        glm::uvec4 aii ; 

        aii.x = base->getIndex();        
        aii.y = i ;  // instance index (for triangulated this contains the mesh index)
        aii.z = 0 ;  // formerly boundary, but with analytic have broken 1-1 volume/boundary relationship so boundary must live in partBuffer
        aii.w = NSensor::RefIndex(sensor) ;  // the only critical one 

        buf->setQuadU(aii, i, 0); 
        
    }
    return buf ; 
}

NPY<unsigned int>* GTree::makeInstanceIdentityBuffer(const std::vector<GNode*>& placements) 
{
    /*
     Repeating identity guint4 for all volumes of an instance (typically ~5 volumes for 1 instance)
     into all the instances (typically large 500-36k).


     Instances need to know the sensor they correspond 
     even though their geometry is duplicated. 

     For analytic geometry this is needed at the volume level 
     ie need buffer of size:
             #transforms * #volumes-per-instance

     For triangulated geometry this is needed at the triangle level
     ie need buffer of size 
             #transforms * #triangles-per-instance

     The triangulated version can be created from the analytic one
     by duplication according to the number of triangles.

    */

    unsigned int numInstances = placements.size() ;


    std::vector<GNode*>& progeny0 = placements[0]->getProgeny();
    unsigned numProgeny0 = placements[0]->getLastProgenyCount();
    assert( progeny0.size() == numProgeny0 );

    unsigned int numVolumes  = numProgeny0 + 1 ; 
    unsigned int num = numVolumes*numInstances ; 

    NPY<unsigned int>* buf = NPY<unsigned int>::make(0, 4);

    for(unsigned int i=0 ; i < numInstances ; i++)
    {
        GNode* base = placements[i] ;

        unsigned ridx = base->getRepeatIndex();


        std::vector<GNode*>& progeny = base->getProgeny();
        unsigned numProgeny = base->getLastProgenyCount();
        assert( numProgeny == numProgeny0 && "repeated geometry for the instances, so the progeny counts must match");

        bool progeny_match = progeny.size() == numProgeny ;

        //if(ridx > 0)
        {
           if(!progeny_match)
           LOG(fatal) 
                      << " progeny_match " << ( progeny_match ? " OK " : " MISMATCH " )
                      << " progeny.size() " << progeny.size() 
                      << " numProgeny " << numProgeny
                      << " numInstances " << numInstances
                      << " numVolumes " << numVolumes
                      << " i " << i 
                      << " ridx " << ridx
                      ;


            assert( progeny_match );
        }

        for(unsigned int s=0 ; s < numVolumes ; s++ )
        {
            GNode* node = s == 0 ? base : progeny[s-1] ; 
            GVolume* volume = dynamic_cast<GVolume*>(node) ;

            guint4 id = volume->getIdentity();
            buf->add(id.x, id.y, id.z, id.w ); 

#ifdef DEBUG
            std::cout  
                  << " i " << i
                  << " s " << s
                  << " node/mesh/boundary/sensor " << id.x << "/" << id.y << "/" << id.z << "/" << id.w 
                  << " nodeName " << node->getName()
                  << std::endl 
                  ;
#endif
        }
    }
    assert(buf->getNumItems() == num);
    return buf ;  
}

/*

::

    In [1]: ii = np.load("iidentity.npy")

    In [3]: ii.shape
    Out[3]: (3360, 4)

    In [4]: ii.reshape(-1,5,4)
    Out[4]: 
    array([[[ 3199,    47,    19,     1],
            [ 3200,    46,    20,     2],
            [ 3201,    43,    21,     3],
            [ 3202,    44,     1,     4],
            [ 3203,    45,     1,     5]],

           [[ 3205,    47,    19,     6],
            [ 3206,    46,    20,     7],
            [ 3207,    43,    21,     8],
            [ 3208,    44,     1,     9],
            [ 3209,    45,     1,    10]],

After requiring an associated sensor surface to provide the sensor index, only cathodes 
have non-zero index::

    In [1]: ii = np.load("iidentity.npy")

    In [2]: ii.reshape(-1,5,4)
    Out[2]: 
    array([[[ 3199,    47,    19,     0],
            [ 3200,    46,    20,     0],
            [ 3201,    43,    21,     3],
            [ 3202,    44,     1,     0],
            [ 3203,    45,     1,     0]],

           [[ 3205,    47,    19,     0],
            [ 3206,    46,    20,     0],
            [ 3207,    43,    21,     8],
            [ 3208,    44,     1,     0],
            [ 3209,    45,     1,     0]],
*/


