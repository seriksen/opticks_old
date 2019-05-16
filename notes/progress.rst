Progress
=========


Updating 
----------

Use *hg-;hg-month n* to review commits for the numbered month, 
negative n eg -12 for December of last year.


`notes-progress` summaries
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This progress text is parsed by `bin/progress.py` in preparation of ``notes-progress`` summaries, 
to work with that parser follow some rules:

1. title lines have a colon after the date, suppress a title by using semi-colon instead
2. other lines have no colons
3. bullet lines to be included in the summary should be in bold


2019 May 
--------

* Taiwan trip 4/1-8 

  * mulling over sphere tracing SDF implicits as workaround for Torus (guidetube)
    and perhaps optimization for PMT 
  * reading on deep learning 
  * working with NEXO user 

* add Linux time/memory profiling : to start investigating the memory hungry translation 

* resume writing 


2019 April
-----------

* work with user to fix issue on Ubuntu 18.04.2 with gcc 7.3.0 

  * virtualbox proved very handy for reproducing user issues

* failed to get Linux containers LXD working on Precision (snap problem with SELinux)

* updating to OptiX 6.0.0. in a hurry to profit from borrowed NVIDIA RTX, proved eventful

  * NVIDIA driver update somehow conspired with long dormant "sleeper" visualization bug 
    to wakeup at just the wrong moment : causing a week of frenzied debugging 
    due to limited time to borrow the 

  * resulted in a development of quite a few OpenGL + OptiX minimal test case examples 
  * optix::GeometryTriangles 
  * GDML editing to remove torus : as causes crash with OptiX 6.0.0
  * ended up buying the RTX GPU 

* developed tarball distribution opticks-dist-*  adopted ORIGIN/.. RPATH
* setup opticks area of cvmfs : for when am ready to make a release
* Opticks installed onto GPU cluster

  * got bad alloc memory issue on lxslc, workaround is to do translation where have more memory 

* raycast benchmark to test NVIDIA RTX 
  

2019 March
-----------

* getting back in saddle after ~5 months hiatus
* redtape : not as bad as last year 
* improve CAlignEngine error handling of missing seq
* getting logging under control 
* Qingdao 2nd Geant4 school in China 3/25-29


2018 October
-------------

* CHEP 2018 proceedings
* viz flightpath enhancements, simple control language 

2018 September
---------------

* CCerenkovGenerator : G4-G4 matching to 1e-8 : so can resume from gensteps, bi-executable convenience
* PMT neck tests : hyperboloid/cone 
* Qingdao seminar ~21st (1.5hr), preparation in env repo
* looking into usage of GPUs for reconstruction

2018 August
-------------

* AB test validating the direct geometry by comparison of geometry NPY buffers

  * plethora of issues surfaces/materials/boundaries/sensors 
  * only way to get a match is to fix problems both in the old and new approaches, 
    even down to the forked assimp external 

* start prototype "user" example project : "CerenkovMinimal" 

  * with SensitiveDetector, Hit collections etc..
  * configured against only the G4OK interface project 
  * used for guiding development of the G4OK package, that
    provides interface between Geant4 user code with an embedded Opticks propagator

* update to Geant4 10.4.2 in preparation for aligned validation 

* adopt two executable with shared geocache pattern for validation,
  (expanding on tboolean using the new capabilities of direct translation of 
   any geometry)

  * 1st executable : anything from a simple Geant4 example to a full detector simulation package 
    with Opticks embedded inside the Geant4 user code using the G4OK package 

  * 2nd executable : operating from geocache+gensteps persisted from the 1st executable 

    * fully instrumented gorilla (records all steps of all photons) OKG4Test executable, 
      with Geant4 embedded inside Opticks 
    * simple purely optical physics : "cleanroom" environment making 
      it possible to attempt alignment of generation + propagation 

* implemented CCerenkovGenerator + CGenstepSource : to allow 2nd executable Geant4 
  to run from gensteps by generating photons at primary level 
  (turning secondary photons from the 1st executable into primaries of the 2nd)

   * **notice this is turning gensteps into first class citizens**

* implemented CAlignEngine for simple switching between pre-cooked RNG streams 



2018 July : discussions with Geant4 members, Linux port, direct translation debug
--------------------------------------------------------------------------------------------------------------

* **discuss proposed extended optical example with Geant4 members**
* **port to Linux CentOS7 Workstation with Volta GPU (NVIDIA Titan V), OptiX 5.1.0, CUDA 9.2**
* **debugging direct geometry translation**

* port python tree balancing to C++ NTreeBalance  
* CHEP + JUNO meetings 
* movie making machinery 
* port the old python opticks-nnt codegen to C++ for the direct route, see x4gen-
  giving code generation of all solids in the geometry 
* refactoring analytic geometry code NCSG, splitting into NCSGData 
* NCSG level persisting 


2018 June : direct Geant4 to Opticks geometry conversion : **simplifies usage**
---------------------------------------------------------------------------------

* simplifies applying Opticks acceleration to any Geant4 geometry

* X4/ExtG4 package for direct conversion of in memory Geant4 model into Opticks GGeo
* YoctoGLRap YOG package for direct conversion from Geant4 into glTF 
* direct fully analytic conversions of G4VSolid into Opticks CSG nnode trees, 
* direct conversions of G4 polgonizations (triangle approximation) into Opticks GMesh 
* adopt integrated approach for analytic and approximate geometry, incorporating 
  both into GGeo rather than the former separate GScene approach 
* direct conversions of materials and surfaces

2018 May : adopt modern CMake target export/import : **simplifies configuration**
-----------------------------------------------------------------------------------

* greatly simplifies Opticks configuration internally and for users

* research modern CMake (3.5+) capabilities for target export/import, find BCM
* adopt Boost CMake Modules (BCM) http://bcm.readthedocs.io/  (proposed for Boost)
  to benefit from modern CMake without the boilerplate 
* much simpler CMakeLists.txt both inside Opticks and in the use of Opticks
  by user code, only need to be concerned with direct dependencies, the tree
  of sub-dependencies is configured  automatically 
* BCM wave over all ~100 CMakeLists.txt took ~10 days
* G4OK project for Geant4 based user code with embedded Opticks, via G4Opticks singleton
* simplify logging OPTICKS_LOG.hh 
* geometry digests to notice changed geometry 

2018 March ; Opticks updated ; macOS High Sierra 10.13.4, Xcode 9.3, CUDA 9.1, OptiX 5.0.1  
---------------------------------------------------------------------------------------------------

* get installation opational onto "new" machine, latest macOS ; High Sierra 10.13.4, Xcode 9.3 with CUDA 9.1 and OptiX 5.0.1


2017 Dec : aligned bi-simulation ~perfect match with simple geometry after fixes 
-----------------------------------------------------------------------------------

* **aligning RNG consumption of GPU/CPU simulations -> trivial validation** 
* **fix polarization + specular reflection discrepancies revealed by aligned running**

* investigate approaches allowing use of the same RNG sequence with Opticks and Geant4

  * near perfect (float precision level) matching with input photons (no reemission yet) 

* add diffuse emitters for testing all angle incidence
* rework specular reflection to match Geant4, fixing polarization discrepancy

2017 Nov ; improved test automation/depth, help LZ user installation 
------------------------------------------------------------------------

* work with LZ user, on AssimpImporter issue
* introduce "--reflectcheat" so photons can stay aligned thru BR/SR 
* direct point-by-point deviation comparisons, for use with common input photons, 
  photons stay aligned until meet RNG (eg from BR/SR/SC/AB)  
* introduce "--testauto" mode that dynamically changes surfaces (simplifying photon histories)
  allowing checks of intersect positions against SDFs without duplicating all the ~50 integration test 
  geometries 
* introduce G4 only universe wrapper volume, to reconcile the boundary-vs-volume 
  model difference between G4 and Opticks
* get bounce truncation to match between Opticks and CFG4, eg for hall-of-mirrors situation
* reimplement the cfg4/CRecorder monolith into many pieces including CG4Ctx for better clarity 
* translation of optical surfaces to Geant4 motivates a reworking of surface geometry
  representation, enhanced persisting simplifies processing and conversion to Geant4  

2017 Oct : emissive test geometry, CPU input photons, Opticks presented to Geant4 plenary
--------------------------------------------------------------------------------------------

* **Opticks presented to plenary session of Geant4 Collaboration Meeting**

* enable any CSG solid to emit test photons, generated CPU side such that 
  Opticks and Geant4 simulations are given exactly the same input photons
* pushed Opticks analytic geometry support thru to okg4, allowing Opticks test geometries to 
  be auto-converted to Geant4 ones ; for okg4 comparisons
* Opticks integration testing ; automate comparison of intersect positions with geometry SDF values 
* debugged Opticks installs on two new Linux distros, Axel desktop, Shandong headless GPU server 
* presenting Opticks to the plenary session of the Geant4 Collaboration Meeting in Australia

2017 Sept : embedded Opticks with Tao Lin, headless GPU server tools at SDU
--------------------------------------------------------------------------------------

* work on some techniques (ffmpeg, okop-snap) to use Opticks on headless GPU server machines, 
  such as combining pure compute raytrace geometry snapshots into mp4 movies
* work with Tao on Opticks/JUNO embedding 
* implement embedded mode of Opticks operation using okop/OpMgr to run  
  inside another process, such as JUNO offline
* introduce okop/OpMgr (pure compute Opticks manager) 
  and usage on headless GPU servers

Big Geometry
~~~~~~~~~~~~~~~

* Eureka ; avoiding having two InstLODCull active regains sanity, with this proviso frustum culling and LOD forking are both working
* InstLODCull simplifications from moving uniform handling to UBO in RContext


2017 Aug : primitives for JUNO : ellipsoid, torus, hyperboloid : solve-quartic troubles
---------------------------------------------------------------------------------------------

* Focus on tricky primitives

Overview
~~~~~~~~~~~

* implemented the primitives needed for JUNO ; torus was difficult, also 
  implemented hyperboloid  ; perhaps we can look into replacing torus with 
  hyperboloid for the PMT (it is much much easier computationally, just quadratics rather than quartics)

* moved analytic geometry processing pre-cache ; so launch time is 
  reduced from ~50 s to < 5 s

* improved OpenGL visualisation performance using 
  instance frustum culling and variable level-of-detail meshes for instances (=PMTs) based on 
  distance to the instance.  These techniques use GPU compute (OpenGL transform feedback) 
  prior to rendering each frame to skip instances that are not visible and replace distant instances with simpler
  geometry.   The improved performance will make it much easier to capture movies…

  As Macs only go to OpenGL 4.1 ; I am limited to techniques available to that version 
  which means no OpenGL compute shaders.  I could of use CUDA interop techniques but 
  if possible it is better to stick with OpenGL for visualisation as that  can work on AMD 
  (and perhaps even Intel) GPUs, meaning much more users can benefit from it.


Solids
~~~~~~~~~

* using doubles for quartic/cubic Solve now seems inevitable, issues are much reduced with doubles but not entirely fixed
* op --j1707 --gltf 3 ; fully analytic raytrace works, not having any triangles saves gobs of GPU memory ; investigate ways to skip torus intersects
* start on hyperbolic hyperboloid of one sheet, hope to model PMT neck with hyperboloid rather than subtracted torus
* torus artifacts gone, after move SolveCubicStrobachPolyFit to use initial gamma using SolveCubicPolyDivision instead of the cursed SolveCubicNumeric

Big Geometry
~~~~~~~~~~~~~~~

* investigate OpenGL LOD and Culling for coping with big geometry
* start checking whats needed to enable instance culling, over in  env- instcull-
* moving analytic GScene into geocache fixes j1707 slow startup, reducing from 50 secs to under 5 secs
* threading LODified meshes thru GGeoLib/GGeoTest
* prep for bringing dynamic GPU LOD fork+frustum culling like env- instcull- into oglrap-, plan to use first class citizen RBuf (of Renderer) to simplify the buffer juggling


2017 July : Solid level bbox Validations and fixes
----------------------------------------------------------------------------------------------------

Solids
~~~~~~~~~

* fix trapezoid misinterpretation (caused impingment) using new unplaced mesh dumping features added to both branches
* fixed cone-z misinterpretation
* added deltaphi imp via CSG_SEGMENT intersect, tboolean-cyslab tboolean-segment
* start on primitives needed for juno1707
* add zcut ellipsoid by using zsphere with scaling adjusted to be 1 for z
* investigate torus artifacts, by looking into cubic approach

Validation ; machinery for comparison G4DAE vs GDML/glTF geometries
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* generalize GMeshLib to work in analytic and non-analytic branches, regularize GNodeLib to follow same persistency/reldir pattern
* factor GMeshLib out of GGeo and add pre-placed base solid mesh persisting into/from geocache, see GMeshLibTest and --gmeshlib option
* get nnode_test_cpp.py codegen to work with nconvexpolyhedron primitives defined by planes and bbox

* impingement debug by comparison of GDML/glTF and G4DAE branches
* comparing GMesh bbox between branches, reveals lots of discrepancies ; GScene_compareMeshes.rst
* bbox comparisons are productive ; cone-z misinterp, missing tube deltaphi
* csg composite/prim bbox avoids polyfail noise reduces discrepant meshes to 12 percent
* moving to parsurf bbox, avoids overlarge analytic bbox with complicated CSG trees
* adopting adaptive parsurf_level to reach a parsurf_target number of surface points knocks 5 lvidx down the chart
* complete classification of top 25 parsurf vs g4poly bbox discrepancies, down to 1mm



2017 June : tapering poly dev, tree balancing, build out validation machinery, uncoincidence
----------------------------------------------------------------------------------------------------

Polygonization ; move on HY poly taking too long
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* try subdivide border tris approach to boolean mesh combination, tboolean-hyctrl
* decide to proceed regardless despite subdiv problems, forming a zippering approach

Solids ; analytic bbox combination, tree balancing positivize, ndisc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* nbbox::CombineCSG avoids the crazy large bbox
* CSG.subdepth to attempt tree balancing by rotation, swapping left right of UNION and INTERSECTIN nodes when that would improve balance
* honouring the complement in bbox and sdf, testing with tboolean-positivize 
* checking deep csg trees with tboolean-sc
* nbox::nudge finding coincident surfaces in CSG difference and nudging them to avoid the speckled ghost surface issues
* tboolean-uncoincide for debugging uncoincide failure 
* tboolean-esr ; investigate ESR speckles and pole artifacting, from degenerate cylinder
* add disc primitive tboolean-disc as degenerate cylinder replacement
* make CSG_DISC work as a CSG subobject in boolean expressions by adding otherside intersects and rigidly oriented normals
* mono bileaf CSG tree balancing to handle mixed deep trees, used for unions of cylinders with inners done via subtraction

Structure
~~~~~~~~~~~~

* completed transfer of node identity, boundary and sensor info, from triangulated G4DAE to analytic GDML/glTF branches in GScene
* moving to absolute tree handling in gltf with selection mask gets steering of the branches much closer

Validation ; intersect point SDF, SDF scanning, containment(child surf vs parent SDF)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* factor GNodeLib out of GGeo to avoid duplication between GScene and GGeo, aiming to allow comparison of triangulated and analytic node trees
* node names and order from full geometry traversals in analytic and triangulated branches are matching, see ana/nodelib.py
* analytic geometry shakedown begins
* prep automated intersect debug by passing OpticksEvent down from OpticksHub into GScene::debugNodeIntersects

* autoscan all CSG trees looking for internal SDF zeros
* tablulate zero crossing results for all trees, odd crossings almost all unions, no-crossing mostly subtraction
* NScanTest not outside issue fixed via minimum absolute cage delta, all the approx 10 odd crossings CSG trees are cy/cy or cy/co unions in need of uncoincidence nudges

* expand parametric surface coverage to most primitives, for object-object coincidence testing of bbox hinted coincidences
* nnode::getCompositePoints collecting points on composite CSG solid surface using nnode::selectBySDF on the parametric points of the primitives


* NScene::check_surf_points classifying node surface points against parent node SDF reveals many small coincidence/impingement issues 
* avoiding precision issues in node/parent collision (coincidence/impingement) by using parent frame does not make issue go away




2017 May : last primitive (trapezoid/convexpolyhedron), tree balancing, hybrid poly, scene structure
-------------------------------------------------------------------------------------------------------

Solids ; trapezoid, nconvexpolyhedron ; tree balancing
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* tboolean-trapezoid ; trapezoid, nconvexpolyhedron 
* nconvexpolyhedron referencing sets of planes just like transforms referencing
* icosahedron check 
* investigate 22 deep CSG solids with binary tree height greater than 3 in DYB near geometry
* implement complemented primitives ; thru the chain from python CSG into npy NCSG, NNode, NPart and on into oxrap csg_intersect_part
* Tubs with inner radius needs an inner nudge, making the inner subtracted cylinder slightly thicker than the outer one
* handling poles and seams in sphere parametrisation 

Polygonization ; hybrid implicit/parametric
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* start HY ; hybrid implicit/parametric polygonization
* parametric primitive meshing with NHybridMesher code HY, test with tboolean-hybrid
* try subdivide border tris approach to boolean mesh combinatio
* adopt centroid splitting succeeds to stay manifold 

Structure ; gltf transport
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* start on GPU scene conversion sc.py, gltf, NScene, GScene
* booting analytic gdml/gltf root from gdml snippets with tgltf-
* repeat candidate finding/using (ie instanced analytic and polygonized subtrees) in NScene/GScene
* integration with oyoctogl- ; for gltf parsing
* tgltf-gdml from oil maxdepth 3, now working with skipped overheight csg nodes (may 20th)



2017 Apr : faster IM poly, lots of primitives, bit twiddle postorder pushes height limit, start with GDML
----------------------------------------------------------------------------------------------------------

Polygonization
~~~~~~~~~~~~~~~~

* integrate implicit mesher IM over a couple of days - much faster than MC or DCS 
  as uses continuation approach and produces prettier meshes
* boot DCS out of Opticks into optional external 
* conclude polygonization fails for cathode and base are a limitation of current poly techniques, 
  need new approach to work with thin volumes, find candidate env-;csgparametric-

Solids ; lots of new primitives ncylinder, nzsphere, ncone, box3
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* start adding transform handling to the CSG tree
* add scaling transform support, debug normal transforms
* fix implicit assumption of normalized ray directions bug in sphere intersection 
* introduce python CSG geometry description into tboolean 
* implement ncylinder
* implement nzsphere
* implement ncone 
* implement CSG_BOX3
* polycones as unions of cones and cylinders
* start looking at CSG tree balancing

CSG Engine ; bit twiddle postorder
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* remove CSG tree height limitation by adoption of bit twiddling postorder, 
  benefiting from morton code experience gained whilst debugging DCS Octree construction

* attempts to use unbounded and open geometry as CSG sub-objects drives home 
  the theory behind CSG - S means SOLID, endcaps are not optional 

Structure ; jump ship to GDML
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* complete conversion of detdesc PMT into NCSG (no uncoincide yet)
* conclude topdown detdesc parse too painful, jump ship to GDML
* GDML parse turns out to be much easier
* implement GDML tree querying to select general subtrees 


2017 Mar : GPU CSG raytracing implementation, SDF modelling, MC and DCS polygonization of CSG trees 
-----------------------------------------------------------------------------------------------------

CSG Engine ; reiteration
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* moving CSG python prototype to CUDA
* reiteration, tree gymnastics
* CSG stacks in CUDA
* fix a real painful rare bug in tree reiteration  

Solids ; implicit modelling with SDFs
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* OpticksCSG unification of type shape codes
* learn geometry modelling with implicit functions, SDFs

Polygonization ; Marching Cubes, Dual Contouring
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* start adding polygonization of CSG trees using SDF isosurface extraction
* integrate marching cubes, MC
* integrate dual contouring sample DCS, detour into getting Octree operational in acceptably performant,
  painful at the time, by got real experience of z-order curves, multi-res and morton codes


2017 Feb : GPU CSG raytracing prototyping
-------------------------------------------

CSG Engine ; python prototyping, recursive into iterative
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

* prototyping GPU CSG in python
* Ulyanov iterative CSG paper pseudocode leads me astray
* GPU binary tree serialization
* adopt XRT boolean lookup tables
* learn how to migrate recursive into iterative


2017 Jan : PSROC presentation, CHEP proceedings
-------------------------------------------------

* CHEP meeting proceedings bulk of the writing  
* start looking at GPU CSG implementation
* PSROC presentation
* PHP


2016 Dec : g4gun, CSG research
----------------------------------

* Paris trip, review
* g4gun 
* CHEP proceedings 
* GPU CSG research 

2016 Nov : G4/Opticks optical physics chisq minimization
---------------------------------------------------------

* scatter debug
* groupvel debug 
* high volume histo chisq numpy comparisons machinery 

2016 Oct : G4/Opticks optical physics chisq minimization
-----------------------------------------------------------

* CHEP meeting 
* DYB optical physics including reemission teleported into cfg4
* CRecorder - for tracing the G4 propagations in Opticks photon record format 
* reemission continuation handling, so G4 recorded propagations can be directly compared to opticks ones
* step-by-step comparisons within the propagations
* tlaser testing 
* tconcentric chisq guided iteration 

2016 Sep : mostly G4/Opticks interop
----------------------------------------

* encapsulate Geant4 into CG4
* multievent handling rejig, looks to be mostly done in optixrap/OEvent.cc
* intro OKMgr and OKG4Mgr the slimmed down replacements for the old App
* Integrated Geant4/Opticks running allowing G4GUN steps to be directly Opticks GPU propagated
* OptiX buffer control worked out for multi-event running, using buffer control flags system  

2016 Aug : OpticksEvent handling, high level app restructure along lines of dependency
-----------------------------------------------------------------------------------------

* migration to OptiX 4.0.0 prompts adoption of buffer control system
* texture handling reworked for 400
* adopt cleaner OpticksEvent layout, with better containment
* add OpticksMode (interop,compute,cfg4) to persisted OpticksEvent metadata
* fix bizarre swarming photon visualization from noise in compressed buffer 
* adjust genstep handling to work with natural (mixed) Scintillation and Cerenkov gensteps
* start app simplification refactoring with low hanging fruit of splitting up classes along 
  lines of dependency - intro OpticksHub (beneath viz, hostside config,geometry,event) 
  and OpticksViz 

* With eye towards future support for fully integrated but layered(for dendency flexibility)
  Opticks/G4 running  

* take sledge hammer to the monolith App, pulling the pieces into separate classes, by dependency
* rework for simultaneous Opticks, G4 simulation - OpticksEvent pairs held in OpticksHub
* integration genstep handoff form G4 to Opticks

2016 Jul : porting to Windows and Linux, Linux interop debug
----------------------------------------------------------------

* migrate logging from boostlog to PLOG, as works better on windows - it also turns out to be better overall
* learning windows symbol export API approachs 
* succeed to get all non-CUDA/Thrust/OptiX packages to compile/run with windows VS2015
* migrate Opticks from env into new opticks repository, mercurial history manipulations
  allowed to bring over the relevant env history into opticks repo
* porting to Linux and multi-user environment in prep for SDU Summer school
* documenting Opticks and organizing the analysis scripts in prep for school
* inconclusive attempts to address Linux interop buffer overwrite issue

2016 Jun : porting to Windows
----------------------------------

* replacing GCache with OpticksResource for wider applicability 
* port externals to Windows/MSYS2/MINGW64
* move to using new repo opticksdata for sharing inputs  
* windows port stymied by g4 not supporting MSYS2/MINGW64  
* rejig to get glew, glfw, imgui, openmesh built and installed on windows with VS2015
* boost too

2016 May : CTests, CFG4 GDML handling, non-GPU photon indexing
------------------------------------------------------------------

* shifts
* getting more CTests to pass 
* bringing more packages into CMake superbuild
* add CGDMLDetector
* workaround lack of material MPT in vintage GDML, using G4DAE info 
* integrating with G4 using CG4 
* CPU Indexer and Sparse, for non-GPU node indexing
* rework event data handling into OpticksEvent

2016 Apr : build structure make to CMake superbuild, spawn Opticks repo
---------------------------------------------------------------------------

* GTC
* factoring usage of OptiX to provide functionality on non-CUDA/OptiX capable nodes
* CMake superbuild with CTests 
* external get/build/install scripts
* prep for spawning Opticks repository 

2016 Mar : Opticks/G4 PMT matching, GPU textures, making movie 
------------------------------------------------------------------

* resolved PMT skimmer BR BR vs BR BT issue - turned out to be Opticks TIR bug
* PmtInBox step-by-step record distribution chi2 comparison 
* rejig material/surface/boundary buffer layout to match OptiX tex2d float4 textures, with wavelength samples and float4 at the tip of the array serialization
* Dayabay presentation
* screen capture movie making 
* GTC presentation

2016 Feb : partitioned analytic geometry, compositing raytrace and rasterized viz
-----------------------------------------------------------------------------------

* create analytic geometry description of Dayabay PMT 
* PMTInBox debugging
* compositing OptiX raytrace with OpenGL rasterized


2016 Jan : Bookmarks, viewpoint animation, presentations
--------------------------------------------------------------------

* rework Bookmarks, split off state handling into NState
* add InterpolatedView for viewpoint animation 
* JUNO meeting presentation 
* PSROC meeting presentation 


2015 : First year of Opticks, based on NVIDIA OptiX
-----------------------------------------------------

**Year Executive Summary**

Develop Opticks based on the NVIDIA OptiX ray tracing framework, replacing Chroma.
Achieve match between Opticks and Geant4 for simple geometries with speedup 
factor of 200x with a mobile GPU. Performance factor expected to exceed 1000x 
with multi-GPU workstations.  

**Year Summary**

* realize lack of multi-GPU is showstopper for Chroma 
* find that NVIDIA OptiX ray tracing framework exposes accelerated geometry intersection 
* develop Opticks (~15 C++ packages: GGeo, AssimpWrap, OptiXRap, ThrustRap, OGLRap,...) 
  built around NVIDIA OptiX to replace Chroma : effectively 
  recreating part of the Geant4 context on the GPU 
* port Geant4 optical physics into Opticks
* achieve match between Opticks and Geant4 for simple geometries, 
  with speedup factor of 200x with laptop GPU with only 384 cores


2015 Dec : matching against theory for prism, rainbow, 200x performance with 384 cores
------------------------------------------------------------------------------------------

* prism test with Plankian light source using GPU texture
* rainbow comparisons against expectation : achieve Geant4/Opticks match with rainbow geometry
* cfg4, new package for comparison against standalone geant4
* cfg4 G4StepPoint recording - creating opticks format photon/step/history records with cfg4-
* Opticks/Geant4 rainbow scatter matching achieved
* enable loading of photons/records into ggv, in pricipal enables visualizing both Opticks and G4 cfg4- generated/propagated events on non-CUDA machines
* revive compute mode reveals 200x faster performance than Geant4 with only 384 CUDA cores 

2015 Nov : refactor for dynamic boundaries, Fresnel reflection matching, PMT uncoincidence
---------------------------------------------------------------------------------------------

* overhaul material/surface/boundary handling to allow dynamic boundary creation post geocache
  (ie geometry configurable from commandline)
* implement dynamic test geometry creation controlled by commandline argument, using "--test" option 
* npy analysis for Fresnel reflection testing
* adopt more rational PMT partitioning surfaces (not a direct translation)

2015 Oct : meshfixing, instanced identity, start analytic partitioning
--------------------------------------------------------------------------

* vertex deduping as standard  
* IAV and OAV mesh surgery
* sensor handling
* identity with instancing
* develop analytic PMT approach : via detdesc parsing and geometrical partitioning
* flexible boundary creation

2015 Sep : thrust for GPU resident photons, OpenMesh for meshfixing
--------------------------------------------------------------------

* use interop Thrust/CUDA/OptiX to make **photons fully GPU resident**, eliminating overheads
* finally(?) nail majority of CUDA/Thrust/OpenGL/OptiX interop issues
* add Torch for testing
* investigate bad material for upwards going photons, find cause is bad geometry
* uncover issue with DYB cleaved meshes, develop fix using OpenMesh

2015 Aug : big geometry handling with Instancing
--------------------------------------------------

* OptiX instancing 
* intro BBox standins
* Thrust interop

2015 Jul : photon index, propagation histories, Linux port
-----------------------------------------------------------

* photon indexing with Thrust
* verifying ThrustIndex by comparison against the much slower SequenceNPY
* auto-finding repeated geometry assemblies by progeny transform/mesh-index digests in GTreeCheck
* interim Linux compatibility working with Tao
* 4-GPU machine testing with Tao
* OpenGL/OptiX instancing 
* trying to get JUNO (big) geometry to work with instancing 
* computeTest timings for Juno Scintillation as vary CUDA core counts

2015 Jun : develop compressed photon record, learn Thrust 
------------------------------------------------------------

* Cerenkov and Scintillation generated photons match to Geant4 achieved within OptiX machinery
* implement Fresnel reflection/refraction with OptiX

* develop highly compressed photon records
* ViewNPY machinery for OpenGL uploading 
* get animation working 
* add GOpticalSurface, for transporting surface props thru Assimp/AssimpWrap into GGeo
* learning Thrust
* OptiX 3.8 , CUDA 7.0 update 


2015 May : GPU textures for materials, geocache, ImGui
---------------------------------------------------------

* bring NPY persistency to GGeo : introducing the geocache
* implement geocache loading to avoid XML parsing on every launch 
  (turned out to be a luxury for DayaBay [saving only a few seconds per launch], 
   but 6 months hence it is a necessity for JUNO [saving several minutes for every launch])
* GSubstanceLib infrastructure
* start bringing materials to GPU via textures
* material code translation in Lookup
* reemission handling, inverse CDF texture creation
* Cerenkov and Scintillation generated photons match to Geant4 achieved within OptiX machinery
* pick ImGui immediate mode GUI renderer
* GUI adoption by the oglrap classes
* prepare presentation 

  * Why not Chroma ? Progress report on migrating to OptiX 
  * http://simoncblyth.bitbucket.io/env/presentation/optical_photon_simulation_with_nvidia_optix.html

2015 April 
------------

* reuse NumpyServer infrastructure for UDP messaging allowing live reconfig of objects 
  with boost::program_option text parsing 
* add quaternion Trackball for interactive control
* avoid duplication with OptiXRap
* arrange OptiX output buffer to be a PBO which is rendered as texture by OpenGL
* create OpenGL visualization package: OGLRap (Prog/Shdr infrastructure) and OptiXEngine ray tracer
* OptiXEngine starting point for propagation, previously focussed on OptiX ray tracing 
* ported Cerenkov generation from Chroma to OptiX

2015 March 
-----------

* encounter OptiX/cuRAND resource issue, workaround using pure CUDA to initialize and persist state
* fail to find suitable C++ higher level OpenGL package, start own oglrap- on top of GLFW, GLEW
* integrate ZMQ messaging with NPY serialization using Boost.ASIO ASIO-ZMQ to create NumpyServer


2015 February 
----------------

* fork Assimp https://github.com/simoncblyth/assimp/commits/master
* benchmarks with using CUDA_VISIBLE_DEVICES to control how many K20m GPUs are used
* fork Assimp for Opticks geometry loading
* test OptiX scaling with IHEP GPU machine
* great GGeo package, intermediary geometry model
* experiment with GPU textures for interpolated material property access 

2015 January 
-------------

* https://bitbucket.org/simoncblyth/env/src/2373bb7245ca3c1b8fb06718d4add402805eab93/presentation/gpu_accelerated_geant4_simulation.txt?fileviewer=file-view-default
* https://simoncblyth.bitbucket.io/env/presentation/gpu_accelerated_geant4_simulation.html

  * G4 Geometry model implications 
  * G4DAE Geometry Exporter
  * G4DAEChroma bridge

* realize lack of multi-GPU support is showstopper for Chroma
* find NVIDIA OptiX, initial tests suggest drastically 50x faster than Chroma
* first look at OptiX immediately after making the above presentation
* fork Assimp for geometry loading into GGeo model
* succeed to strike geometry with Assimp and OptiX


2014 : Year of G4DAEChroma : Geant4 to Chroma runtime bridge
----------------------------------------------------------------

**Year Executive Summary**

Get G4DAE exported geometries into Chroma and integrate Geant4 
and Chroma event data via G4DAEChroma runtime bridge.  

**Year Summary**

* Get Chroma to operate with G4DAE exported geometries. 
* Develop G4DAEView visualization using CUDA/OpenGL interoperation techniques
  and OpenGL shaders for geometry and photon visualization.
* Develop G4DAEChroma runtime bridge interfacing Geant4 with external optical photon propagation.
* Realize that photon transport is too large an overhead, so implement GPU Scintillation/Cerenkov
  generation within Chroma based in transported gensteps

**December 2014**

* realize photon transport has too much overhead, "gensteps" are born 
* implement Cerenkov and Scintillation step transport and photon generation on GPU 

**October/November 2014**

* develop G4DAEChroma (photon transport over ZMQ): Geant4 to Chroma runtime bridge 

**September 2014**

* present G4DAE geometry exporter at: 19th Geant4 Collaboration Meeting, Okinawa, Sept 2014

**August 2014**

* export Daya Bay PMT identifiers
* develop non-graphical propagator

**June/July 2014**

* create GLSL shader visualizations of photon propagations 
* reemission debug 

**May 2014**

* develop ChromaZMQRoot approach to transporting photons from NuWa to Chroma 

**Mar-Apr 2014**

* forked Chroma, adding G4DAE integration and efficient interop buffers
* develop g4daeview geometry viewer (based on pyopengl, glumpy)  

**Jan-Feb 2014**

* December 16th 2013 : purchase Macbook Pro laptop GPU: NVIDIA GeForce GT 750M 
  (in Hong Kong while on trip for DayaBay shifts) 
* integrate G4DAE geometry with Chroma 


2013 Aug-Dec : Initial look, G4DAE geometry exporter 
-----------------------------------------------------

Develop G4DAE Geant4 exporter that liberates tesselated G4 geometries
into COLLADA DAE files, including all material and surface properties.

* study Geant4 and Chroma optical photon propagation
* develop C++ Geant4 geometry exporter : G4DAE 
* experiment with geometry visualizations (webgl, meshlab)

December 2013 (G4DAE visualization 2nd try: meshlab)
-------------------------------------------------------

* meshlab- hijacked for COLLADA viewing
* meshlab COLLADA import terribly slow, and meshlab code is a real mess 
* forked meshlab https://bitbucket.org/simoncblyth/meshlab
* investigate openscenegraph- colladadom- osg-
  (clearly decided meshlab far to messy to be a basis for anything)

November 2013 (G4DAE visualization 1st try: webgl)
----------------------------------------------------

* webgl threejs daeserver.py 

Status report coins G4DAE, were validating G4DAE against VRML2

* https://bitbucket.org/simoncblyth/env/src/9f0c188a8bb2042eb9ad58d95dadf9338e08c634/muon_simulation/nov2013/nov2013_gpu_nuwa.txt?fileviewer=file-view-default

Oct 2013 (G4DAE approach born)
--------------------------------

* translate Geant4 volume tree into COLLADA DAE
* webpy server of DAE subtrees

Sept 2013
----------

* sqlite3 based debugging of VRML exports 
* try reality player VRML viewer
* end Sept, start looking into GDML and COLLADA pycollada-
 
Although VRML was a dead end, it provided the G4Polyhedron 
triangulation approach used later in G4DAE.

Sep 24 2013
~~~~~~~~~~~~~

The only real progress so far is with the geometry aspect
where I have made Geant4 exports of VRML2 and GDML
versions of the Dayabay geometry and examined how those
exporters operate. From that experience, I think that
development of a Geant4 Collada exporter (a common 3D file format)
is the most convenient way to proceed in order to
extract the Chroma needed triangles+materials from Geant4.
For developing the new exporter, I need to learn the relevant
parts of the Collada format and can borrow much code
from the VRML2 and GDML exporters.

August 2013 (geometry exporter study)
---------------------------------------

* Geant4 Muon simulation profiling, fast-
* studing Geant4 and Geant4/Chroma integration
* looking into Geant4 exporters and visualization
* study meshlab-
* trying VRML exports
* try blender
* study Chroma operation

* https://bitbucket.org/simoncblyth/env/commits/e7cb3c9353775de29bade841b171f7a7682cbe9c


July 2013 (surveying landscape)
-----------------------------------

Looked into muon simulation optimization techniques

* photon weighting




Notes
----------

Early years copied here from okc-vi there is more detail over there than here.


