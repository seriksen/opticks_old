/*
 * Copyright (c) 2019 Opticks Team. All Rights Reserved.
 *
 * This file is part of Opticks
 * (see https://bitbucket.org/simoncblyth/opticks).
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); 
 * you may not use this file except in compliance with the License.  
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 */

// ggv --bnd

#include <cassert>


#include "NGLM.hpp"
#include "NPY.hpp"

#include "Opticks.hh"

#include "GMaterialLib.hh"
#include "GSurfaceLib.hh"
#include "GBndLib.hh"
#include "GVector.hh"
#include "GItemList.hh"


#include "OPTICKS_LOG.hh"

const char* TMPDIR = "$TMP/ggeo/GBndLibTest" ; 


class GBndLibTest 
{
    public:
        GBndLibTest(GBndLib* blib) : m_blib(blib) {} ;
        void test_add();
    private:
        GBndLib* m_blib ; 
};

void GBndLibTest::test_add()
{
    const char* spec = "Vacuum/lvPmtHemiCathodeSensorSurface//Bialkali" ; // omat/osur/isur/imat
    //assert(blib->contains(spec));
    bool flip = true ; 
    m_blib->add(spec, flip);

    m_blib->setBuffer(m_blib->createBuffer());
    m_blib->getBuffer()->save(TMPDIR, "bbuf.npy");
}


int main(int argc, char** argv)
{
    OPTICKS_LOG(argc, argv);

    LOG(info) << argv[0] ; 

    Opticks ok(argc, argv) ;
    ok.configure(); 

    LOG(info) << " ok " ; 

    GBndLib* blib = GBndLib::load(&ok);

    LOG(info) << " loaded blib " ; 
    GMaterialLib* mlib = GMaterialLib::load(&ok);
    GSurfaceLib*  slib = GSurfaceLib::load(&ok);

    LOG(info) << " loaded all " 
              << " blib " << blib
              << " mlib " << mlib
              << " slib " << slib
              ;


    blib->setMaterialLib(mlib);
    blib->setSurfaceLib(slib);
    blib->dump();

    blib->dumpMaterialLineMap();

    assert( blib->getNames() == NULL && " expect NULL names before the close ") ; 

    blib->saveAllOverride(TMPDIR); // writing to geocache in tests not allowed, as needs to work from shared install

    assert( blib->getNames() != NULL && " expect non-NULL names after the close ") ; 

    blib->dumpNames("names");

    //test_add(blib);

 
    return 0 ; 
}



