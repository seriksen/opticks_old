#include "AxisApp.hh"

#include "OGLRAP_LOG.hh"
#include "PLOG.hh"

int main(int argc, char** argv)
{
    PLOG_(argc, argv);
    OGLRAP_LOG__ ; 

    LOG(info) << argv[0] ; 

    AxisApp aa(argc, argv); 

    aa.renderLoop();

    return 0 ; 
}
