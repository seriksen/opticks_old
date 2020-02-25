# CentOS 7 Opticks Guide

This guide uses
```code
Oracle VM.GPU2.1 with Tesla P100
CentOS 7
CUDA 10.1
NVidia OptiX 6.0.0
Opticks 1e25685 https://bitbucket.org/simoncblyth/opticks/commits/1e25685ced6a2867d432a84946877adcdd981c62
```

## Setup X11
X11 is required for NVidia OptiX tests.
```code
# Update the packages
[opc@bristollz ~]$ sudo yum -y update

[opc@bristollz ~]$ sudo yum install xorg-x11-apps.x86_64
[opc@bristollz ~]$ sudo yum install xauth

# Now log in using -X for X11 forwarding
ak18773@samsdell:~$ ssh -AX opc@129.213.63.102

# Verify
[opc@bristollz ~]$ xclock
```

## Install Nvidia driver
The installation and verification of the driver is described here. 
The easiest way is to use ELRepo (but this causes conflicts with cuda later)
Some more information is here https://rpmfusion.org/Howto/NVIDIA?highlight=%28CategoryHowto%29
But ignore all of them... do this instead.

```code
# Check that there is a GPU
[opc@bristollz ~]$ lspci | grep NVIDIA
00:04.0 3D controller: NVIDIA Corporation GP100GL [Tesla P100 SXM2 16GB] (rev a1)
```
Check the driver. 
We can see that `nouveau` is the driver. 
We must disable this and use the NVidia one.
```code
[opc@bristollz ~]$ sudo lshw -numeric -C display
  *-display:0               
       description: VGA compatible controller
       product: [1234:1111]
       vendor: [1234]
       physical id: 2
       bus info: pci@0000:00:02.0
       version: 02
       width: 32 bits
       clock: 33MHz
       capabilities: vga_controller bus_master rom
       configuration: driver=bochs-drm latency=0
       resources: irq:0 memory:c0000000-c0ffffff memory:c2001000-c2001fff memory:c2010000-c201ffff
  *-display:1
       description: 3D controller
       product: GP100GL [Tesla P100 SXM2 16GB] [10DE:15F9]
       vendor: NVIDIA Corporation [10DE]
       physical id: 4
       bus info: pci@0000:00:04.0
       version: a1
       width: 64 bits
       clock: 33MHz
       capabilities: bus_master cap_list
       configuration: driver=nouveau latency=0
       resources: iomemory:200-1ff iomemory:240-23f irq:32 memory:c1000000-c1ffffff memory:2000000000-23ffffffff memory:2400000000-2401ffffff
```

Download the driver from [here](https://www.nvidia.co.uk/Download/index.aspx?lang=uk).
Then upload to the instance. 
Streamline this!
```code
ak18773@samsdell:~/Downloads$ scp nvidia-diag-driver-local-repo-rhel7-418.67-1.0-1.x86_64.rpm opc@132.145.215.173:~
```

Now install the driver and and reboot 
```code
[opc@bristollz ~]$ sudo rpm -i nvidia-diag-driver-local-repo-rhel7-418.67-1.0-1.x86_64.rpm
[opc@bristollz ~]$ sudo yum clean all
[opc@bristollz ~]$ sudo yum install cuda-drivers
[opc@bristollz ~]$ sudo reboot
[opc@bristollz ~]$ nvidia-smi
```
Now check that the driver being used
```code
[opc@bristollz ~]$ sudo lshw -numeric -C display
  *-display:0               
       description: VGA compatible controller
       product: [1234:1111]
       vendor: [1234]
       physical id: 2
       bus info: pci@0000:00:02.0
       version: 02
       width: 32 bits
       clock: 33MHz
       capabilities: vga_controller bus_master rom
       configuration: driver=bochs-drm latency=0
       resources: irq:0 memory:c0000000-c0ffffff memory:c2001000-c2001fff memory:c2010000-c201ffff
  *-display:1
       description: 3D controller
       product: GP100GL [Tesla P100 SXM2 16GB] [10DE:15F9]
       vendor: NVIDIA Corporation [10DE]
       physical id: 4
       bus info: pci@0000:00:04.0
       version: a1
       width: 64 bits
       clock: 33MHz
       capabilities: pm msi pciexpress bus_master cap_list
       configuration: driver=nvidia latency=0
       resources: iomemory:200-1ff iomemory:240-23f irq:10 memory:c1000000-c1ffffff memory:2000000000-23ffffffff memory:2400000000-2401ffffff
```

## Install CUDA
Instruction are taken from https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#ubuntu-installation and the cuda download instructions from https://developer.nvidia.com/cuda-downloads.
The post installation actions can found at https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#post-installation-actions.

```code
[opc@bristollz ~]$ sudo yum install kernel-devel-$(uname -r) kernel-headers-$(uname -r)

# enable EPEL
[opc@bristollz ~]$ sudo yum install epel-release


[opc@bristollz ~]$ wget http://developer.download.nvidia.com/compute/cuda/repos/rhel7/x86_64/cuda-repo-rhel7-10.1.168-1.x86_64.rpm
[opc@bristollz ~]$ wget -O ~/cuda_key "http://developer.download.nvidia.com/compute/cuda/repos/rhel7/x86_64/7fa2af80.pub"
[opc@bristollz ~]$ sudo rpm --import ~/cuda_key.pub

[opc@bristollz ~]$ sudo rpm -i cuda-repo-rhel7-10.1.168-1.x86_64.rpm
[opc@bristollz ~]$ sudo yum clean all

[opc@bristollz ~]$ sudo yum install cuda

# reboot
[opc@bristollz ~]$ sudo reboot

# Post installation actions (add to custom_opticks_setup.sh later)
[opc@bristollz ~]$ export PATH=/usr/local/cuda-10.1/bin:/usr/local/cuda-10.1/NsightCompute-2019.3${PATH:+:${PATH}}
[opc@bristollz ~]$ export LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}

# recommended options
[opc@bristollz ~]$ sudo yum install freeglut-devel libX11-devel libXi-devel libXmu-devel make mesa-libGLU-devel

# Verify CUDA
# THERE may be an issue with setting up the enviroment (call the exports from post install actions to fix)
# In order for this verification to work, PATH and LD_LIBRARY_PATH must include CUDA locations
[opc@bristollz ~]$ mkdir cuda-samples && cd cuda-samples
[opc@bristollz cuda-samples]$ cuda-install-samples-10.1.sh .
[opc@bristollz cuda-samples]$ nvcc --version
nvcc: NVIDIA (R) Cuda compiler driver
Copyright (c) 2005-2019 NVIDIA Corporation
Built on Wed_Apr_24_19:10:27_PDT_2019
Cuda compilation tools, release 10.1, V10.1.168

[opc@bristollz simpleAssert]$ cd /home/opc/cuda-samples/NVIDIA_CUDA-10.1_Samples/0_Simple/simpleAssert
[opc@bristollz simpleAssert]$ make

[opc@bristollz simpleAssert]$ ./simpleAssert 
simpleAssert starting...
OS_System_Type.release = 3.10.0-957.21.3.el7.x86_64
OS Info: <#1 SMP Tue Jun 18 16:35:19 UTC 2019>

GPU Device 0: "Tesla P100-SXM2-16GB" with compute capability 6.0

Launch kernel to generate assertion failures

-- Begin assert output

simpleAssert.cu:47: void testKernel(int): block: [1,0,0], thread: [28,0,0] Assertion `gtid < N` failed.
simpleAssert.cu:47: void testKernel(int): block: [1,0,0], thread: [29,0,0] Assertion `gtid < N` failed.
simpleAssert.cu:47: void testKernel(int): block: [1,0,0], thread: [30,0,0] Assertion `gtid < N` failed.
simpleAssert.cu:47: void testKernel(int): block: [1,0,0], thread: [31,0,0] Assertion `gtid < N` failed.

-- End assert output

Device assert failed as expected, CUDA error message is: device-side assert triggered

simpleAssert completed, returned OK
```

## Install Nvidia OptiX
To get OptiX requires an account with the NVIDIA developer program https://developer.nvidia.com/optix.
An account is free.
Once you have an account, download the bash script from the address above.
You must then upload this script to Oracle.
```code
# Prepare instance
[opc@bristollz ~]$ mkdir OptiX

# Upload to OCI instance
# Note, this is a few hundred MB so takes a few minutes
ak18773@samsdell:~/Downloads$ scp NVIDIA-OptiX-SDK-6.0.0-linux64-25650775.sh opc@132.145.219.8:/home/opc/OptiX/

# Install OptiX
[opc@bristollz ~]$ cd OptiX/
[opc@bristollz OptiX]$ sh NVIDIA-OptiX-SDK-6.0.0-linux64-25650775.sh
Do you accept the license? [yN]: 
y
By default the NVIDIA OptiX will be installed in:
  "/home/ubuntu/OptiX/NVIDIA-OptiX-SDK-6.0.0-linux64"
Do you want to include the subdirectory NVIDIA-OptiX-SDK-6.0.0-linux64?
Saying no will install in: "/home/opc/OptiX" [Yn]: 
y

Using target directory: /home/opc/OptiX/NVIDIA-OptiX-SDK-6.0.0-linux64
Extracting, please wait...

Unpacking finished successfully

# Verify OptiX
[opc@bristollz OptiX]$ cd NVIDIA-OptiX-SDK-6.0.0-linux64/SDK-precompiled-samples/
[opc@bristollz SDK-precompiled-samples]$ export LD_LIBRARY_PATH=${PWD}:+:${LD_LIBRARY_PATH}
[opc@bristollz SDK-precompiled-samples]$ ./optixHello --file hello.pbm
[opc@bristollz SDK-precompiled-samples]$ sudo yum install ImageMagick ImageMagick-devel -y
[opc@bristollz SDK-precompiled-samples]$ display hello.pbm
```
On ubuntu running just ./optixHello just works and nopbo isn't an option. 
It's an option on most distributions but not RHEL7/Centos7.
See https://devtalk.nvidia.com/default/topic/1046459/optix/optixmotionblur-unknown-error/

To run some of the other tests, see what commands they have ie. `./optixHello --help`
Many will be able to write out to a file which can then be viewed as above `display <pbm file>`

```
The system will need to be rebooted after this
```code
ubuntu@bristollz:~$ sudo reboot
```
## Install Opticks
Much of this information is available from https://simoncblyth.bitbucket.io/opticks/docs/opticks.html.
Additional information can be found at https://github.com/63990/opticks-install
A large portion of the dependencies can be installed via opticks, including cmake.
To install opticks you have to be root.

### Important notes before installing
Many of the issues which you will face during installation are already documented in `notes/issues/` of opticks.
Other issues are listed at https://groups.io/g/opticks/topics.

### Opticks instructions
Using the opticks functions to build/install things is not always error free. 
It may also gloss over errors so it is best to have a directory of logs.
```code
ubuntu@bristollz:~$ mkdir opticks_build_logs
```

Now as root add the additions from ubuntu user to root .bashrc.
As many of the opticks tests are run NOT interactively, .bashrc is called multiple times, therefore the sourcing should be one of the first things in the script.
```code
# opticks source script
source /home/opc/custom_opticks_setup.sh
```

Now create custom_opticks_setup.sh and fill it with the following
```code
# CUDA Path
export PATH=/usr/local/cuda-10.1/bin:/usr/local/cuda-10.1/NsightCompute-2019.1${PATH:+:${PATH}}
export LD_LIBRARY_PATH=/usr/local/cuda-10.1/lib64${LD_LIRARY_PATH:+:${LD_LIBRARY_PATH}}

# NVidia OptiX Path
export LD_LIBRARY_PATH=/home/opc/OptiX/NVIDIA-OptiX-SDK-6.0.0-linux64/lib64${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}
```

Now can start with opticks.
```code
# Install mercurial
[opc@bristollz ~]$ sudo yum install -y mercurial

# Clone opticks
[opc@bristollz ~]$ hg clone http://bitbucket.org/simoncblyth/opticks

# Checkout which version we need
[opc@bristollz ~]$ cd opticks/
[opc@bristollz opticks]$ hg checkout <commit hash>
```

Edit custom_opticks_setup.sh with the following to make interacting with opticks simpler.
This is taken from the opticks installation guide (linked at the start of this section).
This may need to be updated, see custom_opticks_setup.sh (not currently included).
NOTE: Before these, also include the CUDA and NVidia OptiX exports from before.
```code
# Opticks
export LOCAL_BASE=${HOME}
export OPTICKS_HOME=${HOME}/opticks
export PYTHONPATH=${HOME}
export PATH=${OPTICKS_HOME}/bin:${OPTICKS_HOME}/ana:${LOCAL_BASE}/opticks/lib:${PATH}
opticks-(){  [ -r $OPTICKS_HOME/opticks.bash ] && . $OPTICKS_HOME/opticks.bash && opticks-env $* ; }
opticks-     ##

o(){ cd $(opticks-home) ; hg st ; }
op(){ op.sh $* ; }
```
Now rerun .bashrc to get opticks functions.

Now it is possible to check if the environment is set up correctly. 
We can see that cmake is missing.
```code
[opc@bristollz ~]$ bash -lc "opticks- ; opticks-info " 
/home/opc/opticks/opticks.bash: line 503: cmake: command not found
opticks-externals-info
============================

    opticks-cmake-version  : 


opticks-locations
==================

      opticks-source   :   /home/opc/opticks/opticks.bash
      opticks-home     :   /home/opc/opticks
      opticks-name     :   opticks

      opticks-fold     :   /home/opc/opticks_build/opticks

      opticks-sdir     :   /home/opc/opticks
      opticks-idir     :   /home/opc/opticks_build/opticks
      opticks-bdir     :   /home/opc/opticks_build/opticks/build
      opticks-xdir     :   /home/opc/opticks_build/opticks/externals
      ## cd to these with opticks-scd/icd/bcd/xcd

      opticks-installcachedir   :  /home/opc/opticks_build/opticks/installcache
      opticks-bindir            :  /home/opc/opticks_build/opticks/lib
      opticks-optix-install-dir :  /home/opc/opticks_build/opticks/externals/OptiX


       uname   : Linux bristollz 3.10.0-957.21.3.el7.x86_64 #1 SMP Tue Jun 18 16:35:19 UTC 2019 x86_64 x86_64 x86_64 GNU/Linux
       HOME    : /home/opc
       VERBOSE : 
       USER    : opc

OPTICKS_HOME=/home/opc/opticks
opticks-externals-url
                           bcm :  http://github.com/simoncblyth/bcm.git 
                           glm :  https://github.com/g-truc/glm/releases/download/0.9.9.5/glm-0.9.9.5.zip 
                          glfw :  http://downloads.sourceforge.net/project/glfw/glfw/3.1.1/glfw-3.1.1.zip 
/home/opc/opticks/externals/glew.bash: line 65: opticks-cmake-generator: command not found
                          glew :  http://downloads.sourceforge.net/project/glew/glew/1.13.0/glew-1.13.0.zip 
                          gleq :  https://github.com/simoncblyth/gleq 
                         imgui :  http://github.com/simoncblyth/imgui.git 
                        assimp :  http://github.com/simoncblyth/assimp.git 
                      openmesh :  http://www.openmesh.org/media/Releases/6.3/OpenMesh-6.3.tar.gz 
                          plog :  https://github.com/simoncblyth/plog 
                   opticksdata :  http://bitbucket.org/simoncblyth/opticksdata 
               oimplicitmesher :  https://bitbucket.com/simoncblyth/ImplicitMesher 
                          odcs :  https://github.com/simoncblyth/DualContouringSample 
                      oyoctogl :  https://github.com/simoncblyth/yocto-gl 
                       ocsgbsp :  https://github.com/simoncblyth/csgjs-cpp 
                       xercesc :  http://archive.apache.org/dist/xerces/c/3/sources/xerces-c-3.1.1.tar.gz 
                            g4 :  http://geant4-data.web.cern.ch/geant4-data/releases/geant4.10.04.p02.tar.gz 
opticks-externals-dist
                           bcm :   
                           glm :  /home/opc/opticks_build/opticks/externals/glm/glm-0.9.9.5.zip 
                          glfw :  /home/opc/opticks_build/opticks/externals/glfw/glfw-3.1.1.zip 
                          glew :  /home/opc/opticks_build/opticks/externals/glew/glew-1.13.0.zip 
                          gleq :   
                         imgui :   
                        assimp :   
                      openmesh :  /home/opc/opticks_build/opticks/externals/openmesh/OpenMesh-6.3.tar.gz 
                          plog :   
                   opticksdata :   
               oimplicitmesher :   
                          odcs :   
                      oyoctogl :   
                       ocsgbsp :   
                       xercesc :  xerces-c-3.1.1.tar.gz 
                            g4 :  /home/opc/opticks_build/opticks/externals/g4/geant4.10.04.p02.tar.gz 
opticks-optionals-url
opticks-optionals-dist
```

Now install cmake.
Version 3.12+ is recommended for this, so remove older cmake is needed.
```code
# Run precursor functions
[opc@bristollz ~]$ opticks-
[opc@bristollz ~]$ ocmake-

# See what version ocmake will install (this can also be done by looking in the bash function by ocmake-vi
[opc@bristollz ~]$ ocmake-info
ocmake-info
============

ocmake-vers : 3.14.1
ocmake-nam  : cmake-3.14.1
ocmake-url  : https://github.com/Kitware/CMake/releases/download/v3.14.1/cmake-3.14.1.tar.gz
ocmake-dir  : /home/opc/opticks_build/opticks/externals/cmake/cmake-3.14.1

# Install cmake
[opc@bristollz ~]$ ocmake--

## If this built in installer fails, we can do it ourselves (not this is outdated) and also for ubuntu
ubuntu@bristollz:~$ mkdir software
ubuntu@bristollz:~$ cd software/
ubuntu@bristollz:~/software$ git clone https://gitlab.kitware.com/cmake/cmake.git
ubuntu@bristollz:~/software$ cd cmake/
ubuntu@bristollz:~/software/cmake$ git checkout tags/<version>
ubuntu@bristollz:~/software/cmake$ ./bootstrap && make && sudo make install
```

Now check cmake is installed correctly
```code
# Check cmake is default
[opc@bristollz cmake-3.14.1]$ which cmake
~/bin/cmake
[opc@bristollz cmake-3.14.1]$ cmake --version
cmake version 3.14.1

CMake suite maintained and supported by Kitware (kitware.com/cmake).

# Check cmake is using opticks
[opc@bristollz cmake-3.14.1]$ opticks-info
opticks-externals-info
============================

    opticks-cmake-version  : 3.14.1
```

Now get boost.
May need opticks-cmake-generator in custom_opticks_setup for this (or may be able to exclude it from everything)
```code
[opc@bristollz cmake-3.14.1]$ opticks-
[opc@bristollz cmake-3.14.1]$ boost-
[opc@bristollz cmake-3.14.1]$ boost--

# Or if that fails (which it shouldn't)
# AGAIN to update
ubuntu@bristollz:~$ sudo apt-get install libboost-all-dev
```
### Install Opticks externals
Install opticks externals
```code
# These are the externals which will be installed
[opc@bristollz ~]$ opticks-externals
bcm
glm
glfw
glew
gleq
imgui
assimp
openmesh
plog
opticksaux
oimplicitmesher
odcs
oyoctogl
ocsgbsp
xercesc
g4

[opc@bristollz ~]$ opticks-optionals-install # don't think this actually does anything but opticks docs says do it
opticks-optionals-install
=== -opticks-installer : START Tue 23 Jul 11:10:53 GMT 2019
=== -opticks-installer : DONE Tue 23 Jul 11:10:53 GMT 2019

# In custom_opticks_setup.sh add opticks-cmake-generator
# Resource custom_opticks_setup.sh after
opticks-cmake-generator(){ echo ${OPTICKS_CMAKE_GENERATOR:-Unix Makefiles} ; }

# For g4, add OPTICKS_INSTALL_PREFIX to custom_opticks_setup
# It's best to set this to whatever g4-; g4-prefix gives
#[opc@bristollz ~]$ g4-; g4-prefix
#/home/opc/opticks/externals without the externals
export OPTICKS_INSTALL_PREFIX=/home/opc/opticks
#/externals

# Things needed to install externals
# For bcm, gleq, assimp, plog, odcs, oyoctogl, ocsgbsp, 
[opc@bristollz ~]$ sudo yum install -y git

# I think for glfw and something else
[opc@bristollz ~]$ sudo yum install -y libXcursor-devel libXinerama-devel libXrandr-devel

# No idea... just trying things now to get it working
[opc@bristollz ~]$ sudo yum install -y mesa-dri-drivers mesa-libGLES mesa-libGLw mesa-libGLw-devel

# Fuck it, just install some other things too
[opc@bristollz ~]$ sudo yum install -y freeglut-devel freeglut expat-devel

# Could also get doxygen
[opc@bristollz ~]$ sudo yum install -y doxygen

# edit FindOpticksGLEW.cmake to search in lib64 (for imgui to work)
# /home/opc/opticks/cmake/Modules/FindOpticksGLEW.cmake line 20 to
# PATHS ${OpticksGLEW_PREFIX}/lib64)

# Change to openmesh 7.1?
#/home/opc/opticks/externals/openmesh.bash line 1093

# Now install externals (will take a long time, G4 make will take ~1 hour)
# For when they go wrong do -wipe, eg glfw-wipe then glfw-- (which does glfw-get, glfw-cmake, glfw-make install)
[opc@bristollz ~]$ opticks-externals-install 2>&1 | tee opticks_build_logs/externals_install.log
```
NOTE: g4-- does not correctly write the bash file, there is an extra /external/ in the path, easy to do manually though
Faliure is:
```
=== g4-envg4 : writing script opticks-envg4.bash to externals /home/opc/opticks/externals/externals/opticks-envg4.bash
-bash: /home/opc/opticks/externals/externals/opticks-envg4.bash: No such file or directory
cat: /home/opc/opticks/externals/externals/opticks-envg4.bash: No such file or directory
=== opticks-ext-installer : RC 1 from ext g4 : ABORTING
```

### Install Opticks-full
Now to prepare of opticks-full. 
```code
# opticks-full trys to install the following
[opc@bristollz opticks]$ om-
[opc@bristollz opticks]$ om-subs
okconf
sysrap
boostrap
npy
yoctoglrap
optickscore
ggeo
assimprap
openmeshrap
opticksgeo
cudarap
thrustrap
optixrap
okop
oglrap
opticksgl
ok
extg4
cfg4
okg4
g4ok
integration
ana
analytic
bin
```

To get these working, we require several things

To install:
```code
# Get dev version of openssl for sysrap
[opc@bristollz ~]$ sudo yum install -y openssl-devel
[opc@bristollz ~]$ sudo yum install -y python-pip
[opc@bristollz ~]$ sudo pip install numpy
```

Edits to code
```code
#To fix optixrap error, edit /home/opc/opticks/cmake/Modules/FindOptiX.cmake to contain on line 41
if (DEFINED ENV{OptiX_INSTALL_DIR})
        set(OptiX_INSTALL_DIR $ENV{OptiX_INSTALL_DIR})
endif()
```
Add OptiX_INSTALL_DIR to custom_opticks_setup.sh
```code
export OptiX_INSTALL_DIR=/home/opc/OptiX/NVIDIA-OptiX-SDK-6.0.0-linux64
```

Add compilation flags to custom_opticks_setup.sh
```code
export CXXFLAGS="$CXXFLAGS -fPIC"
export CFLAGS="$CFLAGS -fPIC"
```

We must also set the compute capability.
Not setting this will cause OKConf and sysrap tests to fail as it's default is 0.
The compute capability of the GPU can be found at https://developer.nvidia.com/cuda-gpus.
6.1 = 61, 6.0 = 60 etc...
Add the following line to the setup script...
```code
export OPTICKS_COMPUTE_CAPABILITY=60
```
Now install opticks
```code
opticks-full
```

imgui and glfw seem to cause failures atm
to solve:
```code
glfw-; glfw-cmake; glfw-config; glfw-configure; glfw--
imgui-; imgui-configure; imgui--
```
then run
```code
opticks-full
```

Now prepare cudarap
```code
cudarap-
cudarap-prepare-installation
```


NOTE: opticks-prepare-installation will fail. This looks like a test Simon is doing for different GPUs
NOTE: cudarap- also didn't work correctly, which means `op.sh -G` also wont work. So we cannot go any further than this now
```
=== opticks-prepare-installation : generating RNG seeds into installcache
-bash: cudarap-prepare-rng: command not found
-bash: cudarap-check-rng: command not found
=== opticks-full : DONE Wed 30 Oct 16:41:16 GMT 2019
```


### Verify opticks
Prepare to run opticks tests.
We need the following libraries
```code
sudo yum install -y python-devel python-pip ipython
sudo yum install libcanberra-gtk2 # for tboolean
```

NOTE: Require numpy 1.14+ (1.16.4 works)

Opticks no longer auto downloads the test geometries.
To download, run opticksdata
```
opticksdata-
opticksdata--
```


Set up geometry
```code
op.sh -G
geocache-
geocache-create
op.sh
```

Add the OPTICKS_KEY output from geocache-create to custom_opticks_setup.sh eg:
```code
export OPTICKS_KEY=OKX4Test.X4PhysicalVolume.lWorld0x4bc2710_PV.f6cc352e44243f8fa536ab483ad390ce
```

Run opticks tests
```code
opticks-t
```

# Still three tests failing!



## Using custom geometry
Comment out the OPTICKS_KEY as this refers to the wrong geometry.

Now we must convert this into GPU compatable numpy.
We should be able to do this via `geocache-` functions, however for testing purposes we can skip the middle man and just do ...
` o.sh --okx4 --g4codegen --deletegeocache --gdmlpath /home/opc/LZ/lz_geometry.gdml -D`.
THe `-D` is for debug.

This just runs `OKX4Test --g4codegen --deletegeocache --gdmlpath /home/opc/LZ/lz_geometry.gdml -D` so this can be done instead

After this, an OPTICKS_KEY will be push out. Add this to `custom_opticks_setup.sh` to use it.

### Debugging
Generally running opticks things with `-D` argument.

For proper debugging, install...
```code
debuginfo-install expat-2.1.0-10.el7_3.x86_64 \
                  glibc-2.17-260.el7_6.6.x86_64 \
                  keyutils-libs-1.5.8-3.el7.x86_64 \
                  krb5-libs-1.15.1-37.el7_6.x86_64 \
                  libX11-1.6.5-2.el7.x86_64 \
                  libXau-1.0.8-2.1.el7.x86_64 \
                  libXcursor-1.1.15-1.el7.x86_64 \
                  libXext-1.3.3-3.el7.x86_64 \
                  libXfixes-5.0.3-1.el7.x86_64 \
                  libXi-1.7.9-1.el7.x86_64 \
                  libXinerama-1.1.3-2.1.el7.x86_64 \
                  libXrandr-1.5.1-2.el7.x86_64 \
                  libXrender-0.9.10-1.el7.x86_64 \
                  libXxf86vm-1.1.4-1.el7.x86_64 \
                  libcom_err-1.42.9-13.el7.x86_64 \
                  libgcc-4.8.5-36.el7_6.2.x86_64 \
                  libglvnd-1.0.1-0.8.git5baa1e5.el7.x86_64 \
                  libglvnd-glx-1.0.1-0.8.git5baa1e5.el7.x86_64 \
                  libselinux-2.5-14.1.el7.x86_64 \
                  libstdc++-4.8.5-36.el7_6.2.x86_64 \
                  libxcb-1.13-1.el7.x86_64 \
                  openssl-libs-1.0.2k-16.el7_6.1.x86_64 \
                  pcre-8.32-17.el7.x86_64 \
                  zlib-1.2.7-18.el7.x86_64
```

debuginfo-install krb5-libs-1.15.1-37.el7_7.2.x86_64 libcom_err-1.42.9-16.el7.x86_64 openssl-libs-1.0.2k-19.el7.x86_64

OKX4Test happens, `.opticks/geocache/...` will fill with the solids of the geometry.
If a solid is causing a failure to the conversion, you can see it there.

Once OKX4Test completes, the directory contains 

## Using custom geometry