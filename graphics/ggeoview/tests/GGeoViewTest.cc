#include <cstdio>
#include <iostream>

#include "NGLM.hpp"
#include "App.hh"
#include "PLOG.hh"

#include "BRAP_LOG.hh"
#include "NPY_LOG.hh"
#include "OKCORE_LOG.hh"
#include "GGEO_LOG.hh"
#include "ASIRAP_LOG.hh"
#include "MESHRAP_LOG.hh"
#include "OKGEO_LOG.hh"
#include "OGLRAP_LOG.hh"
#include "GGV_LOG.hh"




int main(int argc, char** argv)
{
    PLOG_(argc, argv);

    BRAP_LOG_ ;
    NPY_LOG_ ;
    OKCORE_LOG_ ;
    GGEO_LOG_ ;
    ASIRAP_LOG_ ;
    MESHRAP_LOG_ ;
    OKGEO_LOG_ ;
    OGLRAP_LOG_ ;
    GGV_LOG_ ;
 

    App app("OPTICKS_", argc, argv);     

    app.initViz();

    app.configure(argc, argv);    // NumpyEvt created in App::config, 
    if(app.isExit()) 
    {
        std::cerr << "app exit after configure" << std::endl ;        
        exit(EXIT_SUCCESS);
    }

    app.prepareViz();      // setup OpenGL shaders and creates OpenGL context (the window)

    app.loadGeometry();    // creates GGeo instance, loads, potentially modifies for (--test) and registers geometry
    if(app.isExit()) exit(EXIT_SUCCESS);

    app.uploadGeometryViz();      // Scene::uploadGeometry, hands geometry to the Renderer instances for upload



    bool load = app.hasOpt("load");

    bool nooptix = app.hasOpt("nooptix");

    if(!nooptix && !load)
    {
        app.loadGenstep();             // hostside load genstep into NumpyEvt

        app.targetViz();               // point Camera at gensteps 

        app.uploadEvtViz();            // allocates GPU buffers with OpenGL glBufferData


#ifdef WITH_OPTIX
        app.prepareOptiX();            // places geometry into OptiX context with OGeo 

        app.prepareOptiXViz();         // creates ORenderer, OTracer

        if(!app.hasOpt("noevent"))
        {
            app.setupEventInEngine();
 
            app.preparePropagator();       // creates OptiX buffers and OBuf wrappers as members of OPropagator

            app.seedPhotonsFromGensteps(); // distributes genstep indices into the photons buffer

            app.initRecords();             // zero records buffer

            app.propagate();

            if(!app.hasOpt("noindex")) 
            {
                app.indexEvt();

                app.indexPresentationPrep();
            } 

            if(app.hasOpt("save") && !app.hasOpt("load"))
            {
                app.saveEvt();

                app.indexEvtOld();  // indexing that depends on downloading to host 
            }
        }
#endif

    }
    else if(load)
    {

#ifdef WITH_OPTIX
        if(app.hasOpt("optixviz"))
        {
            app.prepareOptiX();            // places geometry into OptiX context with OGeo 

            app.prepareOptiXViz();         // creates ORenderer, OTracer

            app.setupEventInEngine();   // for indexing 
        } 
#endif

        app.loadEvtFromFile();

        // huh maybe need to indexEvt if the indices are not loaded, 
        // eg when running with cfg4- no indices are persisted by the save as 
        // do that without assuming OptiX available

        app.indexEvt() ; // this skips if already indexed, and it handles loaded evt 

        app.indexPresentationPrep();

        app.uploadEvtViz();               // allocates GPU buffers with OpenGL glBufferData
    }

    app.prepareGUI();

    app.renderLoop();

    app.cleanup();

    exit(EXIT_SUCCESS);
}
