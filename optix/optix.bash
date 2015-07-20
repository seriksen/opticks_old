optix-src(){      echo optix/optix.bash ; }
optix-source(){   echo ${BASH_SOURCE:-$(env-home)/$(optix-src)} ; }
optix-vi(){       vi $(optix-source) ; }
optix-env(){      elocal- ; }
optix-usage(){ cat << EOU

NVIDIA OptiX Ray Trace Toolkit
================================== 

Resources
----------

* https://devtalk.nvidia.com/default/board/90/optix/

* http://on-demand-gtc.gputechconf.com/gtcnew/on-demand-gtc.php?searchByKeyword=Optix&searchItems=&sessionTopic=&sessionEvent=&sessionYear=&sessionFormat=&submit=&select=+

  Many presentations (videos and pdfs) on OptiX

* http://docs.nvidia.com/gameworks/index.html#gameworkslibrary/optix/optix_programming_guide.htm

OptiX Versions
-----------------

::

    3.5.1         02/21/2014 05:12:40    
    3.6.0         06/11/2014 13:32:05
    3.6.3         09/27/2014 18:52:56    CUDA R331 driver or later 
    3.7.0-beta2                                                   
    3.7.0-beta3   01/30/2015 17:35:59    CUDA R343 driver
    3.7.0         02/24/2015 22:53:11
    3.8.0-beta    03/14/2015 17:04:51
    3.8.0         06/01/2015 07:18:19    CUDA R346 driver or later, for Mac the driver extension module supplied with CUDA 7.0 will need to be installed. 


OptiX Download and Unpack 
-----------------------------

Download and mount dmg::

    optix-ftp    # web interface, click on .dmg 

    mv ~/Downloads/NVIDIA-OptiX-SDK-3.8.0-mac64.dmg $(optix-download-dir)/

    open NVIDIA-OptiX-SDK-3.8.0-mac64.dmg   # mounts volume containing NVIDIA-OptiX-SDK-3.8.0-mac64.pkg 

Examine pkg contents using lsbom, verify that target is all beneath /Developer/OptiX::

    simon:~ blyth$ lsbom /Volumes/NVIDIA-OptiX-SDK-3.8.0-mac64/NVIDIA-OptiX-SDK-3.8.0-mac64.pkg/Contents/Archive.bom
    .   40755   501/0
    ./Developer 40755   501/0
    ./Developer/OptiX   40755   0/80
    ./Developer/OptiX/SDK   40755   0/80
    ./Developer/OptiX/SDK/CMake 40755   0/80
    ./Developer/OptiX/SDK/CMake/CompilerInfo.cmake  100644  0/80    3392    2636181668
    ./Developer/OptiX/SDK/CMake/ConfigCompilerFlags.cmake   100644  0/80    15064   2806959999
    ./Developer/OptiX/SDK/CMake/CopyDLL.cmake   100644  0/80    2850    1542474852
    ./Developer/OptiX/SDK/CMake/FindCUDA    40755   0/80
    ./Developer/OptiX/SDK/CMake/FindCUDA/make2cmake.cmake   100644  0/80    3532    494331911
    ./Developer/OptiX/SDK/CMake/FindCUDA/parse_cubin.cmake  100644  0/80    3666    547407452
    ./Developer/OptiX/SDK/CMake/FindCUDA/run_nvcc.cmake 100644  0/80    13360   3696680251
    ./Developer/OptiX/SDK/CMake/FindCUDA.cmake  100644  0/80    74607   1732037123
    ./Developer/OptiX/SDK/CMake/FindDX.cmake    100644  0/80    1414    2129523030
    ./Developer/OptiX/SDK/CMake/FindOptiX.cmake 100644  0/80    6408    1762934238
    ./Developer/OptiX/SDK/CMake/FindSUtilGLUT.cmake 100644  0/80    2747    312119594 
    ...

Prior to unpacking delete my symbolic link::

    simon:~ blyth$ l /Developer/
    total 8
    drwxr-xr-x  4 root  wheel  136 Jun 29 17:05 NVIDIA
    lrwxr-xr-x  1 root  wheel   11 Feb  2 12:53 OptiX -> OptiX_370b2
    drwxr-xr-x  7 root  admin  238 Jan 22 16:17 OptiX_301
    drwxr-xr-x  7 root  admin  238 Dec 18  2014 OptiX_370b2

    simon:~ blyth$ sudo rm /Developer/OptiX

Open .pkg from mounted .dmg, run the GUI installer, 
then rename and symbolicate::

    simon:Developer blyth$ sudo mv OptiX OptiX_380
    simon:Developer blyth$ sudo ln -s OptiX_380 OptiX 
    simon:Developer blyth$ l
    total 8
    lrwxr-xr-x  1 root  wheel    9 Jun 29 20:48 OptiX -> OptiX_380
    drwxr-xr-x  4 root  wheel  136 Jun 29 17:05 NVIDIA
    drwxr-xr-x  7 root  admin  238 May 29 00:23 OptiX_380
    drwxr-xr-x  7 root  admin  238 Jan 22 16:17 OptiX_301
    drwxr-xr-x  7 root  admin  238 Dec 18  2014 OptiX_370b2


Try Precompiled Samples
--------------------------

::

    cd /Developer/OptiX/SDK-precompiled-samples

    simon:SDK-precompiled-samples blyth$ open sample1.app
    simon:SDK-precompiled-samples blyth$ open sample2.app
    simon:SDK-precompiled-samples blyth$ open cook.app
    simon:SDK-precompiled-samples blyth$ open path_tracer.app
    simon:SDK-precompiled-samples blyth$ open instancing.app
    simon:SDK-precompiled-samples blyth$ open tutorial.app
    simon:SDK-precompiled-samples blyth$ open sphereTessellate.app
    simon:SDK-precompiled-samples blyth$ open whitted.app


Test can compile samples
--------------------------

::

    simon:SDK-precompiled-samples blyth$ optix-samples-get-all
    optix-samples-get-all copy all samples to somewhere writable
    simon:SDK-precompiled-samples blyth$ 



OptiX 3.8,  05/30/2015
------------------------

https://devtalk.nvidia.com/default/topic/836902/optix/optix-3-8-final-release-is-out-/

What is this Mac Driver Extension Module ? 
--------------------------------------------

Presumably CUDA.kext::

    simon:cuda blyth$ file  /System/Library/Extensions/CUDA.kext/Contents/MacOS/CUDA 
    /System/Library/Extensions/CUDA.kext/Contents/MacOS/CUDA: Mach-O universal binary with 1 architecture
    /System/Library/Extensions/CUDA.kext/Contents/MacOS/CUDA (for architecture x86_64): Mach-O 64-bit kext bundle x86_64

The CUDA uninstalll does::

    kextunload /System/Library/Extensions/CUDA.kext

::

    simon:cuda blyth$ kextstat | head -1 && kextstat | grep nvidia
    Index Refs Address            Size       Wired      Name (Version) <Linked Against>
      107    2 0xffffff7f80c52000 0x274000   0x274000   com.apple.nvidia.driver.NVDAResman (8.2.6) <83 74 71 11 5 4 3 1>
      108    0 0xffffff7f80ed1000 0x1ad000   0x1ad000   com.apple.nvidia.driver.NVDAGK100Hal (8.2.6) <107 11 4 3>
      127    0 0xffffff7f81dbe000 0x2000     0x2000     com.nvidia.CUDA (1.1.0) <4 1>


Seems no Mac equivalent of the R346 R355 R343 ...
---------------------------------------------------

Kexts have version numbers (above 1.1.0), 
but they are not referred to, instead just get the text::

    For the Mac, the driver extension module supplied with CUDA 7.0 
    will need to be installed.


Release Notes OptiX 3.8.0 (May 2015)
------------------------------------------

The CUDA R346 or later driver is required. For the Mac, 
the driver extension module supplied with CUDA 7.0 will need to be installed.

Driver R355 or newer is required for optimal performance on Maxwell GPUs. 
OptiX 3.8 supports Maxwell GPUs on earlier drivers, but at up to 20% lower performance.

A CUDA compiler bug that causes timeouts or crashes on Maxwell cards has been
worked around. Unfortunately, this bug fix causes a slowdown of up to 20% on
Maxwell cards unless driver R355 or newer is used.

CMake 2.8.12 http://www.cmake.org/cmake/resources/software.html


Release Notes OptiX Version 3.7 beta 3 (January 2015)
--------------------------------------------------------

The CUDA R343 or later driver is required. The latest available WHQL drivers
are highly recommended. For the Mac, the driver extension module supplied with
CUDA 5.0 or later will need to be installed. Note that the Linux and Mac
drivers can only be obtained from the CUDA 6.5 download page at the moment.

SLI is not required for OptiX to use multiple GPUs, and it interferes when
OptiX uses either D3D or OpenGL interop. Disabling SLI will not degrade OptiX
performance and will provide a more stable environment for OptiX applications
to run. SLI is termed "Multi-GPU mode" in recent NVIDIA Control Panels, with
the correct option being "Disable multi-GPU mode" to ensure OptiX is not
encumbered by graphics overhead.

Release Notes OptiX Version 3.6.3 (September 2014)
----------------------------------------------------

The CUDA R331 or later driver is required. The latest available WHQL drivers
are highly recommended (343 or later for Windows, 343 for Linux and the CUDA
6.0 driver extension for Mac). For the Mac, the driver extension module
supplied with CUDA 5.0 or later will need to be installed. Note that the Linux
and Mac drivers can only be obtained from the CUDA 6.0 download page at the
moment.

Release Notes OptiX Version 3.5 (January 2013)
----------------------------------------------------

The CUDA R319 or later driver is required. The latest available WHQL drivers
are highly recommended (320.92 or later for Windows, 319.60 for Linux and the
CUDA 5.5 driver extension for Mac). For the Mac, the driver extension module
supplied with CUDA 5.0 or later will need to be installed. Note that the Linux
and Mac drivers can only be obtained from the CUDA 5.5 download page at the
moment.


Update OptiX version and build samples
---------------------------------------

::

    -bash-4.1$ optix-linux-jump 370         # modify the symbolic link
    OptiX -> NVIDIA-OptiX-SDK-3.7.0-linux64

    -bash-4.1$ optix-name
    NVIDIA-OptiX-SDK-3.7.0-linux64     

    -bash-4.1$ optix-samples-get-all   ## copy samples, to avoid touching originals

    -bash-4.1$ optix-samples-cmake     ## fails due to cmake version 

    -bash-4.1$ optix-samples-cmake-kludge   ## kludge the requirement, seems to work with 2.6.4
    cmake_minimum_required(VERSION 2.8.8 FATAL_ERROR)
    cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

    -bash-4.1$ optix-samples-cmake     ## now completes
    -bash-4.1$ optix-samples-make      ## 


Determine Driver Version on Linux (on Mac use SysPref panel)
----------------------------------------------------------------

With nvidia-smi or::

    -bash-4.1$ cat /proc/driver/nvidia/version   # original old driver
    NVRM version: NVIDIA UNIX x86_64 Kernel Module  319.37  Wed Jul  3 17:08:50 PDT 2013
    GCC version:  gcc version 4.4.7 20120313 (Red Hat 4.4.7-4) (GCC) 

    -bash-4.1$ cat /proc/driver/nvidia/version   # updated Feb ~7 2015 
    NVRM version: NVIDIA UNIX x86_64 Kernel Module  340.65  Tue Dec  2 09:50:34 PST 2014
    GCC version:  gcc version 4.4.7 20120313 (Red Hat 4.4.7-4) (GCC) 
    -bash-4.1$ 

hgpu01 install
--------------

Manual CUDA::

    -bash-4.1$ cat /etc/redhat-release 
    Scientific Linux release 6.5 (Carbon)

    -bash-4.1$ rpm -qf /usr/local/cuda-5.5
    file /usr/local/cuda-5.5 is not owned by any package

Hmm getting "OptiX Error: Invalid context", with 370b3 
I was getting this on laptop until I updated the driver.

::

    scp /Users/blyth/Downloads/NVIDIA-OptiX-SDK-3.5.1-linux64.run L6:/dyb/dybd07/user/blyth/hgpu01.ihep.ac.cn/
    scp /Users/blyth/Downloads/NVIDIA-OptiX-SDK-3.6.3-linux64.run L6:/dyb/dybd07/user/blyth/hgpu01.ihep.ac.cn/
    scp /Users/blyth/Downloads/NVIDIA-OptiX-SDK-3.7.0-linux64.sh L6:/dyb/dybd07/user/blyth/hgpu01.ihep.ac.cn/

    -bash-4.1$ chmod ugo+x NVIDIA-OptiX-SDK-3.5.1-linux64.run 
    -bash-4.1$ chmod ugo+x NVIDIA-OptiX-SDK-3.6.3-linux64.run 
    -bash-4.1$ chmod ugo+x NVIDIA-OptiX-SDK-3.7.0-linux64.sh 

    -bash-4.1$ ./NVIDIA-OptiX-SDK-3.5.1-linux64.run --prefix=. --include-subdir
    -bash-4.1$ ./NVIDIA-OptiX-SDK-3.6.3-linux64.run --prefix=. --include-subdir
    -bash-4.1$ ./NVIDIA-OptiX-SDK-3.7.0-linux64.sh --prefix=. --include-subdir

    -bash-4.1$ ln -sfnv NVIDIA-OptiX-SDK-3.5.1-PRO-linux64 OptiX
    -bash-4.1$ ln -sfnv NVIDIA-OptiX-SDK-3.6.3-linux64 OptiX
    -bash-4.1$ ln -sfnv NVIDIA-OptiX-SDK-3.7.0-linux64 OptiX

    -bash-4.1$ pwd
    /dyb/dybd07/user/blyth/hgpu01.ihep.ac.cn/OptiX/SDK-precompiled-samples

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample3
    OptiX 3.7.0
    Number of Devices = 2
    ...
    Constructing a context...
    OptiX Error: Invalid context
    (/root/sw/wsapps/raytracing/rtsdk/rel3.7/samples/sample3/sample3.c:96)

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample3
    OptiX 3.6.3
    Number of Devices = 2
    ...
    Constructing a context...
      Created with 2 device(s)
      Supports 2147483647 simultaneous textures
      Free memory:
        Device 0: 4952547328 bytes
        Device 1: 4952547328 bytes

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample3
    OptiX 3.5.1
    Number of Devices = 2
    ...
    Constructing a context...
      Created with 2 device(s)
      Supports 2147483647 simultaneous textures
      Free memory:
        Device 0: 4952547328 bytes
        Device 1: 4952547328 bytes

::

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample7 -f sample7.ppm
    OptiX Error: Invalid context (Details: Function "RTresult _rtContextCompile(RTcontext)" caught exception: Unable to set the CUDA device., [3735714])
    -bash-4.1$ 


Partial override warning
---------------------------

::

    /Developer/OptiX/include/optixu/optixpp_namespace.h(588): warning: overloaded virtual function "optix::APIObj::checkError" is only partially overridden in class "optix::ContextObj"

* http://stackoverflow.com/questions/21462908/warning-overloaded-virtual-function-baseprocess-is-only-partially-overridde


OptiX Transform (Programming Guide)
----------------------------------------

A transform node is used to represent a projective transformation of its
underlying scene geometry. The transform must be assigned exactly one child of
type rtGroup, rtGeometryGroup, rtTransform, or rtSelector, using
rtTransformSetChild. That is, the nodes below a transform may simply be
geometry in the form of a geometry group, or a whole new subgraph of the scene.

The transformation itself is specified by passing a 4×4 floating point matrix
(specified as a 16-element one-dimensional array) to rtTransformSetMatrix.
Conceptually, it can be seen as if the matrix were applied to all the
underlying geometry. However, the effect is instead achieved by transforming
the rays themselves during traversal. This means that **OptiX does not rebuild
any acceleration structures when the transform changes**.

Note that the transform child node may be shared with other graph nodes. That
is, a child node of a transform may be a child of another node at the same
time. This is often useful for instancing geometry.


::

    delta:bin blyth$ optix-;optix-samples-cppfind Transform -l
    /usr/local/env/cuda/OptiX_380_sdk/glass/glass.cpp
    /usr/local/env/cuda/OptiX_380_sdk/hybridShadows/hybridShadows.cpp
    /usr/local/env/cuda/OptiX_380_sdk/instancing/instancing.cpp
    /usr/local/env/cuda/OptiX_380_sdk/isgReflections/isgReflections.cpp
    /usr/local/env/cuda/OptiX_380_sdk/isgShadows/isgShadows.cpp
    /usr/local/env/cuda/OptiX_380_sdk/primeInstancing/primeInstancing.cpp
    /usr/local/env/cuda/OptiX_380_sdk/sample7/sample7.cpp
    /usr/local/env/cuda/OptiX_380_sdk/sutil/OptiXMesh.cpp
    /usr/local/env/cuda/OptiX_380_sdk/sutil/OptiXMeshImpl.cpp
    /usr/local/env/cuda/OptiX_380_sdk/swimmingShark/fishMonger.cpp



Usage
~~~~~~~~

* wrapping moving pieces of geometry into Transforms allows position
  to be changed without rebuilding acceleration structures 


Thoughts on applying *Transform* instancing to complex/large geometries
-----------------------------------------------------------------------------

* Current geocache machinery is flat using final transforms applied to every volume.
  This works fine when treating everything as triangles and yields a very simple
  *convertDrawable*  

  * optix::Geometry GMergedMeshOptiXGeometry::convertDrawable(GMergedMesh* mergedmesh)

* Attempting to operate as if every solid making up the PMT is independant 
  each with a global transform (as the flat geocache encourages) 
  would yield an unnecessarily complicated OptiX geometry of overlapping transforms 

  * this would likely not work and even if it did would be fragile and difficult to 
    move, consider for example a analogous treatment of movable calibration sources 

* PMTs (and calibration sources) are not simple single volumes, 
  they are a collection of volumes arranged via transforms wrt to each other 
  with the assembly placed in the wider detector via a "placement" transform

* can sub-assemblies be auto-identified by finding repeated transform/meshIndex sub-trees ?
 
  * construct transform/meshIndex digests for the tree beneath every node
  * look for repeated such digests and locate the parent placement transforms 

* need to create local assembly frame vertices 


How to persist tree of transforms in the geocache ?
-----------------------------------------------------


OptiX Instancing : 20k teapots
---------------------------------

::

    optix-;optix-samples-cd bin
    ./instancing -i 20000 -n 
    ./instancing -i 20000 --grid=100x100x100



OptiX Selector (Programming Guide)
-------------------------------------

A selector is similar to a group in that it is a collection of higher level
graph nodes. The number of nodes in the collection is set by
rtSelectorSetChildCount, and the individual children are assigned with
rtSelectorSetChild. Valid child types are rtGroup, rtGeometryGroup,
rtTransform, and rtSelector.

The main difference between selectors and groups is that selectors do not have
an acceleration structure associated with them. Instead, a visit program is
specified with rtSelectorSetVisitProgram. This program is executed every time a
ray encounters the selector node during graph traversal. The program specifies
which children the ray should continue traversal through by calling
*rtIntersectChild*.

A typical use case for a selector is dynamic (i.e. per-ray) level of detail: an
object in the scene may be represented by a number of geometry nodes, each
containing a different level of detail version of the object. The geometry
groups containing these different representations can be assigned as children
of a selector. The visit program can select which child to intersect using any
criterion (e.g. based on the footprint or length of the current ray), and
ignore the others.

As for groups and other graph nodes, child nodes of a selector can be shared
with other graph nodes to allow flexible instancing.


Selector Examples
~~~~~~~~~~~~~~~~~~

::

    delta:ggeo blyth$ optix-;optix-samples-cufind IntersectChild
    /usr/local/env/cuda/OptiX_380_sdk/device_exceptions/device_exceptions.cu:    rtIntersectChild( 1 );
    /usr/local/env/cuda/OptiX_380_sdk/device_exceptions/device_exceptions.cu:    rtIntersectChild( 0 );
    /usr/local/env/cuda/OptiX_380_sdk/sample8/selector_example.cu:  rtIntersectChild( index );
    delta:ggeo blyth$ 

    optix-;optix-samples-cd bin

    delta:bin blyth$ ./sample8 -h


OptiX glass
------------

* https://devtalk.nvidia.com/default/topic/458979/?comment=3263252

Overlapping geometry problem


OptiX 3.6.3 problems
----------------------


* https://devtalk.nvidia.com/default/topic/763478/simplest-optix-code-unable-to-set-cuda-device/

::

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample7 -f sample7.ppm
    OptiX Error: Invalid context (Details: Function "RTresult _rtContextCompile(RTcontext)" caught exception: Unable to set the CUDA device., [3735714])
    -bash-4.1$ 
 

OptiX_370b2 rtPrintf bizarre bug
----------------------------------

Whilst debugging wavelength texture lookups with
a program which is only invoked for the single touched pixel 
under the mouse::

     33 RT_PROGRAM void closest_hit_touch()
     34 {
     35   prd_touch.result = contrast_color ;
     36   prd_touch.node = node_index ; 
     37   
     38   prd_touch.texlookup_b = wlookup( NM_BLUE  , 0.5f ) ;
     39   prd_touch.texlookup_g = wlookup( NM_GREEN , 0.5f ) ;
     40   prd_touch.texlookup_r = wlookup( NM_RED   , 0.5f ) ;
     41   
     42   for(int i=-5 ; i < 45 ; i++ )
     43   { 
     44      float wl = wavelength_domain.x + wavelength_domain.z*i ;
     45      float4 lookup = wlookup( wl, 0.5f ); 
     46      rtPrintf("material1.cu::closest_hit_touch node %d   i %2d wl %10.3f   lookup  %10.3f %10.3f %10.3f %10.3f \n",
     47         node_index,
     48         i,
     49         wl,
     50         lookup.x,
     51         lookup.y,
     52         lookup.z,
     53         lookup.w);
     54   }     
     55 }

Get the expected output, BUT on splitting the rtPrintf 
into two calls get **Unknown error**.::

    OptiX Error: Unknown error (Details: Function "RTresult _rtContextCompile(RTcontext)" caught exception: Assertion failed: "Traits::getNext(m_cur) != 0", [7143516]) 

Unfortunately the only way to discover the source of
the problem is by "binary search" trial and error. Moral:

* never make largescale changes to optix programs without testing, 
  always test after making small focussed changes 

* limit use rtPrintf to only one per program ?

* dont leave rtPrintf lying around, just use one at a time whilst debugging
  and then comment them out



Transparency/blending
-----------------------

* https://developer.nvidia.com/content/transparency-or-translucency-rendering

* http://casual-effects.blogspot.tw/2014/03/weighted-blended-order-independent.html



OptiX rtDeclareVariable attribute variables
---------------------------------------------


From optix-pdf section *4.1.4 Attribute Variables* p35

Attribute variables provide a mechanism for communicating data between the
intersection program and the shading programs (e.g., surface normal, texture
coordinates). Attribute variables may only be written in an intersection
program between calls to rtPotentialIntersection and rtReportIntersection.


Do I need multiple OptiX materials ?
--------------------------------------

Aiming for different materials to just corresponds to 
different substance indices used to lookup into a single texture, 
as such no need for separate OptiX materials ? So just need::

    rtReportIntersection(0)

Only Bialkali needs some different behaviour to do PMT id lookups, 
so that could benefit from being a different material.

In mesh_intersect can communicate the primIdx via attribute 
in order to do the substance lookup in closestHit 
Remember mesh_intersect gets called the most, so avoid
doing anything in there that can be done elsewhere.





OptiX and atomics
-------------------

* https://devtalk.nvidia.com/default/topic/522795/optix/atomic-buffer-operations/

  see zoneplate sample


  One thing to keep in mind is that atomic operations will not work in multi-GPU
  situations unless you specify RT_BUFFER_GPU_LOCAL. In that case the data stays
  resident on the device and can only be read by the device that wrote it.


OptiX and curand ?
-------------------

* :google:`optix curand`

* https://devtalk.nvidia.com/search/more/sitecommentsearch/curand%20optix/
* https://devtalk.nvidia.com/default/topic/759883/random-number-streams/?offset=1
* https://devtalk.nvidia.com/default/topic/770325/curand_init-within-optix/

Suggest that it can be made to work

Chroma curand
~~~~~~~~~~~~~~

chroma/chroma/cuda/random.h::

    001 #ifndef __RANDOM_H__
      2 #define __RANDOM_H__
      3 
      4 #include <curand_kernel.h>
      5 
      6 #include "physical_constants.h"
      7 #include "interpolate.h"
      8 
      9 __device__ float
     10 uniform(curandState *s, const float &low, const float &high)
     11 {
     12     return low + curand_uniform(s)*(high-low);
     13 }
    ///   all the random funcs have curandState* s argument 
    ...
    135 __global__ void
    136 init_rng(int nthreads, curandState *s, unsigned long long seed,
    137      unsigned long long offset)
    138 {
    139     int id = blockIdx.x*blockDim.x + threadIdx.x;
    140 
    141     if (id >= nthreads)
    142     return;
    143 
    144     curand_init(seed, id, offset, &s[id]);
    145 }


chroma/chroma/cuda/propagate_hit.cu::

    128 __global__ void
    129 propagate_hit(
    ...
    134       curandState *rng_states,
    ...   
    ...  
    164     int id = blockIdx.x*blockDim.x + threadIdx.x;
    165 
    166     if (id >= nthreads)
    167     return;
    168 
    169     g = &sg;
    170 
    171     curandState rng = rng_states[id];
    ...
    208             generate_cerenkov_photon(p, cs, rng );


chroma/chroma/gpu/tools.py::

    107 init_rng_src = """
    108 #include <curand_kernel.h>
    109 
    110 extern "C"
    111 {
    112 
    113 __global__ void init_rng(int nthreads, curandState *s, unsigned long long seed, unsigned long long offset)
    114 {
    115     int id = blockIdx.x*blockDim.x + threadIdx.x;
    116 
    117     if (id >= nthreads)
    118         return;
    119 
    120     curand_init(seed, id, offset, &s[id]);
    121 }
    122 
    123 } // extern "C"
    124 """
    125 
    126 def get_rng_states(size, seed=1):
    127     "Return `size` number of CUDA random number generator states."
    128     rng_states = cuda.mem_alloc(size*characterize.sizeof('curandStateXORWOW', '#include <curand_kernel.h>'))
    129 
    130     module = pycuda.compiler.SourceModule(init_rng_src, no_extern_c=True)
    131     init_rng = module.get_function('init_rng')
    132 
    133     init_rng(np.int32(size), rng_states, np.uint64(seed), np.uint64(0), block=(64,1,1), grid=(size//64+1,1))
    134 
    135     return rng_states



optix exception codes
-----------------------

/Developer/OptiX/include/internal/optix_declarations.h::

    197 typedef enum
    198 {
    199   RT_EXCEPTION_PROGRAM_ID_INVALID           = 0x3EE,    /*!< Program ID not valid       */
    200   RT_EXCEPTION_TEXTURE_ID_INVALID           = 0x3EF,    /*!< Texture ID not valid       */
    201   RT_EXCEPTION_BUFFER_ID_INVALID            = 0x3FA,    /*!< Buffer ID not valid        */
    202   RT_EXCEPTION_INDEX_OUT_OF_BOUNDS          = 0x3FB,    /*!< Index out of bounds        */
    203   RT_EXCEPTION_STACK_OVERFLOW               = 0x3FC,    /*!< Stack overflow             */
    204   RT_EXCEPTION_BUFFER_INDEX_OUT_OF_BOUNDS   = 0x3FD,    /*!< Buffer index out of bounds */
    205   RT_EXCEPTION_INVALID_RAY                  = 0x3FE,    /*!< Invalid ray                */
    206   RT_EXCEPTION_INTERNAL_ERROR               = 0x3FF,    /*!< Internal error             */
    207   RT_EXCEPTION_USER                         = 0x400,    /*!< User exception             */
    208 
    209   RT_EXCEPTION_ALL                          = 0x7FFFFFFF  /*!< All exceptions        */
    210 } RTexception;


testing optix with curand
---------------------------

When attempting to use curand subsequences getting RT_EXCEPTION_STACK_OVERFLOW::

    Caught exception 0x3FC at launch index (144,0)
    Caught exception 0x3FC at launch index (0,0)
    Caught exception 0x3FC at launch index (96,0)

with the MeshViewer original setting of stack size::

    m_context->setStackSize( 1180 );

Winding up the stack size to 10000 succeeds to run, but unusably slowly.

things to try
~~~~~~~~~~~~~~

* hmm, could subsequent optix launches reuse a buffer initialized on the
  first launch ?  to enable a single initializing launch 
* hmm, but optix is unusable with the large stacksizes needed 
  for curand_init with subsequences and probably changing stack size 
  will invalidate the context ?
* what about using plain CUDA kernel call to 
  do curand_init and prepare the curandState buffer for interop
  with subsequent optix launches 



OptiX and CUDA interop
------------------------

Doc says, if the application creates a CUDA context before OptiX, 
the applicaton should make sure to use the below
to ensure subsequent maximum performance from OptiX.::

    cudaSetDeviceFlags(cudaDeviceMapHost | cudaDeviceLmemResizeToMax);


::

    delta:OptiX_370b2_sdk blyth$ find . -name '*.cpp' -exec grep -H cudaSet {} \;
    ./ocean/ocean.cpp:    cudaSetDevice(m_cuda_device);
    ./ocean/ocean.cpp:  cudaSetDevice( m_cuda_device );
    ./simplePrime/simplePrimeCommon.cpp:    CHK_CUDA( cudaSetDevice(i) );
    ./simplePrimeInstancing/simplePrimeCommon.cpp:    CHK_CUDA( cudaSetDevice(i) );
    ./simplePrimeMasking/simplePrimeCommon.cpp:    CHK_CUDA( cudaSetDevice(i) );
    ./simplePrimepp/simplePrimeCommon.cpp:    CHK_CUDA( cudaSetDevice(i) );
    ./simplePrimeppMultiBuffering/simplePrimeCommon.cpp:    CHK_CUDA( cudaSetDevice(i) );
    ./simplePrimeppMultiGpu/simplePrimeCommon.cpp:    CHK_CUDA( cudaSetDevice(i) );
    ./simplePrimeppMultiGpu/simplePrimeppMultiGpu.cpp:      CHK_CUDA( cudaSetDevice(int(i)) );
    ./simplePrimeppMultiGpu/simplePrimeppMultiGpu.cpp:      CHK_CUDA( cudaSetDevice(int(i)) );
    ./simplePrimeppMultiGpu/simplePrimeppMultiGpu.cpp:      CHK_CUDA( cudaSetDevice(int(i)) );
    ./simplePrimeppMultiGpu/simplePrimeppMultiGpu.cpp:      CHK_CUDA( cudaSetDevice(int(i)) );
    delta:OptiX_370b2_sdk blyth$ 
    delta:OptiX_370b2_sdk blyth$ 
    delta:OptiX_370b2_sdk blyth$ 
    delta:OptiX_370b2_sdk blyth$ pwd
    /usr/local/env/cuda/OptiX_370b2_sdk



/usr/local/env/cuda/OptiX_370b2_sdk/ocean/ocean.cpp::

    340     //
    341     // Setup cufft state
    342     //
    343 
    344     const unsigned int fft_input_size  = FFT_WIDTH * FFT_HEIGHT * sizeof(float2);
    345 
    346     m_context->launch( 0, 0 );
    ///
    ///     presumably ensures OptiX is first to setup CUDA context 
    ///
    347 
    348     m_cuda_device = OptiXDeviceToCUDADevice( m_context, 0 );
    ///
    ///     helper method: OptiX ordinal 0 -> CUDA ordinal 
    ///    
    349 
    350     if ( m_cuda_device < 0 ) {
    351       std::cerr << "OptiX device 0 must be a valid CUDA device number.\n";
    352       exit(1);
    353     }
    354 
    355     // output the CUFFT results directly into Optix buffer
    356     cudaSetDevice(m_cuda_device);
    357 
    358     cutilSafeCall( cudaMalloc( reinterpret_cast<void**>( &m_d_h0 ), fft_input_size ) );
    ///
    ///     plain CUDA allocation of space on device 
    ///
    359 
    360     m_h_h0      = new float2[FFT_WIDTH * FFT_HEIGHT];
    361     generateH0( m_h_h0 );
    /// 
    ///     host side generation, but it didnt have to be
    ///
    362 
    363     cutilSafeCall( cudaMemcpy( m_d_h0, m_h_h0, fft_input_size, cudaMemcpyHostToDevice) );
    ///
    ///     host to device copy  
    364 
    365     memcpy( m_h0_buffer->map(), m_h_h0, fft_input_size );
    366     m_h0_buffer->unmap();
    ///
    ///    copy from host into OptiX buffer
    ///
    367 
    368     // Finalize
    369     m_context->validate();
    370     m_context->compile();





OptiX and OpenGL interop : OptiX depth buffer calculation
------------------------------------------------------------

* http://rickarkin.blogspot.tw/2012/03/optix-is-ray-tracing-framework-it-can.html


Depth buffer combination may be the most important and a bit complicated. As a
ray tracing engine, Optix need not to do depth buffer test, so one can only
find the rtIntersectionDistance, which means the distance from the ray origin
to current ray-surface intersection point. So handily generate an OpenGL
compliant depth buffer is the first problem. A useful reference is
http://www.songho.ca/opengl/gl_projectionmatrix.html

My realization of the depth value construction is  attached as below::

    // eyeDist:  distance from eye to the intersection point.
    // n:           near clipping plane
    // f:            far clipping plane
    __device__ float computeClipDepth( float eyeDist, float n, float f )
    {
        float clipDepth = (f+n)/(f-n) - (1/eyeDist)*2.0f*f*n/(f-n);
        clipDepth = clipDepth*0.5 + 0.5f;
        return clipDepth;
    }

The second problem is to use the generated depth buffer of Optix into OpenGL.
Actually it is totally OpenGL operations. But maybe its not a daily used
process like draw a triangle or shading a scene object, so there is little
resource could be found on the web.  My realization of the depth value
construction is  also attached as below, where depthImg contains per pixel
depth value, coloredImg contains per pixel color value.::

    glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // save buffer bit attribs to stack 
    glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);            // save client attrib to stack
    //
    // above lines prep for changing attribs by saving current ones
    //
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_FALSE);    
    glWindowPos2i(0, 0);  // specify the raster position in window coordinates for pixel operations
    glDrawPixels(w, h, GL_RGBA , GL_FLOAT, coloredImg);

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_ALWAYS);
    //
    // specify the value used for depth buffer comparisons
    // 
    //    GL_LESS
    //         Passes if the incoming depth value is less than the stored depth value.
    //    GL_ALWAYS
    //         Always passes.  (unconditionally write to depth buffer)
    //  

    glWindowPos2i(0, 0);
    glDrawPixels(w, h, GL_DEPTH_COMPONENT , GL_FLOAT, depthImg);

    //
    //  restore initial attribute state 
    //
    glPopClientAttrib();
    glPopAttrib(); // GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT


OptiX talks
------------

* http://on-demand.gputechconf.com/gtc/2012/presentations/SS107-GPU-Ray-Tracing-OptiX.pdf

  Some details on multi GPU interop and avoiding unnecessary copying 


OptiX without OpenGL
----------------------

::

    delta:bin blyth$ ./sample7 -f out.ppm
    delta:bin blyth$ libpng-
    delta:bin blyth$ cat out.ppm | libpng-wpng > out.png 
    Encoding image data...
    Done.
    delta:bin blyth$ open out.png

 
OptiX and SLI handling of multiple GPUs
----------------------------------------

* http://en.wikipedia.org/wiki/Scalable_Link_Interface
* http://devblogs.nvidia.com/parallelforall/cuda-pro-tip-control-gpu-visibility-cuda_visible_devices/

OptiX doesnt like SLI, switch it off.

::

   export CUDA_VISIBLE_DEVICES=0,1,2



OptiX OpenGL interop with PBO
-------------------------------

* http://rickarkin.blogspot.tw/2012/03/use-pbo-to-share-buffer-between-cuda.html

OptiX OpenGL interop 
---------------------

* :google:`OptiX OpenGL interop`

OptiX_Programming_Guide_3.7.0.pdf
-------------------------------------

OpenGL Interop
~~~~~~~~~~~~~~~~

OptiX supports interop for:

* OpenGL buffer objects
* textures
* render buffers

OpenGL buffer objects can be read and written by OptiX program
objects, whereas textures and render buffers can only be read.

Buffer Objects
~~~~~~~~~~~~~~

OpenGL buffer objects like PBOs and VBOs can be encapsulated for use in OptiX
with rtBufferCreateFromGLBO. The resulting buffer is a reference only to the
OpenGL data; the size of the OptiX buffer as well as the format have to be set
via rtBufferSetSize and rtBufferSetFormat. 
When the OptiX buffer is destroyed, the state of the OpenGL buffer object is unaltered. 
Once an OptiX buffer is created, the original GL buffer object is immutable, 
meaning the properties of the GL object like its size cannot be changed 
while registered with OptiX. However, it is still possible to read and write 
to the GL buffer object using the appropriate GL functions. 
If it is necessary to change properties of an object, first call rtBufferGLUnregister 
before making changes. After the changes are made the object has to be 
registered again with rtBufferGLRegister.
This is necessary to allow OptiX to access the objects data again. Registration
and unregistration calls are expensive and should be avoided if possible.






Caveats
----------

* https://devtalk.nvidia.com/default/topic/751906/?comment=4240594
  
  * rtPrintf not printf


OptiX Usage Examples
---------------------

* https://code.google.com/p/hybrid-rendering-thesis/source/browse/trunk/src/Raytracer/OptixRender.cpp?r=44

* https://github.com/keithroe/Legion/blob/master/src/Legion/Renderer/OptiXScene.cpp

  * CMakeLists.txt CUDA macro usage that stuffs ptx into libraries 

* https://github.com/pspkzar/OptiXRenderer/blob/master/src/OptixRenderer.cpp

* https://github.com/nvpro-samples/gl_optix_composite
* https://github.com/nvpro-samples/gl_optix_composite/blob/master/shaders/optix_triangle_mesh.cu

  * texture lookup example


* http://graphicsrunner.blogspot.tw/2011/03/instant-radiosity-using-optix-and.html


OptiX with GLFW
-----------------

* https://code.google.com/p/hybrid-rendering-thesis/source/browse/trunk/glfw_optix/src/main.cpp

See hrt-



nvfx : Generic Effect system for Graphic APIs, including OpenGL and OptiX
---------------------------------------------------------------------------

nvFx is a new approach for compositing shaders and compute kernels together,
using an API-agnostic description of effects for objects materials and scene
management (post-processing, management of rendering passes).

* **curious how the OptiX side of things is implemented**

  * specifically how material params are fed to the OptiX programs


* :google:`nvfx`

* https://github.com/tlorach/nvFX

* http://lorachnroll.blogspot.tw/2013/07/nvfx-effect-system-on-top-of-many.html



Large Codebases Using OptiX
-----------------------------

* macrosim-


Version Switching
------------------

Use symbolic link for version switching::

    delta:Developer blyth$ ll
    total 8
    drwxr-xr-x   7 root  admin   238 Aug  7  2013 OptiX_301
    drwxr-xr-x   3 root  wheel   102 Jan 15  2014 NVIDIA
    drwxr-xr-x   7 root  admin   238 Dec 18 07:08 OptiX_370b2
    drwxr-xr-x  33 root  wheel  1190 Jan 15 08:46 ..
    lrwxr-xr-x   1 root  wheel     9 Jan 22 11:27 OptiX -> OptiX_301
    drwxr-xr-x   6 root  wheel   204 Jan 22 11:27 .




Samples 
-------

::

   open file:///Developer/OptiX/SDK/NVIDIA-OptiX-SDK-samples.html


Above references a missing sample::

   file:///Developer/OptiX/SDK/collision/


* /Developer/OptiX/SDK-precompiled-samples/sample6.app

  * ray traced geometry of a cow

* /Developer/OptiX/SDK-precompiled-samples/shadeTree.app

  * Christmas decorations 



Building samples including sutil library 
-------------------------------------------


::

    delta:OptiX blyth$ optix-name
    OptiX_370b2
    delta:OptiX blyth$ optix-samples-get-all   # copy samples to writable dir
    delta:OptiX blyth$ optix-samples-cmake
    ...
    -- Found CUDA: /usr/local/cuda (Required is at least version "2.3") 
    -- Found OpenGL: /System/Library/Frameworks/OpenGL.framework  
    -- Found GLUT: -framework GLUT  
    Cannot find Cg, hybridShadows will not be built
    Cannot find Cg.h, hybridShadows will not be built
    Disabling hybridShadows, which requires glut and opengl and Cg.
    Cannot find Cg, isgReflections will not be built
    Cannot find Cg.h, isgReflections will not be built
    Disabling isgReflections, which requires glut and opengl and Cg.
    Cannot find Cg, isgShadows will not be built
    Cannot find Cg.h, isgShadows will not be built
    Disabling isgShadows, which requires glut and opengl and Cg.
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /usr/local/env/cuda/OptiX_370b2_sdk_install

    delta:OptiX blyth$ optix-samples-make





Path to SAMPLES_PTX_DIR gets compiled in
-------------------------------------------

::

    delta:SDK blyth$ find . -name '*.*' -exec grep -H SAMPLES_PTX_DIR {} \;
    ./CMakeLists.txt:set(SAMPLES_PTX_DIR "${CMAKE_BINARY_DIR}/lib/ptx" CACHE PATH "Path to where the samples look for the PTX code.")
    ./CMakeLists.txt:set(CUDA_GENERATED_OUTPUT_DIR ${SAMPLES_PTX_DIR})
    ./CMakeLists.txt:  string(REPLACE "/" "\\\\" SAMPLES_PTX_DIR ${SAMPLES_PTX_DIR})
    ./sampleConfig.h.in:#define SAMPLES_PTX_DIR "@SAMPLES_PTX_DIR@"
    ./sutil/sutil.c:  dir = getenv( "OPTIX_SAMPLES_PTX_DIR" );
    ./sutil/sutil.c:  if( dirExists(SAMPLES_PTX_DIR) )
    ./sutil/sutil.c:    return SAMPLES_PTX_DIR;



OptiX-3.7.0-beta2
-------------------

* need to register with NVIDIA OptiX developer program to gain access 

Package installs into same place as 301::

    delta:Contents blyth$ pwd
    /Volumes/NVIDIA-OptiX-SDK-3.7.0-mac64/NVIDIA-OptiX-SDK-3.7.0-mac64.pkg/Contents
    delta:Contents blyth$ lsbom Archive.bom | head -5
    .   40755   501/0
    ./Developer 40755   501/0
    ./Developer/OptiX   40755   0/80
    ./Developer/OptiX/SDK   40755   0/80
    ./Developer/OptiX/SDK/CMake 40755   0/80

So move that aside::

    delta:Developer blyth$ sudo mv OptiX OptiX_301


* all precompiled samples failing 

::

    terminating with uncaught exception of type optix::Exception: Invalid context

    8   libsutil.dylib                  0x000000010f8b71d6 optix::Handle<optix::ContextObj>::create() + 150
    9   libsutil.dylib                  0x000000010f8b5b1b SampleScene::SampleScene() + 59
    10  libsutil.dylib                  0x000000010f8a6a52 MeshScene::MeshScene(bool, bool, bool) + 34
    11                                  0x000000010f870885 MeshViewer::MeshViewer() + 21


    delta:SDK-precompiled-samples blyth$ open ocean.app
    LSOpenURLsWithRole() failed with error -10810 for the file /Developer/OptiX/SDK-precompiled-samples/ocean.app.
    delta:SDK-precompiled-samples blyth$ 


    8   libsutil.dylib                  0x000000010e1141d6 optix::Handle<optix::ContextObj>::create() + 150
    9   libsutil.dylib                  0x000000010e112b1b SampleScene::SampleScene() + 59
    10                                  0x000000010e0d793c WhirligigScene::WhirligigScene(GLUTDisplay::contDraw_E) + 28



::

    delta:SDK-precompiled-samples blyth$ ./sample3
    OptiX 3.7.0
    Number of Devices = 1

    Device 0: GeForce GT 750M
      Compute Support: 3 0
      Total Memory: 2147024896 bytes
      Clock Rate: 925500 kilohertz
      Max. Threads per Block: 1024
      SM Count: 2
      Execution Timeout Enabled: 1
      Max. HW Texture Count: 128
      TCC driver enabled: 0
      CUDA Device Ordinal: 0

    Constructing a context...
    OptiX Error: Invalid context
    (/Volumes/DATA/teamcity/agent/work/ad29186266c461fa/sw/wsapps/raytracing/rtsdk/rel3.7/samples/sample3/sample3.c:96)
    delta:SDK-precompiled-samples blyth$ 

::

     95   printf("Constructing a context...\n");
     96   RT_CHECK_ERROR(rtContextCreate(&context));
     97 



This is with

* CUDA Driver Version: 5.5.47
* GPU Driver Version: 8.26.26 310.40.45f01



OptiX 301 Install issues 
--------------------------

* attempting to open pkg complains of unidentified developer

  * /Volumes/NVIDIA-OptiX-SDK-3.0.1-mac64/NVIDIA-OptiX-SDK-3.0.1-mac64.pkg

* make exception in `SysPrefs > Security & Privacy > General` 

  * hmm mavericks 10.9.4 "open anyway" doesnt work 
  * authenticate and change to from "anywhere" 


::

    delta:~ blyth$ optix-cmake
    ...
    Specified C compiler /usr/bin/cc is not recognized (gcc, icc).  Using CMake defaults.
    Specified CXX compiler /usr/bin/c++ is not recognized (g++, icpc).  Using CMake defaults.
    CMake Warning at CMake/ConfigCompilerFlags.cmake:195 (message):
      Unknown Compiler.  Disabling SSE 4.1 support
    Call Stack (most recent call first):
      CMakeLists.txt:116 (include)


    -- Found CUDA: /usr/local/cuda (Required is at least version "2.3") 
    -- Found OpenGL: /System/Library/Frameworks/OpenGL.framework  
    -- Found GLUT: -framework GLUT  
    Cannot find Cg, hybridShadows will not be built
    Cannot find Cg.h, hybridShadows will not be built
    Disabling hybridShadows, which requires glut and opengl and Cg.
    Cannot find Cg, isgShadows will not be built
    Cannot find Cg.h, isgShadows will not be built
    Disabling isgShadows, which requires glut and opengl and Cg.
    Cannot find Cg, isgReflections will not be built
    Cannot find Cg.h, isgReflections will not be built
    Disabling isgReflections, which requires glut and opengl and Cg.
    -- Configuring done
    -- Generating done
    -- Build files have been written to: /usr/local/env/cuda/optix301
    delta:optix301 blyth$ 


List the samples::

    delta:optix301 blyth$ optix-make help

All giving error::

    delta:optix301 blyth$ optix-make sample6
    [  7%] Building NVCC ptx file lib/ptx/cuda_compile_ptx_generated_triangle_mesh_small.cu.ptx
    clang: error: unsupported option '-dumpspecs'
    clang: error: no input files
    CMake Error at cuda_compile_ptx_generated_triangle_mesh_small.cu.ptx.cmake:200 (message):
      Error generating
      /usr/local/env/cuda/optix301/lib/ptx/cuda_compile_ptx_generated_triangle_mesh_small.cu.ptx


    make[3]: *** [lib/ptx/cuda_compile_ptx_generated_triangle_mesh_small.cu.ptx] Error 1
    make[2]: *** [sutil/CMakeFiles/sutil.dir/all] Error 2
    make[1]: *** [sample6/CMakeFiles/sample6.dir/rule] Error 2
    make: *** [sample6] Error 2
    delta:optix301 blyth$ 

Seems that nvcc is running clang internally with non existing option::

    delta:optix301 blyth$ /usr/local/cuda/bin/nvcc -M -D__CUDACC__ /Developer/OptiX/SDK/cuda/triangle_mesh_small.cu -o /usr/local/env/cuda/optix301/sutil/CMakeFiles/cuda_compile_ptx.dir/__/cuda/cuda_compile_ptx_generated_triangle_mesh_small.cu.ptx.NVCC-depend -ccbin /usr/bin/cc -m64 -DGLUT_FOUND -DGLUT_NO_LIB_PRAGMA --use_fast_math -U__BLOCKS__ -DNVCC -I/usr/local/cuda/include -I/Developer/OptiX/include -I/Developer/OptiX/SDK/sutil -I/Developer/OptiX/include/optixu -I/usr/local/env/cuda/optix301 -I/usr/local/cuda/include -I/System/Library/Frameworks/GLUT.framework/Headers -I/Developer/OptiX/SDK/sutil -I/Developer/OptiX/SDK/cuda
    clang: error: unsupported option '-dumpspecs'
    clang: error: no input files
    delta:optix301 blyth$ 


cmake debug
~~~~~~~~~~~~~

* added "--verbose"
* adding "-ccbin /usr/bin/clang" gets past the "--dumpspecs" failure, now get

    nvcc fatal   : redefinition of argument 'compiler-bindir'


* /Developer/OptiX/SDK/CMake/FindCUDA/run_nvcc.cmake::

    108 # Any -ccbin existing in CUDA_NVCC_FLAGS gets highest priority
    109 list( FIND CUDA_NVCC_FLAGS "-ccbin" ccbin_found0 )
    110 list( FIND CUDA_NVCC_FLAGS "--compiler-bindir" ccbin_found1 )
    111 if( ccbin_found0 LESS 0 AND ccbin_found1 LESS 0 )
    112   if (CUDA_HOST_COMPILER STREQUAL "$(VCInstallDir)bin" AND DEFINED CCBIN)
    113     set(CCBIN -ccbin "${CCBIN}")
    114   else()
    115     set(CCBIN -ccbin "${CUDA_HOST_COMPILER}")
    116   endif()
    117 endif()
     
    * http://public.kitware.com/Bug/view.php?id=13674


cmake fix
~~~~~~~~~~~~~~


Kludge the cmake::

    delta:FindCUDA blyth$ sudo cp run_nvcc.cmake run_nvcc.cmake.original
    delta:FindCUDA blyth$ pwd
    /Developer/OptiX/SDK/CMake/FindCUDA

Turns out not to be necessary, the cmake flag does the trick::

   cmake $(optix-dir) -DCUDA_NVCC_FLAGS="-ccbin /usr/bin/clang"
    

* :google:`cuda 5.5 clang`
* http://stackoverflow.com/questions/19351219/cuda-clang-and-os-x-mavericks
* http://stackoverflow.com/questions/12822205/nvidia-optix-geometrygroup


Check Optix Raytrace Speed on DYB geometry
--------------------------------------------

::

    In [3]: v=np.load(os.path.expandvars("$DAE_NAME_DYB_CHROMACACHE_MESH/vertices.npy"))

    In [4]: v
    Out[4]: 
    array([[ -16585.725, -802008.375,   -3600.   ],
           [ -16039.019, -801543.125,   -3600.   ],
           [ -15631.369, -800952.188,   -3600.   ],
           ..., 
           [ -14297.924, -801935.812,  -12110.   ],
           [ -14414.494, -801973.438,  -12026.   ],
           [ -14414.494, -801973.438,  -12110.   ]], dtype=float32)

    In [5]: v.shape
    Out[5]: (1216452, 3)

    In [6]: t = np.load(os.path.expandvars("$DAE_NAME_DYB_CHROMACACHE_MESH/triangles.npy"))
    In [7]: t.shape
    Out[7]: (2402432, 3)
    In [8]: t.max()
    Out[8]: 1216451
    In [9]: t.min()
    Out[9]: 0


Write geometry in obj format::

    In [11]: fp = file("/tmp/dyb.obj", "w")
    In [12]: np.savetxt(fp, v, fmt="v %.18e %.18e %.18e")
    In [13]: np.savetxt(fp, t, fmt="f %d %d %d")
    In [14]: fp.close()

Geometry appears mangled, as obj format does not handle Russian doll geometry, 
but the optix raytrace is interactive (unless some trickery being used, that is 
greatly faster than chroma raytrace). Fast enough to keep me interested::

    ./sample6 --cache --obj /tmp/dyb.obj --light-scale 5


How to load COLLADA into OptiX ?
-----------------------------------

* nvidia Scenix looks abandoned

* plumped for assimp following example of oppr- example, see assimp- assimptest- raytrace--

* oppr- converts ASSIMP imported mesh into OptiX geometry::

    delta:OppositeRenderer blyth$ find . -name '*.cpp' -exec grep -H getSceneRootGroup {} \;
    ./RenderEngine/renderer/OptixRenderer.cpp:        m_sceneRootGroup = scene.getSceneRootGroup(m_context);
    ./RenderEngine/scene/Cornell.cpp:optix::Group Cornell::getSceneRootGroup(optix::Context & context)
    ./RenderEngine/scene/Scene.cpp:optix::Group Scene::getSceneRootGroup( optix::Context & context )
    delta:OppositeRenderer blyth$ 


OptiX Tutorial
---------------

* http://docs.nvidia.com/gameworks/content/gameworkslibrary/optix/optix_quickstart.htm

Tutorials gt 5 asserting in rtContextCompile::

    delta:bin blyth$ ./tutorial -T 5 
    OptiX Error: Unknown error (Details: Function "RTresult _rtContextCompile(RTcontext_api*)" caught exception: Assertion failed: [1312612])

Binary search reveals the culprit to be the *sin(phi)*::

     74 rtTextureSampler<float4, 2> envmap;
     75 RT_PROGRAM void envmap_miss()
     76 {
     77   float theta = atan2f( ray.direction.x, ray.direction.z );
     78   float phi   = M_PIf * 0.5f -  acosf( ray.direction.y );
     79   float u     = (theta + M_PIf) * (0.5f * M_1_PIf);
     80   float v     = 0.5f * ( 1.0f + sin(phi) );
     81   // the "sin" above causing an assert with OptiX_301 CUDA 5.5 without --use_fast_math 
     82   prd_radiance.result = make_float3( tex2D(envmap, u, v) );
     83 } 

* https://devtalk.nvidia.com/default/topic/559505/apparently-an-unexplicable-error/

Resolved by adding *--use_fast_math* to the cmake commandline setting CUDA_NVCC_FLAGS::

   cmake -DOptiX_INSTALL_DIR=$(optix-install-dir) -DCUDA_NVCC_FLAGS="-ccbin /usr/bin/clang --use_fast_math " "$(optix-sdir)"

After a few crashes like the above observe GPU memory to be almost full
and attempts to run anything on the GPU fail with a system 
exception report. To free up some GPU memory sleep/revive the machine::

    delta:bin blyth$ cu
    timestamp                Fri Jan 23 10:44:11 2015
    tag                      default
    name                     GeForce GT 750M
    compute capability       (3, 0)
    memory total             2.1G
    memory used              2.1G
    memory free              51.4M
    delta:bin blyth$ 


f64 check
-----------

::

    delta:raytrace blyth$ grep f64 *.ptx
    MeshViewer_generated_TriangleMesh.cu.ptx:   .target sm_10, map_f64_to_f32
    MeshViewer_generated_TriangleMesh.cu.ptx:   ld.global.f32   %f64, [ray+28];
    MeshViewer_generated_TriangleMesh.cu.ptx:   set.lt.u32.f32  %r30, %f64, %f58;
    MeshViewer_generated_material1.cu.ptx:  .target sm_10, map_f64_to_f32
    RayTrace_generated_TriangleMesh.cu.ptx: .target sm_10, map_f64_to_f32
    RayTrace_generated_TriangleMesh.cu.ptx: ld.global.f32   %f64, [ray+28];
    RayTrace_generated_TriangleMesh.cu.ptx: set.lt.u32.f32  %r30, %f64, %f58;
    RayTrace_generated_material0.cu.ptx:    .target sm_10, map_f64_to_f32
    RayTrace_generated_material1.cu.ptx:    .target sm_10, map_f64_to_f32
    RayTrace_generated_tutorial0.cu.ptx:    .target sm_10, map_f64_to_f32
    RayTrace_generated_tutorial0.cu.ptx:    cvt.sat.f32.f32     %f64, %f63;
    RayTrace_generated_tutorial0.cu.ptx:    mul.f32     %f66, %f64, %f65;
    delta:raytrace blyth$ 


Following updating CUDA from 5.5 to 6.5 get
---------------------------------------------

While still using OptiX301::

    delta:sample3 blyth$ raytrace-v -n
    [ 19%] Built target AssimpGeometryTest
    Scanning dependencies of target MeshViewer
    [ 22%] Building CXX object CMakeFiles/MeshViewer.dir/MeshViewer.cpp.o
    Linking CXX executable MeshViewer
    [ 61%] Built target MeshViewer
    [100%] Built target RayTrace
    dyld: Library not loaded: @rpath/libcudart.dylib
      Referenced from: /Developer/OptiX/lib64/liboptix.1.dylib
      Reason: Incompatible library version: liboptix.1.dylib requires version 1.1.0 or later, but libcudart.5.5.dylib provides version 0.0.0
    Trace/BPT trap: 5
    delta:sample3 blyth$ 


But with the beta OptiX_370b2 the invalid context issue is gone::

	delta:SDK-precompiled-samples blyth$ ./sample3
	OptiX 3.7.0
	Number of Devices = 1

	Device 0: GeForce GT 750M
	  Compute Support: 3 0
	  Total Memory: 2147024896 bytes
	  Clock Rate: 925500 kilohertz
	  Max. Threads per Block: 1024
	  SM Count: 2
	  Execution Timeout Enabled: 1
	  Max. HW Texture Count: 128
	  TCC driver enabled: 0
	  CUDA Device Ordinal: 0

	Constructing a context...
	  Created with 1 device(s)
	  Supports 2147483647 simultaneous textures
	  Free memory:
	    Device 0: 1099292672 bytes


OptiX cmake claims needs 2.8.8 by appears to build ok with 2.6.4
-------------------------------------------------------------------

::

    CMake Error at CMakeLists.txt:82 (cmake_minimum_required):
      CMake 2.8.8 or higher is required.  You are running version 2.6.4

::

    cmake-samples-cmake-kludge  # inplace edit the version requirement



Each OptiX release requires a different driver release
----------------------------------------------------------

sample7 works with Optix370 (new driver 340.65)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample7 -f out.ppm
    -bash-4.1$ ll *.ppm
    -rw-r--r-- 1 blyth dyw 2211856 Feb  9 11:53 out.ppm
    -bash-4.1$ date
    Mon Feb  9 11:54:10 CST 2015
    -bash-4.1$ optix-name
    NVIDIA-OptiX-SDK-3.7.0-linux64
    -bash-4.1$ 

    -bash-4.1$ cat /proc/driver/nvidia/version
    NVRM version: NVIDIA UNIX x86_64 Kernel Module  340.65  Tue Dec  2 09:50:34 PST 2014
    GCC version:  gcc version 4.4.7 20120313 (Red Hat 4.4.7-4) (GCC) 


sample7 fails with OptiX 363 (old driver)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

	-bash-4.1$ ./sample7 -f out.ppm
	OptiX Error: Invalid context (Details: Function "RTresult _rtContextLaunch2D(RTcontext, unsigned int, RTsize, RTsize)" caught exception: Unable to set the CUDA device., [3735714])
	-bash-4.1$ 
	-bash-4.1$ pwd
	/dyb/dybd07/user/blyth/hgpu01.ihep.ac.cn/env/cuda/NVIDIA-OptiX-SDK-3.6.3-linux64_sdk_install/bin
	-bash-4.1$ 


sample7 works with OptiX 351 (old driver)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    -bash-4.1$ LD_LIBRARY_PATH=. ./sample7 -f sample7.ppm

    delta:~ blyth$ scp L6:/dyb/dybd07/user/blyth/hgpu01.ihep.ac.cn/OptiX/SDK-precompiled-samples/sample7.ppm .
    delta:~ blyth$ libpng-
    delta:~ blyth$ cat sample7.ppm | libpng-wpng > sample7.png
    Encoding image data...
    Done.
    delta:~ blyth$ open sample7.png



Benchmark
------------

Quick look at scaling with GPU cores, close to linear 

* CUDA_VISIBLE_DEVICES=0
* CUDA_VISIBLE_DEVICES=1
* unset 

* K20m 2496 cores each,  total 4992 cores

* 750m 384 cores 

::

    In [2]: 4992./384.
    Out[2]: 13.0

    In [3]: 345.154/28.03
    Out[3]: 12.313735283624688

    In [4]: 49.1/28.03
    Out[4]: 1.7516946129147342


1,1,2 K20m::


    Time to load geometry: 5.05531 s.
    Time to compile kernel: 0.220482 s.
    Time to build AS      : 0.162403 s.
    PREPROCESS: MeshViewer | compile 0.000169992 sec | accelbuild 0.0002141 sec
    BENCHMARK: MeshViewer | 20.3645 fps | 1 (warmup) | 2 (timed) | 0.0982101 sec | 49.105 ms/f 

    Time to load geometry: 4.99532 s.
    Time to compile kernel: 0.218149 s.
    Time to build AS      : 0.160232 s.
    PREPROCESS: MeshViewer | compile 0.000129938 sec | accelbuild 0.000213146 sec
    BENCHMARK: MeshViewer | 20.354 fps | 1 (warmup) | 2 (timed) | 0.0982609 sec | 49.1304 ms/f 

    Time to load geometry: 4.9231 s.
    Time to compile kernel: 0.22342 s.
    Time to build AS      : 0.312235 s.
    PREPROCESS: MeshViewer | compile 0.000135899 sec | accelbuild 0.000245094 sec
    BENCHMARK: MeshViewer | 35.669 fps | 1 (warmup) | 2 (timed) | 0.056071 sec | 28.0355 ms/f 


1 750m::

    Time to load geometry: 3.05554 s.
    Time to compile kernel: 0.160299 s.
    Time to build AS      : 0.286529 s.
    PREPROCESS: MeshViewer | compile 9.70364e-05 sec | accelbuild 0.000139952 sec
    BENCHMARK: MeshViewer | 2.89726 fps | 1 (warmup) | 2 (timed) | 0.690308 sec | 345.154 ms/f 






EOU
}



optix-export(){
   export OPTIX_SDK_DIR=$(optix-sdk-dir)
   export OPTIX_INSTALL_DIR=$(optix-install-dir)
   export OPTIX_SAMPLES_INSTALL_DIR=$(optix-samples-install-dir)
}

optix-fold(){ 
   case $NODE_TAG in 
      D)  echo /Developer ;;
      G1) echo $(local-base) ;;
      LT) echo /home/ihep/data/repo/opticks ;;
      *) echo $(local-base) ;;
   esac
}
optix-dir(){         echo $(optix-fold)/OptiX/SDK ; }
optix-sdk-dir-old(){ echo $(optix-fold)/OptiX_301/SDK ; }
optix-sdk-dir(){     echo $(optix-fold)/OptiX/SDK ; }
optix-download-dir(){ echo $(local-base)/env/cuda ; }
optix-bdir(){         echo $(local-base)/env/cuda/$(optix-name) ; }
optix-install-dir(){ echo $(dirname $(optix-sdk-dir)) ; }
optix-idir(){        echo $(dirname $(optix-sdk-dir))/include ; }
optix-sdir(){        echo $(env-home)/optix ; }
optix-samples-src-dir(){     echo $(local-base)/env/cuda/$(optix-name)_sdk ; }
optix-samples-install-dir(){ echo $(local-base)/env/cuda/$(optix-name)_sdk_install ; }

optix-samples-scd(){   cd $(optix-samples-src-dir)/$1 ; }
optix-samples-cd(){    cd $(optix-samples-install-dir)/$1 ; }
optix-download-cd(){   cd $(optix-download-dir) ; }

optix-ftp(){ open https://ftpservices.nvidia.com ; }


optix-cd(){  cd $(optix-dir); }
optix-bcd(){ cd $(optix-samples-install-dir); }
optix-scd(){ cd $(optix-sdir); }
optix-icd(){ cd $(optix-idir); }
optix-doc(){ cd $(optix-fold)/OptiX/doc ; }

optix-samples-find(){    optix-samples-cppfind $* ; }
optix-samples-cufind(){  find $(optix-samples-src-dir) -name '*.cu'  -exec grep ${2:--H} ${1:-rtReportIntersection} {} \; ;}
optix-samples-hfind(){   find $(optix-samples-src-dir) -name '*.h'   -exec grep ${2:--H} ${1:-rtReportIntersection} {} \; ;}
optix-samples-cppfind(){ find $(optix-samples-src-dir) -name '*.cpp' -exec grep ${2:--H} ${1:-rtReportIntersection} {} \; ;}
optix-find(){            find $(optix-idir)            -name '*.h'   -exec grep ${2:--H} ${1:-setMiss} {} \; ; }
optix-ifind(){           find $(optix-idir)            -name '*.h'   -exec grep ${2:--H} ${1:-setMiss} {} \; ; }

optix-pdf(){ open $(optix-fold)/OptiX/doc/OptiX_Programming_Guide_$(optix-version).pdf ; }



optix-readlink(){ readlink $(optix-fold)/OptiX ; }
optix-name(){  echo ${OPTIX_NAME:-$(optix-readlink)} ; }
optix-jump(){    
   local iwd=$PWD
   local ver=${1:-301}
   cd $(optix-fold)
   sudo ln -sfnv OptiX_$ver OptiX 
   cd $iwd
}
optix-old(){   optix-jump 301 ; }
optix-beta(){  optix-jump 370b2 ; }

optix-linux-name(){
   case $1 in 
      351) echo NVIDIA-OptiX-SDK-3.5.1-PRO-linux64 ;;
      363) echo NVIDIA-OptiX-SDK-3.6.3-linux64 ;;
      370) echo NVIDIA-OptiX-SDK-3.7.0-linux64 ;;
   esac
}

optix-version(){
   case $(optix-name) in 
     OptiX_380)   echo 3.8.0 ;;
     OptiX_301)   echo 3.0.2 ;;
     OptiX_370b2) echo 3.7.0 ;;
  esac
}


optix-linux-jump(){
    local vers=${1:-351}
    local name=$(optix-linux-name $vers)
    [ -z "$name" ] && echo $msg version $vers unknown && type optix-linux-name && return 

    cd $(optix-fold)    
    ln -sfnv $name OptiX
}

   



optix-samples-names(){ cat << EON
CMakeLists.txt
sampleConfig.h.in
cuda
CMake
sample1
sample2
sample3
sample4
sample5
sample5pp
sample6
sample7
sample8
simpleAnimation
sutil
tutorial
materials
transparency
EON

## remember that after adding a name here, need to uncomment in the CMakeLists.txt for it to get built
}



optix-samples-get-all(){

   local src=$(optix-sdk-dir)
   local dst=$(optix-samples-src-dir)
 
   mkdir -p $dst

   echo $FUNCNAME copy all samples to somewhere writable 
   cp -R $src/* $dst/
 
}


optix-samples-get-selected(){
   local sdir=$(optix-samples-src-dir)
   mkdir -p $sdir

   local src=$(optix-sdk-dir)
   local dst=$sdir
   local cmd
   local name
   optix-samples-names | while read name ; do 

      if [ -d "$src/$name" ]
      then 
          if [ ! -d "$dst/$name" ] ; then 
              cmd="cp -r $src/$name $dst/"
          else
              cmd="echo destination directory exists already $dst/$name"
          fi
      elif [ -f "$src/$name" ] 
      then 
          if [ ! -f "$dst/$name" ] ; then 
              cmd="cp $src/$name $dst/$name"
          else
              cmd="echo destination file exists already $dst/$name"
          fi
      else
          cmd="echo src $src/$name missing"
      fi 
      #echo $cmd
      eval $cmd
   done
}


optix-cuda-nvcc-flags(){
    case $NODE_TAG in 
       D) echo -ccbin /usr/bin/clang --use_fast_math ;;
       *) echo --use_fast_math ;; 
    esac
}



#optix-samples-cmake-kludge(){
#    optix-samples-scd
#    grep cmake_minimum_required CMakeLists.txt 
#    perl -pi -e 's,2.8.8,2.6.4,' CMakeLists.txt 
#    grep cmake_minimum_required CMakeLists.txt 
#}


optix-samples-cmake(){
    local iwd=$PWD
    local bdir=$(optix-samples-install-dir)
    #rm -rf $bdir   # starting clean 
    mkdir -p $bdir
    optix-bcd
    cmake -DOptiX_INSTALL_DIR=$(optix-install-dir) \
          -DCUDA_NVCC_FLAGS="$(optix-cuda-nvcc-flags)" \
           "$(optix-samples-src-dir)"
    cd $iwd
}

optix-samples-make(){
    local iwd=$PWD
    optix-bcd
    make $* 
    cd $iwd
}




optix-samples-run(){
    local name=${1:-materials}
    optix-samples-make $name
    local cmd="$(optix-bdir)/bin/$name"
    echo $cmd
    eval $cmd
}

optix-tutorial(){
    local tute=${1:-10}

    optix-samples-make tutorial

    local cmd="$(optix-bdir)/bin/tutorial -T $tute --texture-path $(optix-sdk-dir)/tutorial/data"
    echo $cmd
    eval $cmd
}


optix-tutorial-cd(){
   cd $(optix-sdk-dir)/tutorial
}
optix-tutorial-vi(){
   vi $(optix-sdk-dir)/tutorial/*
}




optix-verbose(){
  export VERBOSE=1 
}
optix-unverbose(){
  unset VERBOSE
}



optix-check(){
/usr/local/cuda/bin/nvcc -ccbin /usr/bin/clang --verbose -M -D__CUDACC__ /Developer/OptiX/SDK/cuda/triangle_mesh_small.cu -o /usr/local/env/cuda/optix301/sutil/CMakeFiles/cuda_compile_ptx.dir/__/cuda/cuda_compile_ptx_generated_triangle_mesh_small.cu.ptx.NVCC-depend -ccbin /usr/bin/cc -m64 -DGLUT_FOUND -DGLUT_NO_LIB_PRAGMA --use_fast_math -U__BLOCKS__ -DNVCC -I/usr/local/cuda/include -I/Developer/OptiX/include -I/Developer/OptiX/SDK/sutil -I/Developer/OptiX/include/optixu -I/usr/local/env/cuda/optix301 -I/usr/local/cuda/include -I/System/Library/Frameworks/GLUT.framework/Headers -I/Developer/OptiX/SDK/sutil -I/Developer/OptiX/SDK/cuda
}



optix-check-2(){

cd /usr/local/env/cuda/OptiX_301/tutorial && /usr/bin/c++   -DGLUT_FOUND -DGLUT_NO_LIB_PRAGMA -fPIC -O3 -DNDEBUG \
     -I/Developer/OptiX/include \
     -I/Users/blyth/env/cuda/optix/OptiX_301/sutil \
     -I/Developer/OptiX/include/optixu \
     -I/usr/local/env/cuda/OptiX_301 \
     -I/usr/local/cuda/include \
     -I/System/Library/Frameworks/GLUT.framework/Headers \
       -o /dev/null \
       -c /Users/blyth/env/cuda/optix/OptiX_301/tutorial/tutorial.cpp

}



optix-diff(){
   local name=${1:-sutil/MeshScene.h}
   local cmd="diff $(optix-sdk-dir-old)/$name $(optix-sdk-dir)/$name"
   echo $cmd
   eval $cmd
}

optix-rdiff(){
   local rel="sutil"
   local cmd="diff -r --brief $(optix-sdk-dir-old)/$rel $(optix-sdk-dir)/$rel"
   echo $cmd
   eval $cmd
}





