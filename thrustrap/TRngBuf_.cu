#include <iostream>
#include <iomanip>

#include <thrust/for_each.h>
#include <thrust/iterator/counting_iterator.h> 
#include <curand_kernel.h> 

#include "TRngBuf.hh"

#include "NPY.hpp"
#include "PLOG.hh"

TRngBuf::TRngBuf(unsigned ni, unsigned nj, CBufSpec spec, unsigned long long seed, unsigned long long offset )
    :
    TBuf("trngbuf", spec, "\n" ),
    m_ni(ni),
    m_nj(nj),
    m_num_elem(ni*nj),
    m_id_offset(0),
    m_id_max(1000),
    m_seed(seed),
    m_offset(offset),
    m_dev((float*)getDevicePtr())
{
}


void TRngBuf::generate(unsigned id_offset, unsigned id_0, unsigned id_1)
{
    m_id_offset = id_offset ;  
    thrust::for_each( 
          thrust::counting_iterator<unsigned>(id_0), 
          thrust::counting_iterator<unsigned>(id_1), 
           *this);
}

void TRngBuf::generate()
{

    std::cout << "TRngBuf::generate"
              << " ni " << m_ni
              << " id_max " << m_id_max
              << std::endl  
              ;

    unsigned seq = 0 ; 
    unsigned id_offset = 0 ; 
    while( id_offset < m_ni )
    {
        unsigned remaining = m_ni - id_offset ;    
        unsigned id_per_gen = m_id_max ; 
        if( id_per_gen > remaining ) id_per_gen = remaining ; 

        std::cout 
              << "TRngBuf::generate"
              << " seq " << seq
              << " id_offset " << std::setw(10) << id_offset
              << " id_per_gen " << std::setw(10) << id_per_gen
              << " remaining " << std::setw(10) << remaining
              << std::endl 
              ;

        generate( id_offset, 0, id_per_gen );

        id_offset += id_per_gen ; 
        seq++ ; 
    }
}

 
__device__ 
void TRngBuf::operator()(unsigned id) 
{ 
    unsigned uid = id + m_id_offset ; 

    curandState s; 
    curand_init(m_seed, uid , m_offset, &s); 

    for(unsigned j = 0; j < m_nj; ++j) 
    {
        unsigned uoffset = uid*m_nj+j ;
        if(uoffset < m_num_elem)
        {
            m_dev[uoffset] = curand_uniform(&s)  ; 
        }
    }
} 
 
