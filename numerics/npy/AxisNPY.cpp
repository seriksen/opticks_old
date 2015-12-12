#include "AxisNPY.hpp"
#include "NPY.hpp"

#include <iostream>
#include <iomanip>
#include <algorithm>

#include <glm/glm.hpp>
#include "GLMFormat.hpp"
#include "GLMPrint.hpp"


#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL
// trace/debug/info/warning/error/fatal


void AxisNPY::dump(const char* msg)
{
    if(!m_axis) return ;
    LOG(info) << msg ; 

    unsigned int ni = m_axis->m_ni ;
    unsigned int nj = m_axis->m_nj ;
    unsigned int nk = m_axis->m_nk ;
    assert( nj == 3 && nk == 4 );


    for(unsigned int i=0 ; i < ni ; i++)
    {

        glm::vec4 vpos = m_axis->getQuad(i,0);
        glm::vec4 vdir = m_axis->getQuad(i,1);
        glm::vec4 vcol = m_axis->getQuad(i,2);

        printf("%2u %s %s %s  \n", 
                i, 
                gpresent(vpos,2,11).c_str(),
                gpresent(vdir,2,7).c_str(),
                gpresent(vcol,2,7).c_str()
             );
    }

}


