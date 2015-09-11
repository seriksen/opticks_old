#include <thrust/device_vector.h>
#include "ThrustHistogram.hh"
#include "NPY.hpp"
#include "Index.hpp"
#include "assert.h"

/*
   target is typically   uchar4 so can access 
   as quad from OpenGL with entries 
   for  history/material/spare/spare idx 
   
   to populate such a structure need to specify 

       itemsize : 4  <-- the stride :w
       offset   : 0 (history) 1 (material)
       
*/

int main()
{
    // sling histories onto device and create histogram 

    typedef unsigned long long T ;
    typedef unsigned char S ;

    NPY<T>* history = NPY<T>::load("/tmp/thv.npy");

    std::vector<T>& his = history->data();
    unsigned int num_elements = his.size();

    thrust::device_vector<T> dhis(his.begin(), his.end());
    T* dhis_ptr = dhis.data().get();       
    T* dhis_ptr_alt = thrust::raw_pointer_cast(&dhis[0]);
    assert( dhis_ptr == dhis_ptr_alt );    // two eqivalent ways to access raw device pointers


    //thrust::device_vector<S> dtgt(size); // <--- cannot do in .cpp, needs to be in .cu https://github.com/thrust/thrust/issues/526

    unsigned int target_offset = 0 ; 
    unsigned int target_itemsize = 1 ; 
    unsigned int sequence_offset = 0 ; 
    unsigned int sequence_itemsize = 1 ; 

    std::vector<S> tgt(num_elements*target_itemsize);  
    thrust::device_vector<S> dtgt(tgt.begin(), tgt.end()); 
    S* dtgt_ptr = dtgt.data().get();       

    // above creates device vectors to mimic the actual situation 
    // of addressing OpenGL/OptiX buffers 

    ThrustHistogram<T,S> th("ThrustHistogramHistory", num_elements, sequence_itemsize, sequence_offset, target_itemsize, target_offset );
     // NB target must be itemsize*size of history 

    th.createHistogram( dhis );

    th.dumpHistogram();

    th.apply(dhis, dtgt); 


    thrust::host_vector<S> htgt = dtgt ;                // full pullback, expensive 

    NPY<S>* target = NPY<S>::make(htgt.size(), 1, 1, htgt.data()); 
    target->setVerbose();
    target->save("/tmp/ThrustHistogramTest.npy");


    Index* index = th.getIndex();
    index->dump(); 


    cudaDeviceSynchronize();

    return 0 ; 
}

/*

In [1]: t = np.load("/tmp/thv.npy")

In [2]: t.shape
Out[2]: (612841, 1, 1)

In [3]: t[:,0,0]
Out[3]: array([19649, 15297,    65, ...,     3,     3,     3], dtype=uint64)

In [4]: tt = t[:,0,0]

In [5]: map(hex_,tt[:1000])
Out[5]: 
['0x4cc1',
 '0x3bc1',
 '0x41',
 '0x3bc1',
 '0x3bc1',
 '0x4c1',
 '0x4c1',
 '0x4c1',
 '0x41',
 '0x41',
 '0x41',
 '0x4cbc1',
 '0x4c1',
 '0x4c1',
 '0x4c1',
  ...

n [8]: utt = np.unique(tt)

In [9]: len(utt)
Out[9]: 2307




*/

