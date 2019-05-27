#include <iomanip>
#include <sstream>
#include <cstring>
#include <vector>

#include <cuda.h>
#include <cuda_runtime_api.h>

// sysrap-
#include "S_freopen_redirect.hh"
#include "SSys.hh"
#include "SStr.hh"

// brap-
#include "BStr.hh"
#include "BFile.hh"
#include "BTimeStamp.hh"
//#include "STimes.hh"
#include "BTimes.hh"

// npy-
#include "NGLM.hpp"
#include "NPY.hpp"
#include "GLMFormat.hpp"

// okc-
#include "Opticks.hh"
#include "OpticksEntry.hh"
#include "OpticksBufferControl.hh"

// optixrap-
#include "SPPM.hh"
#include "OConfig.hh"
#include "OContext.hh"
#include "OError.hh"

#include "PLOG.hh"
using namespace optix ; 

const char* OContext::COMPUTE_ = "COMPUTE" ; 
const char* OContext::INTEROP_ = "INTEROP" ; 

const char* OContext::OPTIX_CACHE_LINUX = "/var/tmp/OptixCache" ; 
const char* OContext::CacheDir()  // static
{
   return OPTIX_CACHE_LINUX ;  
}



plog::Severity OContext::LEVEL = debug ; 


const char* OContext::getModeName() const 
{
    switch(m_mode)
    {
       case COMPUTE:return COMPUTE_ ; break ; 
       case INTEROP:return INTEROP_ ; break ; 
    }
    assert(0);
}
const char* OContext::getRunLabel() const 
{
    return m_runlabel ; 
}
const char* OContext::getRunResultsDir() const 
{
    return m_runresultsdir ; 
}

OpticksEntry* OContext::addEntry(char code)
{
    LOG(LEVEL) << "OContext::addEntry " << code ; 

    bool defer = true ; 
    unsigned index ;
    switch(code)
    { 
        case 'G': index = addEntry("generate.cu", "generate", "exception", defer) ; break ;
        case 'T': index = addEntry("generate.cu", "trivial",  "exception", defer) ; break ;
        case 'Z': index = addEntry("generate.cu", "zrngtest",  "exception", defer) ; break ;
        case 'N': index = addEntry("generate.cu", "nothing",  "exception", defer) ; break ;
        case 'R': index = addEntry("generate.cu", "tracetest",  "exception", defer) ; break ;
        case 'D': index = addEntry("generate.cu", "dumpseed", "exception", defer) ; break ;
        case 'S': index = addEntry("seedTest.cu", "seedTest", "exception", defer) ; break ;
        case 'P': index = addEntry("pinhole_camera.cu", "pinhole_camera" , "exception", defer);  break;
    }
    return new OpticksEntry(index, code) ; 
}


unsigned OContext::getDebugPhoton() const 
{
    return m_debug_photon ; 
}

Opticks* OContext::getOpticks() const 
{
    return m_ok ; 
}


// needed by rayleighTest
OConfig* OContext::getConfig() const 
{
    return m_cfg ; 
}



struct Device
{
   int index ; 
   char name[256] ;  
   int computeCaps[2];
   RTsize total_mem;
   int ordinal ;  

   Device(unsigned index_)
      :
      index(index_)
   {
        RT_CHECK_ERROR(rtDeviceGetAttribute(index, RT_DEVICE_ATTRIBUTE_NAME, sizeof(name), name));
        RT_CHECK_ERROR(rtDeviceGetAttribute(index, RT_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY, sizeof(computeCaps), &computeCaps));
        RT_CHECK_ERROR(rtDeviceGetAttribute(index, RT_DEVICE_ATTRIBUTE_TOTAL_MEMORY, sizeof(total_mem), &total_mem));
        RT_CHECK_ERROR(rtDeviceGetAttribute(index, RT_DEVICE_ATTRIBUTE_CUDA_DEVICE_ORDINAL, sizeof(ordinal), &ordinal));
   } 

   std::string desc() const 
   {
       std::stringstream ss ;  
       ss <<  "Device " << index << " " << std::setw(30) << name 
          << " ordinal " << ordinal 
          <<  " Compute Support: " << computeCaps[0] << " " << computeCaps[1] 
          <<  " Total Memory: " <<  (unsigned long long)total_mem
          ;
       return ss.str();
   }   
};


struct VisibleDevices 
{
    unsigned num_devices;
    unsigned version;
    std::vector<Device> devices ; 

    VisibleDevices()
    {
        RT_CHECK_ERROR(rtDeviceGetDeviceCount(&num_devices));
        RT_CHECK_ERROR(rtGetVersion(&version));
        for(unsigned i = 0; i < num_devices; ++i) 
        {
            Device d(i); 
            devices.push_back(d);     
        }  
    }

    std::string desc() const 
    {
       std::stringstream ss ;  
       for(unsigned i = 0; i < num_devices; ++i) ss << devices[i].desc() << std::endl ; 
       return ss.str();
    }
};


void OContext::CheckDevices()
{
    VisibleDevices vdev ; 
    LOG(info) << vdev.desc(); 

    const char* frame_renderer = Opticks::Instance()->getFrameRenderer();
    if( frame_renderer != NULL)
    {
        assert( vdev.num_devices == 1 && "expecting only a single visible device, the one driving the display, in interop mode") ; 
        const char* optix_device = vdev.devices[0].name ;
        LOG(info) << " frame_renderer " << frame_renderer ; 
        LOG(info) << " optix_device " << optix_device  ; 
        bool interop_device_match = SStr::Contains( frame_renderer, optix_device )  ; 
        assert( interop_device_match && "OpenGL and OptiX must be taking to the same single device in interop mode"  ); 
    }
    else
    {
        LOG(info) << " NULL frame_renderer : compute mode ? " ;  
    }
}



OContext* OContext::Create(Opticks* ok, const char* cmake_target, const char* ptxrel )
{
    int rtxmode = ok->getRTX();
#if OPTIX_VERSION_MAJOR >= 6
    InitRTX( rtxmode ); 
#else
    assert( rtxmode == 0 && "Cannot use --rtx 1/2/-1 options prior to OptiX 6.0.0 " ) ;
#endif

    CheckDevices();

    LOG(verbose) << "optix::Context::create() START " ; 
    optix::Context context = optix::Context::create();
    LOG(verbose) << "optix::Context::create() DONE " ; 

    OContext* ocontext = new OContext(context, ok, cmake_target, ptxrel );

    return ocontext ; 
}


#if OPTIX_VERSION_MAJOR >= 6
void OContext::InitRTX(int rtxmode)  // static
{
    if(rtxmode == -1)
    {
        LOG(fatal) << " --rtx " << rtxmode << " leaving ASIS "  ;   
    }
    else
    { 
        int rtx0(-1) ;
        RT_CHECK_ERROR( rtGlobalGetAttribute(RT_GLOBAL_ATTRIBUTE_ENABLE_RTX, sizeof(rtx0), &rtx0) );
        assert( rtx0 == 0 );  // despite being zero performance suggests it is enabled

        int rtx = rtxmode > 0 ? 1 : 0 ;       
        LOG(fatal) << " --rtx " << rtxmode << " setting  " << ( rtx == 1 ? "ON" : "OFF" )  ; 
        RT_CHECK_ERROR( rtGlobalSetAttribute(RT_GLOBAL_ATTRIBUTE_ENABLE_RTX, sizeof(rtx), &rtx));

        int rtx2(-1) ; 
        RT_CHECK_ERROR(rtGlobalGetAttribute(RT_GLOBAL_ATTRIBUTE_ENABLE_RTX, sizeof(rtx2), &rtx2));
        assert( rtx2 == rtx );
    }
}
#endif


OContext::OContext(optix::Context context, Opticks* ok, const char* cmake_target, const char* ptxrel ) 
    : 
    m_context(context),
    m_ok(ok),
    m_cfg(new OConfig(m_context, cmake_target, ptxrel)),
    m_mode(m_ok->isCompute() ? COMPUTE : INTEROP),
    m_debug_photon(m_ok->getDebugIdx()),
    m_entry(0),
    m_closed(false),
    m_verbose(false),
    m_llogpath(NULL),
    m_launch_count(0),
    m_runlabel(m_ok->getRunLabel()),
    m_runresultsdir(m_ok->getRunResultsDir())
{
    init();
    initPrint();
}

void OContext::init()
{

    unsigned int num_ray_type = getNumRayType() ;
    m_context->setRayTypeCount( num_ray_type );   // more static than entry type count

    unsigned stacksize_bytes = m_ok->getStack() ;
    m_ok->set("stacksize", stacksize_bytes );

    m_context->setStackSize(stacksize_bytes);
    LOG(LEVEL) << "OContext::init " 
              << " mode " << getModeName()
              << " num_ray_type " << num_ray_type 
              << " stacksize_bytes " << stacksize_bytes
              ; 

}


bool OContext::hasTopGroup() const 
{
    return m_top.get() != NULL ;  
}

void OContext::createTopGroup()
{
    if(hasTopGroup()) 
    {
        LOG(error) << " already have m_top Group " ; 
        return ; 
    }

    m_top = m_context->createGroup();
    m_context[ "top_object" ]->set( m_top );
}

optix::Group OContext::getTopGroup()
{
    if(!hasTopGroup())
    {
        createTopGroup();
    }
    return m_top ; 
}





void OContext::initPrint()
{
    m_context->setPrintBufferSize(4096);
    //m_context->setPrintBufferSize(2*2*2*8192);

    m_context->setExceptionEnabled(RT_EXCEPTION_ALL, false );  
    // disable all exceptions 
    // this is different from the default of leaving STACKOVERFLOW exception enabled


    glm::ivec3 idx ; 
    if(m_ok->getPrintIndex(idx))   // --pindex 0  : 1st index
    {
        m_context->setPrintEnabled(true);
        m_context->setPrintLaunchIndex(idx.x, idx.y, idx.z);
        LOG(LEVEL) << "setPrintLaunchIndex "
                   << " idx.x " << idx.x
                   << " idx.y " << idx.y
                   << " idx.z " << idx.z
                   ; 
    } 
    else if( m_ok->isPrintEnabled() )   // --printenabled 
    {    
         m_context->setPrintEnabled(true);
         assert( m_context->getPrintEnabled() == true );  
         LOG(info) << " --printenabled " ; 
    }
    else
    {
         return ;  
    }


    // only enable exceptions when print also enabled
    if( m_ok->isExceptionEnabled() )
    {
        m_context->setExceptionEnabled(RT_EXCEPTION_ALL, false );  
    }


    unsigned uindex = m_ok->hasMask() ? m_ok->getMaskIndex(idx.x) : idx.x ; 
    m_llogpath = m_ok->isPrintIndexLog() ?  LaunchLogPath(uindex) : NULL ; 

    LOG(LEVEL) << "OContext::initPrint " 
               << " idx " << gformat(idx) 
               << " llogpath " << ( m_llogpath ? m_llogpath : "-" )
               ;  
}

const char* OContext::getPrintIndexLogPath() const 
{
    return m_llogpath ;  
}


const char* OContext::LaunchLogPath(unsigned index)
{
    const char* name = BStr::concat<unsigned>("ox_", index, ".log" ); 
    std::string path = BFile::FormPath("$TMP", name ); 
    return strdup(path.c_str()); 
}




optix::Context OContext::getContext()
{
     return m_context ; 
}
optix::Context& OContext::getContextRef()
{
     return m_context ; 
}

unsigned int OContext::getNumRayType()
{
    return e_rayTypeCount ;
}

OContext::Mode_t OContext::getMode()
{
    return m_mode ; 
}

bool OContext::isCompute()
{
    return m_mode == COMPUTE ; 
}
bool OContext::isInterop()
{
    return m_mode == INTEROP ; 
}

OContext::~OContext()
{
    cleanUp(); 
}

void OContext::cleanUp()
{
    m_context->destroy();
    m_context = 0;

    cleanUpCache();
}


/**
OContext::cleanUpCache
--------------------------

The cache directory on Linux uses a common
path for all users /var/tmp/OptixCache 
which presents a permissons problem on multi-user systems.

OContext::cleanUpCache is a workaround that deletes the 
cache directory at termination so subsequent users can create 
(and delete) their own such directory.

Note that crashes that prevent the running of cleanupCache 
will cause context creation to fail for subsequent users with::

    terminate called after throwing an instance of 'optix::Exception'
    what():  OptiX was unable to open the disk cache with sufficient privileges. Please make sure the database file is writeable by the current user.

**/

void OContext::cleanUpCache()
{
    const char* key = "OPTICKS_KEEPCACHE" ; 
    int keepcache = SSys::getenvint( key, 0 ); 
    const char* cachedir = CacheDir(); 
    if( keepcache > 0 ) 
    {
        LOG(fatal) << " keeping cache " << cachedir 
                  << " as envvar set " << key 
                  ;  
    }
    else
    {
        LOG(info) << " RemoveDir " << cachedir ; 
        BFile::RemoveDir( cachedir ); 
    }
}


optix::Program OContext::createProgram(const char* cu_filename, const char* progname )
{  
    LOG(LEVEL) << "OContext::createProgram START "
              << " cu_filename " << cu_filename
              << " progname " << progname
              ;

    optix::Program prog = m_cfg->createProgram(cu_filename, progname);

    LOG(LEVEL) << "OContext::createProgram DONE "
              << " cu_filename " << cu_filename
              << " progname " << progname
              ;
    return prog ; 
}

unsigned int OContext::addEntry(const char* cu_filename, const char* raygen, const char* exception, bool defer)
{
    return m_cfg->addEntry(cu_filename, raygen, exception, defer ); 
}
unsigned int OContext::addRayGenerationProgram( const char* filename, const char* progname, bool defer)
{
    assert(0);
    return m_cfg->addRayGenerationProgram(filename, progname, defer);
}
unsigned int OContext::addExceptionProgram( const char* filename, const char* progname, bool defer)
{
    assert(0);
    return m_cfg->addExceptionProgram(filename, progname, defer);
}

void OContext::setMissProgram( unsigned int index, const char* filename, const char* progname, bool defer )
{
    m_cfg->setMissProgram(index, filename, progname, defer);
}



void OContext::close()
{
    if(m_closed) return ; 

    m_closed = true ; 

    unsigned int num = m_cfg->getNumEntryPoint() ;

    LOG(debug) << "numEntryPoint " << num ; 

    m_context->setEntryPointCount( num );

    LOG(debug) << "setEntryPointCount done." ;
 
    if(m_verbose) m_cfg->dump("OContext::close");

    m_cfg->apply();

    LOG(debug) << "m_cfg->apply() done." ;

}


void OContext::dump(const char* msg)
{
    m_cfg->dump(msg);
}
unsigned int OContext::getNumEntryPoint()
{
    return m_cfg->getNumEntryPoint();
}


void OContext::launch(unsigned int lmode, unsigned int entry, unsigned int width, unsigned int height, BTimes* times )
{
    if(!m_closed) close();

    LOG(LEVEL)
              << " entry " << entry 
              << " width " << width 
              << " height " << height 
              ;


    if(lmode & VALIDATE)
    {
        double dt = validate_();
        LOG(LEVEL) << "VALIDATE time: " << dt ;
        if(times) times->add("validate", m_launch_count,  dt) ;
    }

    if(lmode & COMPILE)
    {
        double dt = compile_();
        LOG(LEVEL) << "COMPILE time: " << dt ;
        if(times) times->add("compile", m_launch_count,  dt) ;
    }

    if(lmode & PRELAUNCH)
    {
        double dt = launch_(entry, 0u, 0u );
        LOG(LEVEL) << "PRELAUNCH time: " << dt ;
        if(times) times->add("prelaunch", m_launch_count,  dt) ;
    }

    if(lmode & LAUNCH)
    {
        double dt = m_llogpath ? launch_redirected_(entry, width, height ) : launch_(entry, width, height );
        LOG(LEVEL) << "LAUNCH time: " << dt  ;
        if(times) times->add("launch", m_launch_count,  dt) ;
    }

    m_launch_count += 1 ; 
}


double OContext::validate_()
{
    double t0, t1 ; 
    t0 = BTimeStamp::RealTime();

    m_context->validate(); 

    t1 = BTimeStamp::RealTime();
    return t1 - t0 ; 
}

double OContext::compile_()
{
    double t0, t1 ; 
    t0 = BTimeStamp::RealTime();

    m_context->compile(); 

    t1 = BTimeStamp::RealTime();
    return t1 - t0 ; 
}

double OContext::launch_(unsigned entry, unsigned width, unsigned height)
{
    double t0, t1 ; 
    t0 = BTimeStamp::RealTime();

    m_context->launch( entry, width, height ); 

    t1 = BTimeStamp::RealTime();
    return t1 - t0 ; 
}


double OContext::launch_redirected_(unsigned entry, unsigned width, unsigned height)
{
    assert( m_llogpath ) ;

    S_freopen_redirect sfr(stdout, m_llogpath );

    double dt = launch_( entry, width, height ) ;

    return dt ;  
}

/*

OContext::launch_redirected_ succeeds to write kernel rtPrintf 
logging to file BUT a subsequent same process "system" invokation 
of python has problems
indicating that the cleanup is not complete::

    Traceback (most recent call last):
      File "/Users/blyth/opticks/ana/tboolean.py", line 62, in <module>
        print ab
    IOError: [Errno 9] Bad file descriptor
    2017-12-13 15:33:13.436 INFO  [321569] [SSys::run@50] tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch   rc_raw : 256 rc : 1
    2017-12-13 15:33:13.436 WARN  [321569] [SSys::run@57] SSys::run FAILED with  cmd tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch  
    2017-12-13 15:33:13.436 INFO  [321569] [OpticksAna::run@79] OpticksAna::run anakey tboolean cmdline tboolean.py --tag 1 --tagoffset 0 --det tboolean-box --src torch   rc 1 rcmsg OpticksAna::run non-zero RC from ana script
    2017-12-13 15:33:13.436 FATAL [321569] [Opticks::dumpRC@186]  rc 1 rcmsg : OpticksAna::run non-zero RC from ana script
    2017-12-13 15:33:13.436 INFO  [321569] [SSys::WaitForInput@151] SSys::WaitForInput OpticksAna::run paused : hit RETURN to continue...

*/



template <typename T>
void OContext::upload(optix::Buffer& buffer, NPY<T>* npy)
{
    unsigned int numBytes = npy->getNumBytes(0) ;

    OpticksBufferControl ctrl(npy->getBufferControlPtr());
    bool verbose = ctrl("VERBOSE_MODE") || SSys::IsVERBOSE() ;

    if(ctrl(OpticksBufferControl::OPTIX_OUTPUT_ONLY_))
    { 
         LOG(warning) << "OContext::upload NOT PROCEEDING "
                      << " name " << npy->getBufferName()
                      << " as " << OpticksBufferControl::OPTIX_OUTPUT_ONLY_
                      << " desc " << npy->description("skip-upload") 
                      ;
     
    }
    else if(ctrl("UPLOAD_WITH_CUDA"))
    {
        if(verbose) LOG(info) << npy->description("UPLOAD_WITH_CUDA markDirty") ;

        void* d_ptr = NULL;
        rtBufferGetDevicePointer(buffer->get(), 0, &d_ptr);
        cudaMemcpy(d_ptr, npy->getBytes(), numBytes, cudaMemcpyHostToDevice);
        buffer->markDirty();
        if(verbose) LOG(info) << npy->description("UPLOAD_WITH_CUDA markDirty DONE") ;

    }
    else
    {
        if(verbose) LOG(info) << npy->description("standard OptiX UPLOAD") ;
        memcpy( buffer->map(), npy->getBytes(), numBytes );
        buffer->unmap(); 
    }
}


template <typename T>
void OContext::download(optix::Buffer& buffer, NPY<T>* npy)
{
    assert(npy);
    OpticksBufferControl ctrl(npy->getBufferControlPtr());
    bool verbose = ctrl("VERBOSE_MODE") || SSys::IsVERBOSE() ;

    bool proceed = false ; 
    if(ctrl(OpticksBufferControl::OPTIX_INPUT_ONLY_))
    {
         proceed = false ; 
         LOG(warning) << "OContext::download NOT PROCEEDING "
                      << " name " << npy->getBufferName()
                      << " as " << OpticksBufferControl::OPTIX_INPUT_ONLY_
                      << " desc " << npy->description("skip-download") 
                      ;
    }
    else if(ctrl(OpticksBufferControl::COMPUTE_MODE_))
    {
         proceed = true ; 
    }
    else if(ctrl(OpticksBufferControl::OPTIX_NON_INTEROP_))
    {   
         proceed = true ;
         LOG(info) << "OContext::download PROCEED for " << npy->getBufferName() << " as " << OpticksBufferControl::OPTIX_NON_INTEROP_  ;
    }   
    
    if(proceed)
    {

        if(verbose)
             LOG(info) << " VERBOSE_MODE "  << " " << npy->description("download") ;

        void* ptr = buffer->map() ; 
        npy->read( ptr );
        buffer->unmap(); 
    }
    else
    {
        if(verbose)
             LOG(info)<< npy->description("download SKIPPED") ;

    }
}





optix::Buffer OContext::createEmptyBufferF4() 
{
    optix::Buffer emptyBuffer = m_context->createBuffer(RT_BUFFER_INPUT_OUTPUT, RT_FORMAT_FLOAT4, 0);
    return emptyBuffer ;
}


template <typename T>
optix::Buffer OContext::createBuffer(NPY<T>* npy, const char* name)
{
    assert(npy);
    OpticksBufferControl ctrl(npy->getBufferControlPtr());
    bool verbose = ctrl("VERBOSE_MODE") || SSys::IsVERBOSE() ;

    bool compute = isCompute()  ; 

    if(verbose) 
       LOG(info) << "OContext::createBuffer "
              << std::setw(20) << name 
              << std::setw(20) << npy->getShapeString()
              << " mode : " << ( compute ? "COMPUTE " : "INTEROP " )
              << " BufferControl : " << ctrl.description(name)
              ;

    unsigned int type(0);
    bool noctrl = false ; 
    
    if(      ctrl("OPTIX_INPUT_OUTPUT") )  type = RT_BUFFER_INPUT_OUTPUT ;
    else if( ctrl("OPTIX_OUTPUT_ONLY")  )  type = RT_BUFFER_OUTPUT  ;
    else if( ctrl("OPTIX_INPUT_ONLY")   )  type = RT_BUFFER_INPUT  ;
    else                                   noctrl = true ; 
   
    if(noctrl) LOG(fatal) << "no buffer control for " << name << ctrl.description("") ;
    assert(!noctrl);

 
    if( ctrl("BUFFER_COPY_ON_DIRTY") )     type |= RT_BUFFER_COPY_ON_DIRTY ;
    // p170 of OptiX_600 optix-api 


    optix::Buffer buffer ; 

    if( compute )
    {
        buffer = m_context->createBuffer(type);
    }
    else if( ctrl("OPTIX_NON_INTEROP") )
    {
        buffer = m_context->createBuffer(type);
    }
    else
    {
         int buffer_id = npy ? npy->getBufferId() : -1 ;
         if(!(buffer_id > -1))
             LOG(fatal) << "OContext::createBuffer CANNOT createBufferFromGLBO as not uploaded  "
                        << " name " << std::setw(20) << name
                        << " buffer_id " << buffer_id 
                         ; 
         assert(buffer_id > -1 );
         buffer = m_context->createBufferFromGLBO(type, buffer_id);
    } 

    configureBuffer<T>(buffer, npy, name );
    return buffer ; 
}




template <typename T>
unsigned OContext::determineBufferSize(NPY<T>* npy, const char* name)
{
    unsigned int ni = std::max(1u,npy->getShape(0));
    unsigned int nj = std::max(1u,npy->getShape(1));  
    unsigned int nk = std::max(1u,npy->getShape(2));  

    bool is_seed = strcmp(name, "seed")==0 ;

    RTformat format = getFormat(npy->getType(), is_seed);
    unsigned int size ; 

    if(format == RT_FORMAT_USER || is_seed)
    {
        size = ni*nj*nk ; 
    }
    else
    {
        size = npy->getNumQuads() ;  
 
    }
    return size ; 
}


template <typename T>
void OContext::configureBuffer(optix::Buffer& buffer, NPY<T>* npy, const char* name)
{

    bool is_seed = strcmp(name, "seed")==0 ;

    RTformat format = getFormat(npy->getType(), is_seed);
    buffer->setFormat(format);  // must set format, before can set ElementSize


    unsigned size = determineBufferSize(npy, name);

    const char* label ; 
    if(     format == RT_FORMAT_USER) label = "USER";
    else if(is_seed)                  label = "SEED";
    else                              label = "QUAD";



    std::stringstream ss ; 
    ss 
       << std::setw(10) << name
       << std::setw(20) << npy->getShapeString()
       << " " << label 
       << " size " << size ; 
       ;
    std::string hdr = ss.str();

    if(format == RT_FORMAT_USER )
    {
        buffer->setElementSize(sizeof(T));
        LOG(LEVEL) << hdr
                  << " elementsize " << sizeof(T)
                  ;
    }
    else
    {
        LOG(LEVEL) << hdr ;
    }
    

    buffer->setSize(size); 

    //
    // NB in interop mode, the OptiX buffer is just a reference to the 
    // OpenGL buffer object, however despite this the size
    // and format of the OptiX buffer still needs to be set as they control
    // the addressing of the buffer in the OptiX programs 
    //
    //         79 rtBuffer<float4>               genstep_buffer;
    //         80 rtBuffer<float4>               photon_buffer;
    //         ..
    //         85 rtBuffer<short4>               record_buffer;     // 2 short4 take same space as 1 float4 quad
    //         86 rtBuffer<unsigned long long>   sequence_buffer;   // unsigned long long, 8 bytes, 64 bits 
    //
}


template <typename T>
void OContext::resizeBuffer(optix::Buffer& buffer, NPY<T>* npy, const char* name)
{
    OpticksBufferControl ctrl(npy->getBufferControlPtr());
    bool verbose = ctrl("VERBOSE_MODE") ;

    unsigned size = determineBufferSize(npy, name);
    buffer->setSize(size); 

    if(verbose)
    LOG(info) << "OContext::resizeBuffer " << name << " shape " << npy->getShapeString() << " size " << size  ; 
}





RTformat OContext::getFormat(NPYBase::Type_t type, bool is_seed)
{
    RTformat format ; 
    switch(type)
    {
        case NPYBase::FLOAT:     format = RT_FORMAT_FLOAT4         ; break ; 
        case NPYBase::SHORT:     format = RT_FORMAT_SHORT4         ; break ; 
        case NPYBase::INT:       format = RT_FORMAT_INT4           ; break ; 
        case NPYBase::UINT:      format = RT_FORMAT_UNSIGNED_INT4  ; break ; 
        case NPYBase::CHAR:      format = RT_FORMAT_BYTE4          ; break ; 
        case NPYBase::UCHAR:     format = RT_FORMAT_UNSIGNED_BYTE4 ; break ; 
        case NPYBase::ULONGLONG: format = RT_FORMAT_USER           ; break ; 
        case NPYBase::DOUBLE:    format = RT_FORMAT_USER           ; break ; 
    }

    if(is_seed)
    {
         assert(type == NPYBase::UINT);
         format = RT_FORMAT_UNSIGNED_INT ;
         LOG(LEVEL) << "OContext::getFormat override format for seed " ; 
    }
    return format ; 
}




void OContext::snap(const char* path)
{
    if(m_ok->isNoSavePPM())
    {
        LOG(fatal) << " --nosaveppm " << path ; 
        return ;  
    }


    optix::Buffer output_buffer = m_context["output_buffer"]->getBuffer() ; 

    RTsize width, height, depth ;
    output_buffer->getSize(width, height, depth);

    bool yflip = true ; 
    LOG(LEVEL) 
         << " path " << path 
         << " width " << width
         << " height " << height
         << " depth " << depth
         << " yflip " << yflip
         ;   

    void* ptr = output_buffer->map() ; 

    int ncomp = 4 ;   
    SPPM::write(path,  (unsigned char*)ptr , width, height, ncomp, yflip );

    output_buffer->unmap(); 
}


void OContext::save(const char* path)
{
    optix::Buffer output_buffer = m_context["output_buffer"]->getBuffer() ;

    RTsize width, height, depth ;
    output_buffer->getSize(width, height, depth);

    LOG(info)
         << " width " << width
         << " width " << (int)width
         << " height " << height
         << " height " << (int)height
         << " depth " << depth
         ;

    NPY<unsigned char>* npy = NPY<unsigned char>::make(width, height, 4) ;
    npy->zero();

    void* ptr = output_buffer->map() ;
    npy->read( ptr );

    output_buffer->unmap();

    npy->save(path);
}







template OXRAP_API void OContext::upload<unsigned>(optix::Buffer&, NPY<unsigned>* );
template OXRAP_API void OContext::download<unsigned>(optix::Buffer&, NPY<unsigned>* );
template OXRAP_API void OContext::resizeBuffer<unsigned>(optix::Buffer&, NPY<unsigned>*, const char* );

template OXRAP_API void OContext::upload<float>(optix::Buffer&, NPY<float>* );
template OXRAP_API void OContext::download<float>(optix::Buffer&, NPY<float>* );
template OXRAP_API void OContext::resizeBuffer<float>(optix::Buffer&, NPY<float>*, const char* );

template OXRAP_API void OContext::upload<short>(optix::Buffer&, NPY<short>* );
template OXRAP_API void OContext::download<short>(optix::Buffer&, NPY<short>* );
template OXRAP_API void OContext::resizeBuffer<short>(optix::Buffer&, NPY<short>*, const char* );

template OXRAP_API void OContext::upload<unsigned long long>(optix::Buffer&, NPY<unsigned long long>* );
template OXRAP_API void OContext::download<unsigned long long>(optix::Buffer&, NPY<unsigned long long>* );
template OXRAP_API void OContext::resizeBuffer<unsigned long long>(optix::Buffer&, NPY<unsigned long long>*, const char* );

template OXRAP_API optix::Buffer OContext::createBuffer(NPY<float>*, const char* );
template OXRAP_API optix::Buffer OContext::createBuffer(NPY<short>*, const char* );
template OXRAP_API optix::Buffer OContext::createBuffer(NPY<unsigned long long>*, const char* );
template OXRAP_API optix::Buffer OContext::createBuffer(NPY<unsigned>*, const char* );


