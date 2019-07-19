
#include <string>
#include <vector>


#include "SAr.hh"
#include "STime.hh"
#include "SSys.hh"
#include "BStr.hh"
#include "NGLM.hpp"
#include "OpticksCfg.hh"
#include "Opticks.hh"

class Opticks ; 

template OKCORE_API void BCfg::addOptionF<Opticks>(Opticks*, const char*, const char* );
template OKCORE_API void BCfg::addOptionI<Opticks>(Opticks*, const char*, const char* );
template OKCORE_API void BCfg::addOptionS<Opticks>(Opticks*, const char*, const char* );


#include "PLOG.hh"


template <class Listener>
OpticksCfg<Listener>::OpticksCfg(const char* name, Listener* listener, bool live) 
       : 
       BCfg(name, live),
       m_listener(listener),
       m_cvd(""),
       m_size(""),
       m_position(""),
       m_dbgcsgpath(""),
       m_logname(""),
       m_event_cat(""),
       //m_event_pfx("source"),
       m_event_pfx(""),
       m_event_tag("1"),
       m_integrated_event_tag("100"),
       m_liveline(""),

       m_configpath(""),
       m_exportconfig("$TMP"),
       m_torchconfig(""),
       m_g4gunconfig(""),
       m_g4inimac(""),
       m_g4runmac(""),
       m_g4finmac(""),
       m_anakey(""),
       m_anakeyargs(""),
       m_testconfig(""),
       m_state_tag(""),
       m_materialprefix("/dd/Materials/"),
       m_snapconfig(""),
       m_zexplodeconfig("-5564.975,1000."),  // -(5564.950 + 5565.000)/2.0 = -5564.975
       m_meshversion(""),
       m_rendermode(""),
       m_rendercmd(""),
       m_islice(""),
       m_fslice(""),
       m_pslice(""),
       m_instancemodulo(""),
       m_pindex(""),
       m_dindex(""),
       m_oindex(""),
       m_gindex(""),
       m_mask(""),
       m_x4polyskip(""),
       m_csgskiplv(""),
       m_accel(""),
       m_seqmap(""),
       m_dbgseqhis("0"),
       m_dbgseqmat("0"),
       m_dbgmesh(""),
       m_fxreconfig("0"),
       m_fxabconfig("0"),
       m_fxscconfig("0"),
       m_apmtslice(""),
       //m_lvsdname("PmtHemiCathode,HeadonPmtCathode"),
       m_lvsdname(""),
       m_cathode("Bialkali"),
       m_cerenkovclass("C4Cerenkov1042"),
       m_scintillationclass("C4Scintillation1042"),
       m_epsilon(0.1f),     
       m_pixeltimescale(1e-10f),     
       m_seed(42),     
       m_rtx(0),
       m_renderlooplimit(0),
       m_rngmax(3),     
       m_rngmaxscale(1000000),     
       m_bouncemax(9),     
       m_recordmax(10),
       m_timemaxthumb(6.f),
       m_timemax(-1.f),
       m_animtimerange("-1,-1"),
       m_animtimemax(-1.f),
       m_animator_period(200),
       m_ivperiod(100),
       m_ovperiod(180),
       m_tvperiod(100),
       m_repeatidx(-1),
       m_multievent(1),
       m_enabledmergedmesh(""),
       m_analyticmesh(-1),
       m_cameratype(0),
       m_modulo(-1),
       m_generateoverride(0),
       m_propagateoverride(0),
       m_debugidx(0),
       m_dbgnode(-1),
       m_dbgmm(-1),
       m_dbglv(-1),
       m_stack(2180),
       m_num_photons_per_g4event(10000),
       m_loadverbosity(0),
       m_importverbosity(0),
       m_meshverbosity(0),
       m_verbosity(0),
       m_apmtidx(0),
       m_flightpathdir("/tmp"),
       m_apmtmedium(""),
       m_srcgltfbase("$TMP/nd"),
       m_srcgltfname("scene.gltf"),
       m_gltfconfig("check_surf_containment=0,check_aabb_containment=0,instance_repeat_min=400,instance_vertex_min=0"),
       m_gltf(0),
       m_gltftarget(0),
       m_layout(1),
       m_lodconfig("levels=3,verbosity=3"),
       m_lod(0),
       m_target(0),
       m_alignlevel(0),
       m_exename(SAr::Instance->exename()), 
       m_gpumonpath(BStr::concat("$TMP/",m_exename ? m_exename : "OpticksCfg","_GPUMon.npy")),
       m_runcomment(""),
       m_runstamp(STime::EpochSeconds()),
       m_runlabel(""),
       m_runfolder(strdup(m_exename))
{   
   init();  
   m_listener->setCfg(this); 
}


template <class Listener>
void OpticksCfg<Listener>::init()
{
   m_desc.add_options()
       ("cfg4", "use Geant4 for generation/propagation, only supports TORCH source type with test geometries") ;

   m_desc.add_options()
       ("version,v", "print version string") ;

   m_desc.add_options()
       ("help,h",    "print help message") ;

   m_desc.add_options()
       ("idpath,i",  "print idpath based on input envvars") ;

   m_desc.add_options()
       ("nogeocache,G",  "inhibit use of the geocache") ;

   m_desc.add_options()
       ("deletegeocache",  "deletes the geocache before recreating") ;

   m_desc.add_options()
       ("dbg_with_hemi_ellipsoid_bug",  "checking the effect of putting the bug back in X4Solid::convertEllipsoid") ;



   m_desc.add_options()
       ("nopropagate,P",  "inhibit generation/propagation") ;

   m_desc.add_options()
       ("xanalytic",  "FORMERLY switched on analytic geometry in optixrap, now enabled by default with this option ignored, used --xtriangle to switch OFF") ;

   m_desc.add_options()
       ("xtriangle",  "disable --xanalytic ") ;




   m_desc.add_options()
       ("xgeometrytriangles",  
        "switches on use of optix::GeometryTriangles utilizing RT Cores in Turing and later GPUs."
        "This requires OptiX 6.0.0 or later, and RTX mode must be enabled. "
        "This option is slated for removal, instead use \"--rtx 2\" to enable RTX with GeometryTriangles ") ;

   char rtx[128];
   snprintf(rtx,128, "OptiX RTX execution mode, -1:ASIS 0:OFF 1:ON 2:ON with GeometryTriangles. Default %d", m_rtx );
   m_desc.add_options()
       ("rtx",  boost::program_options::value<int>(&m_rtx), rtx );



   char renderlooplimit[128];
   snprintf(renderlooplimit,128, "When greater than zero limits the number of turns of the renderloop. Default %d", m_renderlooplimit );
   m_desc.add_options()
       ("renderlooplimit",  boost::program_options::value<int>(&m_renderlooplimit), renderlooplimit );




   m_desc.add_options()
       ("dumpenv",  
         "dump envvars with names starting with OPTICKS at Opticks instanciation "
         " and envvars starting with G4,OPTICKS,DAE,IDPATH in Opticks::configure "
       ) ;

   m_desc.add_options()
       ("noevent,E",  "inhibit event handling") ;

   m_desc.add_options()
       ("nostep,S",  "inhibit step by step recording") ;

   m_desc.add_options()
       ("nog4propagate",  "inhibit G4 propagate in a bi-simulation executable such as OKG4Test, see OpticksRun OKG4Mgr") ;


   m_desc.add_options()
       ("nointerpol",  "inhibit interpolation in cfg4/tests/CInterpolationTest for identity check") ;

   m_desc.add_options()
       ("finebndtex",  "Use 1nm pitch wavelength domain for boundary buffer (ie material and surface properties) obtained by interpolation postcache, see GGeo::loadFromCache");

   m_desc.add_options()
       ("nogroupvel",  "inhibit group_velocity property calculated from refractive_index in GMaterialLib::postLoadFromCache ");

   m_desc.add_options()
       ("liverecorder", "adopt cfg4/CRecorder live stepping recording");


   m_desc.add_options()
       ("nore",  "inhibit reemission by zeroing reemission_prob of named scintillators after loading from cache, see GMaterialLib::postLoadFromCache ") ;
   m_desc.add_options()
       ("xxre",  "enhance reemission by setting reemission_prob to one for named scintillators after loading from cache, see GMaterialLib::postLoadFromCache ") ;
   m_desc.add_options()
       ("fxre",  "enable application of config from   --fxreconfig option ");

   m_desc.add_options()
       ("fxreconfig",   boost::program_options::value<std::string>(&m_fxreconfig), "post cache artificial modification of reemission prob of scintillating materials, see GMaterialLib::postLoadFromCache" );


   m_desc.add_options()
       ("noab",  "inhibit absorption by setting large absorption_length of all materials after loading from cache, see GMaterialLib::postLoadFromCache ") ;
   m_desc.add_options()
       ("xxab",  "enhance absorption by setting small absorption_length of all materials after loading from cache, see GMaterialLib::postLoadFromCache ") ;
   m_desc.add_options()
       ("fxab",  "enable application of config from   --fxabconfig option ");
   m_desc.add_options()
       ("fxabconfig",   boost::program_options::value<std::string>(&m_fxabconfig), "post cache artificial modification of absorption lengths of materials, see GMaterialLib::postLoadFromCache" );


   m_desc.add_options()
       ("nosc",  "inhibit scattering by setting large scattering_length of all materials after loading from cache, see GMaterialLib::postLoadFromCache ") ;
   m_desc.add_options()
       ("xxsc",  "enhance scattering by setting small scattering_length of all materials after loading from cache, see GMaterialLib::postLoadFromCache ") ;
   m_desc.add_options()
       ("fxsc",  "enable application of config from   --fxscconfig option ");
   m_desc.add_options()
       ("fxscconfig",   boost::program_options::value<std::string>(&m_fxscconfig), "configure post cache artificial modification of scattering lengths of materials, see GMaterialLib::postLoadFromCache" );

   m_desc.add_options()
       ("apmtslice",   boost::program_options::value<std::string>(&m_apmtslice), "Analytic PMT slice specification string." );


   m_desc.add_options()
       ("ctrldrag",   "use alternative cursor interaction approach, see Frame::cursor_moved ") ; 

   m_desc.add_options()
       ("nosaveppm",   "skip the saving of PPM files from compute snaps") ; 



   m_desc.add_options()
       ("savehit",   "save hits even in production running") ; 

   m_desc.add_options()
       ("vizg4",   "when vizualizing loaded events, upload the G4Evt not the OKEvt one ") ; 

   m_desc.add_options()
       ("g4codegen",   "generate and persist Geant4 geometry code for each solid of the geometry in X4PhysicalVolume, see X4GEN_DIR ") ; 


   m_desc.add_options()
       ("tracer",   "used to argforced signal from OTracerTest that progagator should not be setup avoiding issue tracer_crash.rst") ; 



   char pixeltimescale[128];
   snprintf(pixeltimescale,128, "Adhoc scaling of trace times to form a color . Default %10.4f", m_pixeltimescale);
   m_desc.add_options()
       ("pixeltimescale",  boost::program_options::value<float>(&m_pixeltimescale), pixeltimescale );



   m_desc.add_options()
       ("nooptix,O",  "inhibit use of OptiX") ;

   m_desc.add_options()
       ("nonet,N",  "inhibit use of network") ;

   m_desc.add_options()
       ("stamp",  "output profile stamps as they are made, see OpticksProfile") ; 


   m_desc.add_options()
       ("utaildebug",  "switch on the debug storing of an extra random float at the tail of the photon simulation as an additional check of random alignment") ; 

   m_desc.add_options()
       ("production",  "skip all debug/test features, do only what is needed in production") ; 



   m_desc.add_options()
       ("jwire",  "enable wire frame view, use only with workstation GPUs") ;

   m_desc.add_options()
       ("cg4sigint",  "interrupt in CG4::preinit a good place to set Geant4 breakpoints, see env-;gdb- for hints") ;




   m_desc.add_options()
       ("optixviz",  "Enable OptiXViz, needed in load mode where ordinarily OptiX is not enabled as no propagation is done.") ;

   m_desc.add_options()
       ("noviz,V",  "just generate, propagate and save : no visualization, (OKMgr level)") ;

   m_desc.add_options()
       ("noindex,I",  "no photon/record indexing") ;

   m_desc.add_options()
       ("noinstanced",  "inhibit instancing, use when debugging few volumes") ;

   m_desc.add_options()
       ("instcull",  "experimental instance culling via transform feedback pass to establish visible instances, see oglrap/Scene.cc") ;


   m_desc.add_options()
       ("zbuf",  "experimental: enable OptiX Z-buffer rendering with ORenderer, OFrame ... ") ;



   m_desc.add_options()
       ("trivial",  "swap OptiX generate program with trivial standin for debugging") ;

   m_desc.add_options()
       ("nothing",  "swap OptiX generate program with do nothing standin for debugging, NB this is different from --nopropagate as a launch is done") ;

   m_desc.add_options()
       ("dumpseed",  "swap OptiX generate program with standin for debugging, that just dumps the seed ");

   m_desc.add_options()
       ("seedtest",  "interop buffer overwrite debugging") ;

   m_desc.add_options()
       ("tracetest",  "swap OptiX generate program with tracetest standin for debugging") ;

   m_desc.add_options()
       ("zrngtest",  "swap OptiX generate program with zrngtest standin for debugging, saving 16 uniform rand into photon buffer") ;

   m_desc.add_options()
       ("align",  "attempt to use cuRAND GPU generated random number sequence within Geant4") ;
   m_desc.add_options()
       ("dbgnojumpzero",  "debug of the maligned six, see notes/issues/RNG_seq_off_by_one.rst") ;
   m_desc.add_options()
       ("dbgskipclearzero",  "debug of the maligned six, see notes/issues/RNG_seq_off_by_one.rst") ;
   m_desc.add_options()
       ("dbgkludgeflatzero",  "debug of the maligned six, see notes/issues/RNG_seq_off_by_one.rst") ;


   m_desc.add_options()
       ("simplify",  "simplify OptiX geometry for debugging") ;

   m_desc.add_options()
       ("qe1",  "Perfect quantum efficiency of cathodes") ;

   m_desc.add_options()
       ("geocenter",  "center view on geometry rather than the default genstep centering") ;

   m_desc.add_options()
       ("g4ui",  "cfg4: start g4ui session") ;


   m_desc.add_options()("trace",  "loglevel");
   m_desc.add_options()("debug",  "loglevel");
   m_desc.add_options()("info",  "loglevel");
   m_desc.add_options()("warning",  "loglevel");
   m_desc.add_options()("error",  "loglevel");
   m_desc.add_options()("fatal",  "loglevel");


   m_desc.add_options()
       ("compute",  "COMPUTE mode, ie not INTEROP") ; 

   m_desc.add_options()
       ("interop",  "INTEROP mode, normally the default, but convenient to trump an existing --compute on commandline, switching on visualization.") ; 




   m_desc.add_options()
       ("scintillation,s",  "load scintillation gensteps") ;

   m_desc.add_options()
       ("cerenkov,c",  "load cerenkov gensteps") ;

   m_desc.add_options()
       ("natural,n",  "load natural gensteps containing a mixture of scintillation and cerenkov steps") ;

   m_desc.add_options()
       ("machinery",  "machinery testing gensteps/event potentially non-indexable  ");


   m_desc.add_options()
       ("live",   "get gensteps direct from live Geant4") ; 

   m_desc.add_options()
       ("embedded",  "get gensteps direct from host into which Opticks is embedded via OpMgr interface") ; 



   m_desc.add_options()
       ("reflectcheat",  
           "artifically replace the random number generation to decide whether to reflect or transmit"
           "with the ucheat float photon_index/num_photons; "
           "this debug trickery enables G4 and OK simulations of the same "
           "input emitconfig photons to stay point-by-point aligned thru refections. "
       ) ;

   m_desc.add_options()
       ("dbgdownload",  "debug event downloading from GPU") ;

   m_desc.add_options()
       ("dbghit",  "debug hit downloading from GPU") ;


   m_desc.add_options()
       ("dbggeotest",  "debug creation of test geometry") ;


   m_desc.add_options()
       ("save",  "download generated/propagated event data from GPU and save to file") ;

   m_desc.add_options()
       ("nosave",  "disable saving event data to file, --nosave option trumps any --save options on commandline  ") ;




   m_desc.add_options()
       ("load",  "load event data from file and upload to GPU for visualization") ;

   m_desc.add_options()
       ("noload",  "disable prior or subsequent load option, as seen from Opticks::isLoad method") ;



   m_desc.add_options()
       ("torch",  "fabricate torch genstep using torch config settings") ;

   m_desc.add_options()
       ("torchconfig",   boost::program_options::value<std::string>(&m_torchconfig), "torch configuration" );

   m_desc.add_options()
       ("torchdbg",  "dump details of torch config") ;

   m_desc.add_options()
       ("sourcedbg",  "dump details of emitsource ") ;

   m_desc.add_options()
       ("dbgaim",  "dump details of targetting, see OpticksAim ") ;



   m_desc.add_options()
       ("dbgflags",  "debug bad flags" );

   m_desc.add_options()
       ("dbgseed",  "save empty interop mode photons buffer after seeding to $TMP/dbgseed.npy "); 
   m_desc.add_options()
       ("dbginterop", "used Scene::initRenderersDebug with subset of renderers "); 
   m_desc.add_options()
       ("dbguploads", "used in OpticksViz::uploadEvent to dump the uploads table "); 
   m_desc.add_options()
       ("dbgtestgeo", "debug test geometry see cfg4/CTestDetector"); 
   m_desc.add_options()
       ("dbganalytic", "debug analytic test geometry see ggeo/GGeoTest"); 

   m_desc.add_options()
       ("dbgsurf", "debug surface handling see notes/issues/surface_review.rst "); 

   m_desc.add_options()
       ("dbgbnd", "debug boundary handling see notes/issues/surface_review.rst "); 

   m_desc.add_options()
       ("dbgtex", "debug boundary texture see OBndLib "); 



   m_desc.add_options()
       ("dbgrec", "debug CRecorder "); 

   m_desc.add_options()
       ("dbgzero", "debug CRecorder zero seqmat/seqhis "); 

   m_desc.add_options()
       ("recpoi", "experimental point based CRecorder/CRec  "); 

   m_desc.add_options()
       ("recpoialign", 
        "use --recpoialign together with --recpoi to "
        "align step truncation between of the recpoi recorder cfg4/CRec, "
        "by using the same step limit as with non-recpoi running" 
        "This spins G4 wheels for recpoi, it is more efficient not to do this but then "
        "the recpoi random sequences will diverge from non-recpoi due to the different step truncation. "
       ); 



   m_desc.add_options()
       ("reccf", "compare recpoi recstp  "); 




   m_desc.add_options()
       ("dbgclose", "debug proplib close "); 




   m_desc.add_options()
       ("onlyseed", "exit App after seeding "); 

   m_desc.add_options()
       ("nogeometry", "skip loading of geometry, for debugging only "); 





   m_desc.add_options()
       ("g4gun",  "enable cfg4- geant4 only particle gun ") ;

   m_desc.add_options()
       ("emitsource",  "indicates use of photons obtained from emissive geometry ") ;

   m_desc.add_options()
       ("dbgemit",  "enable OpticksGen/NEmitPhotonsNPY debugging  ") ;


   m_desc.add_options()
       ("primarysource",  "indicates use of primary non-optical particles as source, simular to g4gun  ") ;



   m_desc.add_options()
       ("g4gunconfig",   boost::program_options::value<std::string>(&m_g4gunconfig), "g4gun configuration" );

   m_desc.add_options()
       ("g4gundbg",  "dump details of g4gun config") ;


   m_desc.add_options()
       ("g4inimac",   boost::program_options::value<std::string>(&m_g4inimac), "path to g4 initialization .mac file, see cfg4-/CG4" );

   m_desc.add_options()
       ("g4runmac",   boost::program_options::value<std::string>(&m_g4runmac), "path to g4 run .mac file, see cfg4-/CG4" );

   m_desc.add_options()
       ("g4finmac",   boost::program_options::value<std::string>(&m_g4finmac), "path to g4 final .mac file, see cfg4-/CG4" );

   m_desc.add_options()
       ("anakey",   boost::program_options::value<std::string>(&m_anakey), "string identifying an analysis configuration, often the invoking bash FUNCNAME stem, see OpticksAna " );
   m_desc.add_options()
       ("anakeyargs",   boost::program_options::value<std::string>(&m_anakeyargs), 
            "Extra arguments for the anakey script, see OpticksAna." 
            "As passing spaces thru bash scripts/commandline is problematic encode "
            "spaces in the argument list with underscores, for example  "
            "    --anakeyargs \"--c2max_0.5\"   "
            "which is replaced with a space. " 
      );





   m_desc.add_options()
       ("mat",  "Placeholder to avoid warning from GMaterialLibTest, op --mat   ") ;


   m_desc.add_options()
       ("cmat",  "Placeholder for exe choosing option, CMaterialLibTest, op --cmat   ") ;



   m_desc.add_options()
       ("materialdbg",  "dump details of material lookup, ok.isMaterialDbg() ") ;


   m_desc.add_options()
       ("steppingdbg",  "dump details of cfg4 stepping") ;

   m_desc.add_options()
       ("indexdbg",  "dump details of indexing") ;

   m_desc.add_options()
       ("forceindex",  "force indexing even when index exists already") ;



   m_desc.add_options()
       ("meshfixdbg",  "dump details of meshfixing, only active when rebuilding geocache with -G option") ;






   m_desc.add_options()
       ("test",  "fabricate dynamic test geometry, materials and surfaces configured via testconfig settings") ;

   m_desc.add_options()
       ("testconfig",   boost::program_options::value<std::string>(&m_testconfig), "dynamic test geometry configuration" );

   m_desc.add_options()
       ("testauto",  "Switch on experimental automation of test geometry boundary and emitconfig setup"
                     " to facilitate NCSGIntersect checking with OpticksEventAna. ") ;




   m_desc.add_options()
       ("export",  "cfg4: write geometry to file using exportconfig settings") ;

   m_desc.add_options()
       ("exportconfig",   boost::program_options::value<std::string>(&m_exportconfig), "export configuration" );




   m_desc.add_options()
       ("primary",  "Enable recording of primary vertices in cfg4-, stored in NumpyEvt primary ") ;



   m_desc.add_options()
       ("zexplode",  "explode mesh in z for debugging split unions") ;

   m_desc.add_options()
       ("zexplodeconfig",   boost::program_options::value<std::string>(&m_zexplodeconfig), "zexplode configuration" );





   m_desc.add_options()
       ("snap",  "Enable non-OpenGL rendering to ppm files, use --snapconfig to configure, see npy-/NSnapConfig  ") ;

   m_desc.add_options()
       ("snapconfig",   boost::program_options::value<std::string>(&m_snapconfig), "snap (non-OpenGL rendering to ppm) configuration" );


   m_desc.add_options()
       ("g4snap",  "Enable G4TheRayTracer snapping to jpeg files, use --g4snapconfig to configure, see cfg4-/CRayTracer  ") ;

   m_desc.add_options()
       ("g4snapconfig",   boost::program_options::value<std::string>(&m_g4snapconfig), "g4snap ray trace configuration" );






   m_desc.add_options()
       ("apmtload",  "Load Analytic PMT : Opticks::isAnalyticPMTLoad, see GGeo::loadGeometry. "
                     "This option prevents always loading the analytic PMT, as it is normally not used") ;

   char apmtidx[128];
   snprintf(apmtidx,128, "Analytic PMT index. Default %d", m_apmtidx);
   m_desc.add_options()
       ("apmtidx",  boost::program_options::value<int>(&m_apmtidx), apmtidx );

   m_desc.add_options()
       ("apmtmedium",   boost::program_options::value<std::string>(&m_apmtmedium), "material name of PMT medium" );


   char modulo[128];
   snprintf(modulo,128, "Modulo scaledown input gensteps for speed/debugging, eg 10 to decimate. Values less than 1 disable scaledown. Default %d", m_modulo);
   m_desc.add_options()
       ("modulo",  boost::program_options::value<int>(&m_modulo), modulo );

   char propagateoverride[256];
   snprintf(propagateoverride,256, 
          "Override photons to propagate for debugging, eg 1 for a single photon. Value of zero disables any override. Negative values are assumes to in millions. Default %d", m_propagateoverride);
   m_desc.add_options()
       ("propagateoverride",  boost::program_options::value<int>(&m_propagateoverride), propagateoverride );


   char generateoverride[256];
   snprintf(generateoverride,256, 
          "Override photons to generate for debugging, eg 1 for a single photon. Values of zero disables any override, Negative values are assumed to be in millions. Default %d", m_generateoverride);
   m_desc.add_options()
       ("generateoverride",  boost::program_options::value<int>(&m_generateoverride), generateoverride );



   char debugidx[128];
   snprintf(debugidx,128, "Index of item eg Photon for debugging. Default %d", m_debugidx);
   m_desc.add_options()
       ("debugidx",  boost::program_options::value<int>(&m_debugidx), debugidx );


   char dbgnode[128];
   snprintf(dbgnode,128, "Index of volume node for debugging, see OpticksHub::debugNode. Default %d", m_dbgnode);
   m_desc.add_options()
       ("dbgnode",  boost::program_options::value<int>(&m_dbgnode), dbgnode );


   char dbgmm[128];
   snprintf(dbgmm,128, "Index of merged mesh for debugging. Default %d", m_dbgmm);
   m_desc.add_options()
       ("dbgmm",  boost::program_options::value<int>(&m_dbgmm), dbgmm );

   char dbglv[128];
   snprintf(dbglv,128, "Index of solid/LV for debugging. Default %d", m_dbglv);
   m_desc.add_options()
       ("dbglv",  boost::program_options::value<int>(&m_dbglv), dbglv );




   char dbgmesh[128];
   snprintf(dbgmesh,128, "Name of mesh solid for debugging, see GMeshLibTest, invoke inside environment with --gmeshlib option. Default %s", m_dbgmesh.c_str());
   m_desc.add_options()
       ("dbgmesh",  boost::program_options::value<std::string>(&m_dbgmesh), dbgmesh );



   char stack[128];
   snprintf(stack,128, "OptiX stack size, smaller the faster util get overflows. Default %d", m_stack);
   m_desc.add_options()
       ("stack",  boost::program_options::value<int>(&m_stack), stack );

   char epsilon[128];
   snprintf(epsilon,128, "OptiX propagate epsilon. Default %10.4f", m_epsilon);
   m_desc.add_options()
       ("epsilon",  boost::program_options::value<float>(&m_epsilon), epsilon );


   char g4ppe[256];
   snprintf(g4ppe,256, 
"Number of torch photons to generate/propagate per event with Geant4 cfg4.sh,"
" changing the number of events to meet the photon total. Default %d", m_num_photons_per_g4event);
   m_desc.add_options()
       ("g4ppe",  boost::program_options::value<int>(&m_num_photons_per_g4event), g4ppe );

   m_desc.add_options()
       ("alt,a",  "use alternative record renderer") ;

   m_desc.add_options()
       ("fullscreen,f",  "start in fullscreen mode") ;

   m_desc.add_options()
       ("tag",   boost::program_options::value<std::string>(&m_event_tag), "non zero positive integer string identifying an event" );

   m_desc.add_options()
       ("itag",   boost::program_options::value<std::string>(&m_integrated_event_tag), "integrated eventtag to load/save, used from OPG4 package" );


   m_desc.add_options()
       ("cat",   boost::program_options::value<std::string>(&m_event_cat), "event category for organization of event files, typically used instead of detector for test geometries such as prism and lens" );

   m_desc.add_options()
       ("pfx",   boost::program_options::value<std::string>(&m_event_pfx), "event prefix for organization of event files, typically source or the name of the creating executable or the testname" );




   m_desc.add_options()
       ("state",   boost::program_options::value<std::string>(&m_state_tag), "Bookmarks state tag, allowing use of multiple collections of bookmarks." );

   m_desc.add_options()
       ("materialprefix",   boost::program_options::value<std::string>(&m_materialprefix), "Materials prefix string eg /dd/Materials/ " );



   m_desc.add_options()
       ("meshversion",   boost::program_options::value<std::string>(&m_meshversion), "debug only option for testing alternate mesh versions" );

   m_desc.add_options()
       ("rendermode",   boost::program_options::value<std::string>(&m_rendermode), "debug only rendermode, see oglrap-/OpticksViz::prepareScene" );

   m_desc.add_options()
       ("rendercmd",   boost::program_options::value<std::string>(&m_rendercmd), "debug only rendercmd, see oglrap-/OpticksViz::prepareScene" );



   m_desc.add_options()
       ("islice",        boost::program_options::value<std::string>(&m_islice), "debug only option for use of partial instanced geometry, specified by python slice style colon delimited ints " );

   m_desc.add_options()
       ("fslice",        boost::program_options::value<std::string>(&m_fslice), "debug only option for use of partial face geometry, specified by python slice style colon delimited ints " );

   m_desc.add_options()
       ("pslice",        boost::program_options::value<std::string>(&m_pslice), "debug only option for selecting parts of analytic geometry, specified by python slice style colon delimited ints " );

   m_desc.add_options()
       ("instancemodulo",  boost::program_options::value<std::string>(&m_instancemodulo), "debug only option for selecting modulo scaledowns of analytic geometry instance counts eg 1:5,2:10 for mm1 and mm2 modulo scaledowns of 5 and 10 " );



   m_desc.add_options()
       ("printenabled",  "default to enabling rtPrint,  see oxrap-/OContext") ; 

   m_desc.add_options()
       ("exceptionenabled",  "enable exceptions, but only when printenabled or pindex is also enabled, see oxrap-/OContext::initPrint ") ; 

   m_desc.add_options()
       ("pindexlog",  "Use redirect to write stdout rtPrintf from OptiX launch to a logfile, see oxrap-/OContext") ; 


   m_desc.add_options()
       ("pindex",        boost::program_options::value<std::string>(&m_pindex), 
                     "debug OptiX launch print index specified by three comma delimited ints,"
                     " giving 3D spec of single launch slot to enable debugging for "
                   );

   m_desc.add_options()
       ("dindex",        boost::program_options::value<std::string>(&m_dindex), "debug photon_id indices in comma delimited list of ints, no size limit" );
   m_desc.add_options()
       ("oindex",        boost::program_options::value<std::string>(&m_oindex), "other debug photon_id indices in comma delimited list of ints, no size limit" );
   m_desc.add_options()
       ("gindex",        boost::program_options::value<std::string>(&m_gindex), 
               "generate debug photon_id indices in comma delimited list of ints, no size limit"
               "Used to CPU side restrict photons to generate in CGenstepSource::addPrimaryVertices "    
       );



   m_desc.add_options()
       ("mask",     boost::program_options::value<std::string>(&m_mask), 
                    "comma delimited list of photon indices specifying mask selection to apply to both simulations"
                    "see OpticksDbg, CInputPhotonSource, CRandomEngine"
                    "notes/issues/where_mask_running.rst "
                 );


   m_desc.add_options()
       ("x4polyskip",    boost::program_options::value<std::string>(&m_x4polyskip), 
                  "comma delimited string listing lvIdx indices, no size limit," 
                  "specifying solids to skip from Geant4 polygonization. "
                  "Instead bounding box placeholders are used. "
                  "This avoids G4 boolean processor infinite loop issue with some deep CSG trees."
 
         );

   char csgskiplv[256];
   snprintf(csgskiplv,256, "comma delimited string listing lvIdx indices to skip in direct geometry conversion. Default %s", m_csgskiplv.c_str() );
   m_desc.add_options()
       ("csgskiplv",  boost::program_options::value<std::string>(&m_csgskiplv), csgskiplv );


   m_desc.add_options()
       ("accel",        boost::program_options::value<std::string>(&m_accel), "OptiX Accel structure builder, comma delimited list. See OGeo. CAUTION case sensitive Bvh/Trbvh/Sbvh/NoAccel  ");

   m_desc.add_options()
       ("dbgseqhis",      boost::program_options::value<std::string>(&m_dbgseqhis), "Debug photon history hex string" );

   m_desc.add_options()
       ("dbgseqmat",      boost::program_options::value<std::string>(&m_dbgseqmat), "Debug photon material hex string" );


   m_desc.add_options()
       ("seqmap",      boost::program_options::value<std::string>(&m_seqmap), "Debug photon history hex string value map, used eg for NCSGIntersect checks via OpticksAnaEvent." );


   char lvsdname[512]; 
   snprintf(lvsdname,512, 
"When lvsdname is blank logical volumes with an SD associated are used. "
"As workaround for GDML not persisting the SD it is also possible to identify SD by their LV names, using this option. "
"Provide a comma delimited string with substrings to search for in the logical volume names "
"when found the volumes will be treated as sensitive detectors, see X4PhysicalVolume::convertSensors " 
"Default %s ",  m_lvsdname.c_str() );

   m_desc.add_options()
       ("lvsdname",   boost::program_options::value<std::string>(&m_lvsdname), lvsdname ) ; 


   char cathode[128]; 
   snprintf(cathode,128, 
"Shortname of the cathode material. Default %s ", m_cathode.c_str() );
   m_desc.add_options()
       ("cathode",   boost::program_options::value<std::string>(&m_cathode), cathode ) ; 



   char cerenkovclass[128]; 
   snprintf(cerenkovclass,128, 
"Name of the Cerenkov class to use in CFG4.CPhysicsList. Default %s ", m_cerenkovclass.c_str() );
   m_desc.add_options()
       ("cerenkovclass",   boost::program_options::value<std::string>(&m_cerenkovclass), cerenkovclass ) ; 


   char scintillationclass[128]; 
   snprintf(scintillationclass,128, 
"Name of the Scintillation class to use in CFG4.CPhysicsList. Default %s ", m_scintillationclass.c_str() );
   m_desc.add_options()
       ("scintillationclass",   boost::program_options::value<std::string>(&m_scintillationclass), scintillationclass ) ; 







   char seed[128];
   snprintf(seed,128, 
"Unsigned int seed used with NEmitPhotons/BRng"
"Default %d ", m_seed);
   m_desc.add_options()
       ("seed",  boost::program_options::value<unsigned>(&m_seed), seed );


   char rngmax[256];
   snprintf(rngmax,256, 
"Maximum number of photons (in millions) that can be generated/propagated as limited by the number of pre-persisted curand streams. "
"Value must match envvar CUDAWRAP_RNG_MAX and corresponding pre-cooked seeds, see cudawrap- for details. "
"Default %d ", m_rngmax);

   m_desc.add_options()
       ("rngmax",  boost::program_options::value<int>(&m_rngmax), rngmax );

   char bouncemax[128];
   snprintf(bouncemax,128, 
"Maximum number of boundary bounces, 0:prevents any propagation leaving generated photons"
"Default %d ", m_bouncemax);
   m_desc.add_options()
       ("bouncemax,b",  boost::program_options::value<int>(&m_bouncemax), bouncemax );







   // keeping bouncemax one less than recordmax is advantageous 
   // as bookeeping is then consistent between the photons and the records 
   // as this avoiding truncation of the records

   char recordmax[128];
   snprintf(recordmax,128, 
"Maximum number of photon step records per photon, 1:to minimize without breaking machinery. Default %d ", m_recordmax);
   m_desc.add_options()
       ("recordmax,r",  boost::program_options::value<int>(&m_recordmax), recordmax );






   char timemaxthumb[256];
   snprintf(timemaxthumb,256, 
"Adjust the rule-of-thumb factor for setting timemax based in spatial extent of geometry, see Opticks::setupTimeDomain. Default %f.", m_timemaxthumb);
   m_desc.add_options()
       ("timemaxthumb",  boost::program_options::value<float>(&m_timemaxthumb), timemaxthumb );




   char timemax[256];
   snprintf(timemax,256, 
"Maximum time in nanoseconds. A negative value sets the timedomain using a rule of thumb based on geometry extent. Default %f.", m_timemax);
   m_desc.add_options()
       ("timemax",  boost::program_options::value<float>(&m_timemax), timemax );


   char animtimerange[256];
   snprintf(animtimerange,256, 
"Comma delimited times in nanoseconds specifying the animation time range, negative value sets the range using a rule of thumb based on geometry extent. Default %s.", m_animtimerange.c_str());
   m_desc.add_options()
       ("animtimerange",  boost::program_options::value<std::string>(&m_animtimerange), animtimerange );


   char animtimemax[128];
   snprintf(animtimemax,128, 
"Maximum animation time in nanoseconds. Default %f ", m_animtimemax);
   m_desc.add_options()
       ("animtimemax",  boost::program_options::value<float>(&m_animtimemax), animtimemax );

   char animator_period[128];
   snprintf(animator_period,128, "Event Animator Period, typically in range 200 to 400 controlling the number of steps of the animation. Default %d ", m_animator_period);
   m_desc.add_options()
       ("evperiod",  boost::program_options::value<int>(&m_animator_period), animator_period);

   char ivperiod[128];
   snprintf(ivperiod,128, "Interpolated View Period, typically in range 50 to 400 controlling the number of steps of the animation. Default %d ", m_ivperiod);
   m_desc.add_options()
       ("ivperiod",  boost::program_options::value<int>(&m_ivperiod), ivperiod);

   char ovperiod[128];
   snprintf(ovperiod,128, "Orbital View Period, typically in range 180 to 360 controlling the number of steps of the animation. Default %d ", m_ovperiod);
   m_desc.add_options()
       ("ovperiod",  boost::program_options::value<int>(&m_ovperiod), ovperiod);

   char tvperiod[128];
   snprintf(tvperiod,128, "Track View Period, typically in range 50 to 200 controlling the number of steps of the animation. Default %d ", m_tvperiod);
   m_desc.add_options()
       ("tvperiod",  boost::program_options::value<int>(&m_tvperiod), tvperiod);




   char flightpathdir[256]; 
   snprintf(flightpathdir,256, 
"Directory from which to load flightpath.npy style persisted view for use in InterpolatedView/FlightPath. Default %s ",  m_flightpathdir.c_str() );

   m_desc.add_options()
       ("flightpathdir",   boost::program_options::value<std::string>(&m_flightpathdir), flightpathdir ) ; 



   char repeatidx[128];
   snprintf(repeatidx,128, "Repeat index used in development of instanced geometry, -1:flat full geometry. Default %d ", m_repeatidx);
   m_desc.add_options()
       ("repeatidx",  boost::program_options::value<int>(&m_repeatidx), repeatidx );


   char multievent[128];
   snprintf(multievent,128, "Multievent count used in development of multiple event propagation. Default %d ", m_multievent);
   m_desc.add_options()
       ("multievent",  boost::program_options::value<int>(&m_multievent), multievent);



   char enabledmergedmesh[128];
   snprintf(enabledmergedmesh,128, "(former restrictmesh) Comma delimited string giving list of mesh indices to convert into OptiX geometry eg \"0,2,5\". Or blank for all. Default %s ", m_enabledmergedmesh.c_str() );
   m_desc.add_options()
       ("enabledmergedmesh",  boost::program_options::value<std::string>(&m_enabledmergedmesh), enabledmergedmesh );

   char analyticmesh[128];
   snprintf(analyticmesh,128, "Index of instanced mesh with which to attempt analytic OptiX geometry eg 1,2. Or -1 for no analytic geometry. Default %d ", m_analyticmesh);
   m_desc.add_options()
       ("analyticmesh",  boost::program_options::value<int>(&m_analyticmesh), analyticmesh );


   char cameratype[128];
   snprintf(cameratype,128, "Initial cameratype 0:PERSPECTIVE,1:ORTHOGRAPHIC,2:EQUIRECTANGULAR (only raytrace). Default %d ", m_cameratype);
   m_desc.add_options()
       ("cameratype",  boost::program_options::value<int>(&m_cameratype), cameratype );





   char loadverbosity[128];
   snprintf(loadverbosity,128, "Geometry Loader Verbosity eg AssimpGGeo.  Default %d ", m_loadverbosity);
   m_desc.add_options()
       ("loadverbosity",  boost::program_options::value<int>(&m_loadverbosity), loadverbosity );

   char importverbosity[128];
   snprintf(importverbosity,128, "Geometry Importer Verbosity eg AssimpImporter.  Default %d ", m_importverbosity);
   m_desc.add_options()
       ("importverbosity",  boost::program_options::value<int>(&m_importverbosity), importverbosity );



   char meshverbosity[128];
   snprintf(meshverbosity,128, "Mesh Operation Verbosity eg GMergedMesh::Create.  Default %d ", m_meshverbosity);
   m_desc.add_options()
       ("meshverbosity",  boost::program_options::value<int>(&m_meshverbosity), meshverbosity );

   char verbosity[128];
   snprintf(verbosity,128, "General Opticks Verbosity.  Default %d ", m_verbosity);
   m_desc.add_options()
       ("verbosity",  boost::program_options::value<int>(&m_verbosity), verbosity );



   ///////////////

   m_desc.add_options()
       ("cvd",  boost::program_options::value<std::string>(&m_cvd),
            "String CUDA_VISIBLE_DEVICES to be set as an envvar internally within Opticks resource setup."
            "Useful when using an external envvar is inconvenient, such as when using gdb ");


   m_desc.add_options()
       ("size",  boost::program_options::value<std::string>(&m_size),
            "Comma delimited screen window coordinate width,height,window2pixel eg 1024,768,2  ");

   m_desc.add_options()
       ("position",  boost::program_options::value<std::string>(&m_position),
            "Comma delimited screen window upper left coordinate x,y,-,- eg 100,100  "
            "NB although 0,0 is screen top left the application title bar prevents positioning of the window over pixels 0:20 (approx) in y. " 
            "Also when the frame size is large positioning is constrained"
       );


   m_desc.add_options()
       ("dbgcsgpath",   boost::program_options::value<std::string>(&m_dbgcsgpath),
         "Path to directory containing NCSG trees for debugging, loaded via NCSGList.");

   m_desc.add_options()
       ("logname",   boost::program_options::value<std::string>(&m_logname),
         "name of logfile");

   m_desc.add_options()
       ("config",   boost::program_options::value<std::string>(&m_configpath),
         "name of a file of a configuration.");

   m_desc.add_options()
       ("liveline",  boost::program_options::value<std::string>(&m_liveline),
           "string with spaces to be live parsed, as test of composed overrides");




   char srcgltfbase[128];
   snprintf(srcgltfbase,128, "String identifying directory of glTF geometry files to load with --gltf option. Default %s ", m_srcgltfbase.c_str() );
   m_desc.add_options()
       ("srcgltfbase",   boost::program_options::value<std::string>(&m_srcgltfbase), srcgltfbase );

   char srcgltfname[128];
   snprintf(srcgltfname,128, "String identifying name of glTF geometry file to load with --gltf option. Default %s ", m_srcgltfname.c_str() );
   m_desc.add_options()
       ("srcgltfname",   boost::program_options::value<std::string>(&m_srcgltfname), srcgltfname );

   char gltfconfig[128];
   snprintf(gltfconfig,128, "String configuring glTF geometry file loading with --gltf option. Default %s ", m_gltfconfig.c_str() );
   m_desc.add_options()
       ("gltfconfig",   boost::program_options::value<std::string>(&m_gltfconfig), gltfconfig );

   char gltftarget[128];
   snprintf(gltftarget,128, "Integer specifying glTF geometry node target for partial geometry, Default %d ", m_gltftarget );
   m_desc.add_options()
       ("gltftarget",  boost::program_options::value<int>(&m_gltftarget), gltftarget );


   m_desc.add_options()
       ("gltf",  boost::program_options::value<int>(&m_gltf), 
             "Integer controlling  glTF analytic geometry file loading. "
             "The default of zero uses old triangulated approach "
             "Non-zero values use glTF analytic geometry. "
             ""
             "gltf == 3 combines G4 triangulation for OpenGL viz (lowpoly and more reliable that home grown poly)   "
             "with glTF analytic for raytrace and simulation (see GScene::createVolume)  "                 
       );


   char layout[128];
   snprintf(layout,128, "Integer controlling OPTICKS_RESOURCE_LAYOUT, for alternate geocache dir.. Default %d ", m_layout );
   m_desc.add_options()
       ("layout",  boost::program_options::value<int>(&m_layout), layout );


   char lodconfig[128];
   snprintf(lodconfig,128, "String configuring LOD (level-of-detail) meshing, which is controlled with --lod option. Default %s ", m_lodconfig.c_str() );
   m_desc.add_options()
       ("lodconfig",   boost::program_options::value<std::string>(&m_lodconfig), lodconfig );

   char lod[128];
   snprintf(lod,128, "integer controlling lod (level-of-detail) meshing. default %d ", m_lod );
   m_desc.add_options()
       ("lod",  boost::program_options::value<int>(&m_lod), lod );


   m_desc.add_options()
       ("raylod",  
              "Switch on experimental OptiX raytrace level-of-detail (LOD) via visit_instance.cu based geometry selectors" 
              "See oxrap/OGeo.cc:makeRepeatedGroup and notes/issues/can-optix-selector-defer-expensive-csg.rst "
              ""
              "This is entirely distinct from the OpenGL mesh based LOD.") ;


   char gpumonpath[128];
   snprintf(gpumonpath,128, "Path to write the GPU buffer usage monitor file. Default %s ", m_gpumonpath.c_str() );
   m_desc.add_options()
       ("gpumonpath",   boost::program_options::value<std::string>(&m_gpumonpath), gpumonpath );

   m_desc.add_options()
       ("gpumon", "Switch on GPU buffer usage recording. ");   


   char runcomment[128];
   snprintf(runcomment,128, "Informational comment used for run intent identification. Default %s ", m_runcomment.c_str() );
   m_desc.add_options()
       ("runcomment",   boost::program_options::value<std::string>(&m_runcomment), runcomment );


   char runstamp[256];
   snprintf(runstamp,256, "Integer seconds from the epoch. Overriding the default of now %d "
                          "enables grouping together related runs under the same stamp. Use \"date +%%s\" in the shell.", m_runstamp );
   m_desc.add_options()
       ("runstamp",   boost::program_options::value<int>(&m_runstamp), runstamp );


   char runlabel[128];
   snprintf(runlabel,128, "Short string used for result identification. Default %s ", m_runlabel.c_str() );
   m_desc.add_options()
       ("runlabel",   boost::program_options::value<std::string>(&m_runlabel), runlabel );

   char runfolder[128];
   snprintf(runfolder,128, "Short string used organizing result output in the filesystem. Default %s ", m_runfolder.c_str() );
   m_desc.add_options()
       ("runfolder",   boost::program_options::value<std::string>(&m_runfolder), runfolder );





           


   char target[128];
   snprintf(target,128, "Integer controlling center_extent target. Default %d ", m_target );
   m_desc.add_options()
       ("target",  boost::program_options::value<int>(&m_target), target );



   char alignlevel[128];
   snprintf(alignlevel,128, "Integer controlling align verbosity, see CRandomEngine. Default %d ", m_alignlevel );
   m_desc.add_options()
       ("alignlevel",  boost::program_options::value<int>(&m_alignlevel), alignlevel );




    // the below formerly called size seems not to be working, so use simpler size above 
   addOptionS<Listener>(m_listener, 
            "livesize", 
            "Comma delimited screen window coordinate width,height,window2pixel eg 1024,768,2  ");

   // this size is being overriden: 
   // the screen size is set by Opticks::init using size from composition 

   addOptionI<Listener>(m_listener, 
            "dumpevent", 
            "Control GLFW event dumping ");






}   



template <class Listener>
const std::string& OpticksCfg<Listener>::getDbgCSGPath()
{
    return m_dbgcsgpath ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getLogName()
{
    return m_logname ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getCVD()
{
    return m_cvd ;
}




template <class Listener>
const std::string OpticksCfg<Listener>::SIZE_P = "1280,720,1" ; 


template <class Listener>
const std::string& OpticksCfg<Listener>::getSize() const 
{
    bool is_p = strcmp(m_size.c_str(), "p") == 0 ; 
    return is_p ? SIZE_P : m_size ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getPosition()
{
    return m_position ;
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getConfigPath()
{
    return m_configpath ;
}




template <class Listener>
const char* OpticksCfg<Listener>::getEventTag() const 
{
    return m_event_tag.empty() ? NULL : m_event_tag.c_str() ;
}
template <class Listener>
const char* OpticksCfg<Listener>::getEventCat() const 
{
    const char* cat_envvar_default = SSys::getenvvar("TESTNAME" , NULL ); 
    return m_event_cat.empty() ? cat_envvar_default : m_event_cat.c_str() ;
}
template <class Listener>
const char* OpticksCfg<Listener>::getEventPfx() const 
{
    const char* pfx_envvar_default = SSys::getenvvar("TESTNAME" , NULL ); 
    return m_event_pfx.empty() ? pfx_envvar_default : m_event_pfx.c_str() ;
}


// no longer used ?
template <class Listener>
const char* OpticksCfg<Listener>::getIntegratedEventTag() const 
{
    return m_integrated_event_tag.empty() ? NULL :  m_integrated_event_tag.c_str() ;
}






template <class Listener>
const std::string& OpticksCfg<Listener>::getStateTag()
{
    return m_state_tag ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getMaterialPrefix()
{
    return m_materialprefix ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getLiveLine()
{
    return m_liveline ;
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getTorchConfig()
{
    return m_torchconfig ;
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getG4IniMac()
{
    return m_g4inimac ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getG4RunMac()
{
    return m_g4runmac ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getG4FinMac()
{
    return m_g4finmac ;
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getG4GunConfig() const 
{
    return m_g4gunconfig ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getAnaKey() const 
{
    return m_anakey ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getAnaKeyArgs() const 
{
    return m_anakeyargs ;
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getTestConfig()
{
    return m_testconfig ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getExportConfig()
{
    return m_exportconfig ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getSnapConfig()
{
    return m_snapconfig ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getG4SnapConfig()
{
    return m_g4snapconfig ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getZExplodeConfig()
{
    return m_zexplodeconfig ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getMeshVersion()
{
    return m_meshversion ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getRenderMode()
{
    return m_rendermode ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getRenderCmd()
{
    return m_rendercmd ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getISlice()
{
    return m_islice ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getFSlice()
{
    return m_fslice ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getPSlice()
{
    return m_pslice ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getInstanceModulo()
{
    return m_instancemodulo ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getPrintIndex() const 
{
    return m_pindex ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getDbgIndex() const
{
    return m_dindex ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getGenIndex() const
{
    return m_gindex ;
}





template <class Listener>
const std::string& OpticksCfg<Listener>::getDbgMesh() const 
{
    return m_dbgmesh ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getOtherIndex()
{
    return m_oindex ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getMask() const 
{
    return m_mask ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getX4PolySkip() const 
{
    return m_x4polyskip ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getCSGSkipLV() const   // --csgskiplv
{
    return m_csgskiplv ; 
}






template <class Listener>
const std::string& OpticksCfg<Listener>::getAccel()
{
    return m_accel ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getSeqMap() const 
{
    return m_seqmap  ;
}

template <class Listener>
void OpticksCfg<Listener>::setSeqMap(const char* seqmap)
{
    m_seqmap = seqmap  ;
}






template <class Listener>
const std::string& OpticksCfg<Listener>::getDbgSeqhis()
{
    return m_dbgseqhis  ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getDbgSeqmat()
{
    return m_dbgseqmat  ;
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getFxReConfig()
{
    return m_fxreconfig  ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getFxAbConfig()
{
    return m_fxabconfig  ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getFxScConfig()
{
    return m_fxscconfig  ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getAnalyticPMTSlice()
{
    return m_apmtslice  ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getFlightPathDir()
{
    return m_flightpathdir   ;
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getAnalyticPMTMedium()
{
    return m_apmtmedium  ;
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getLVSDName()
{
    return m_lvsdname  ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getCathode()
{
    return m_cathode  ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getCerenkovClass()
{
    return m_cerenkovclass  ;
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getScintillationClass()
{
    return m_scintillationclass  ;
}





template <class Listener>
float OpticksCfg<Listener>::getEpsilon() const
{
    return m_epsilon ; 
}
template <class Listener>
float OpticksCfg<Listener>::getPixelTimeScale() const
{
    return m_pixeltimescale ; 
}


template <class Listener>
unsigned OpticksCfg<Listener>::getSeed() const 
{
    return m_seed ; 
}
template <class Listener>
int OpticksCfg<Listener>::getRTX() const 
{
    return m_rtx ; 
}

template <class Listener>
int OpticksCfg<Listener>::getRenderLoopLimit() const 
{
    return m_renderlooplimit ; 
}

template <class Listener>
int OpticksCfg<Listener>::getRngMax()
{
    return m_rngmax*m_rngmaxscale ; 
}

template <class Listener>
int OpticksCfg<Listener>::getBounceMax()
{
    return m_bouncemax ; 
}

template <class Listener>
int OpticksCfg<Listener>::getRecordMax()
{
    return m_recordmax ; 
}

template <class Listener>
float OpticksCfg<Listener>::getTimeMaxThumb() const   // --timemaxthumb
{
    return m_timemaxthumb ; 
}

template <class Listener>
float OpticksCfg<Listener>::getTimeMax() const   // --timemax
{
    return m_timemax ; 
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getAnimTimeRange() const 
{
    return m_animtimerange  ;
}

template <class Listener>
float OpticksCfg<Listener>::getAnimTimeMax() const   // --animtimemax
{
    return m_animtimemax ; 
}

template <class Listener>
int OpticksCfg<Listener>::getInterpolatedViewPeriod()
{
    return m_ivperiod ; 
}

template <class Listener>
int OpticksCfg<Listener>::getOrbitalViewPeriod()
{
    return m_ovperiod ; 
}

template <class Listener>
int OpticksCfg<Listener>::getTrackViewPeriod()
{
    return m_tvperiod ; 
}
template <class Listener>
int OpticksCfg<Listener>::getAnimatorPeriod()
{
    return m_animator_period ; 
}



template <class Listener>
int OpticksCfg<Listener>::getRepeatIndex()
{
    return m_repeatidx ; 
}
template <class Listener>
int OpticksCfg<Listener>::getMultiEvent() const
{
    return m_multievent ; 
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getEnabledMergedMesh() const 
{
    return m_enabledmergedmesh ; 
}
template <class Listener>
int OpticksCfg<Listener>::getAnalyticMesh() const 
{
    return m_analyticmesh ; 
}

template <class Listener>
int OpticksCfg<Listener>::getCameraType() const 
{
    return m_cameratype ; 
}





template <class Listener>
int OpticksCfg<Listener>::getModulo()
{
    return m_modulo ; 
}

template <class Listener>
int OpticksCfg<Listener>::getGenerateOverride()
{
    int go = m_generateoverride ; 
    return go < 0 ? -go*1000000 : go ; 
}

template <class Listener>
int OpticksCfg<Listener>::getPropagateOverride()
{
    int po = m_propagateoverride ; 
    return po < 0 ? -po*1000000 : po ; 
}





template <class Listener>
int OpticksCfg<Listener>::getDebugIdx() const 
{
    return m_debugidx ; 
}

template <class Listener>
int OpticksCfg<Listener>::getDbgNode() const 
{
    return m_dbgnode ; 
}

template <class Listener>
int OpticksCfg<Listener>::getDbgMM() const 
{
    return m_dbgmm ; 
}

template <class Listener>
int OpticksCfg<Listener>::getDbgLV() const 
{
    return m_dbglv ; 
}






template <class Listener>
int OpticksCfg<Listener>::getStack() const 
{
    return m_stack ; 
}




template <class Listener>
int OpticksCfg<Listener>::getNumPhotonsPerG4Event()
{
    return m_num_photons_per_g4event ; 
}

template <class Listener>
int OpticksCfg<Listener>::getLoadVerbosity()
{
    return m_loadverbosity ; 
}
template <class Listener>
int OpticksCfg<Listener>::getImportVerbosity()
{
    return m_importverbosity ; 
}


template <class Listener>
int OpticksCfg<Listener>::getMeshVerbosity()
{
    return m_meshverbosity ; 
}


template <class Listener>
int OpticksCfg<Listener>::getVerbosity()
{
    return m_verbosity ; 
}




template <class Listener>
int OpticksCfg<Listener>::getAnalyticPMTIndex()
{
    return m_apmtidx ; 
}



template <class Listener>
const std::string& OpticksCfg<Listener>::getGPUMonPath() const 
{
    return m_gpumonpath ;  
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getRunComment() const 
{
    return m_runcomment ;  
}
template <class Listener>
int OpticksCfg<Listener>::getRunStamp() const 
{
    return m_runstamp ; 
}

template <class Listener>
const std::string& OpticksCfg<Listener>::getRunLabel() const 
{
    return m_runlabel ;  
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getRunFolder() const 
{
    return m_runfolder ;  
}






template <class Listener>
const std::string& OpticksCfg<Listener>::getSrcGLTFBase()
{
   return m_srcgltfbase ; 
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getSrcGLTFName()
{
   return m_srcgltfname ; 
}
template <class Listener>
const std::string& OpticksCfg<Listener>::getGLTFConfig()
{
   return m_gltfconfig ; 
}

template <class Listener>
int OpticksCfg<Listener>::getGLTF()
{
    return m_gltf ; 
}


template <class Listener>
int OpticksCfg<Listener>::getGLTFTarget()
{
    return m_gltftarget ; 
}

template <class Listener>
int OpticksCfg<Listener>::getLayout() const 
{
    return m_layout ; 
}


template <class Listener>
const std::string& OpticksCfg<Listener>::getLODConfig()
{
   return m_lodconfig ; 
}

template <class Listener>
int OpticksCfg<Listener>::getLOD() const 
{
    return m_lod ; 
}


template <class Listener>
int OpticksCfg<Listener>::getTarget() const 
{
    return m_target ; 
}


template <class Listener>
int OpticksCfg<Listener>::getAlignLevel() const 
{
    return m_alignlevel ; 
}






template <class Listener>
void OpticksCfg<Listener>::dump(const char* msg)
{
    LOG(info) << msg ;
    
    std::vector<std::string> names ; 
    names.push_back("compute");
    names.push_back("save");
    names.push_back("load");
    names.push_back("test");

    names.push_back("g4gun");

    for(unsigned int i=0 ; i < names.size() ; i++)
    {
        LOG(info) << std::setw(10) << names[i] << " " <<  hasOpt(names[i].c_str()) ;
    }
}


template class OKCORE_API OpticksCfg<Opticks> ;

