#!/usr/bin/env python
#
# Copyright (c) 2019 Opticks Team. All Rights Reserved.
#
# This file is part of Opticks
# (see https://bitbucket.org/simoncblyth/opticks).
#
# Licensed under the Apache License, Version 2.0 (the "License"); 
# you may not use this file except in compliance with the License.  
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
# See the License for the specific language governing permissions and 
# limitations under the License.
#

"""
enum.py
=================

Canonical usage is for parsing OpticksPhoton.h 
which is done from custom commands in optickscore/CMakeLists.txt

"""
import os, re, logging, argparse
log = logging.getLogger(__name__)

class Enum(dict):
    lptn = re.compile("^\s*(\w+)\s*=\s*(.*?),*\s*?$")
    vptn = re.compile("^0x1 <<\s*(\d+)$")
    def __init__(self, path, mskval=True):
        dict.__init__(self)
        log.info("parsing %s " % path )
        path = os.path.expandvars(path)
        self.parse(path, mskval=mskval)

    def __repr__(self):
        return "\n".join([" %-2d : %s " % (kv[1], kv[0] ) for kv in sorted(self.items(), key=lambda kv:kv[1])]) 

    def _get_ini(self):
        return "\n".join(["%s=%s" % (kv[0], kv[1] ) for kv in sorted(self.items(), key=lambda kv:kv[1])]) 
    ini = property(_get_ini)

    def parse(self, path, mskval=True):
        lines = map(str.strip,file(path).readlines())
        for line in lines:
            lm = self.lptn.match(line)
            if lm:
                 lg = lm.groups()
                 assert len(lg) == 2 
                 label, val = lg

                 vm = self.vptn.match(val)
                 assert vm 
                 vg = vm.groups()
                 assert len(vg) == 1
                 n = int(vg[0])

                 emsk = eval(val)
                 msk = 0x1 << n 
                 assert emsk == msk

                 log.debug( "%-40s     ==> [%s]    [%s]  ==> [%d] ==> [%x]  " % (line, label, val, n, msk) )

                 self[label] = msk if mskval else n + 1 


if __name__ == '__main__':

    #default_path = "$OPTICKS_HOME/optickscore/OpticksPhoton.h"
    default_path = "$OPTICKS_INSTALL_PREFIX/include/OpticksCore/OpticksPhoton.h"
    parser = argparse.ArgumentParser(__doc__)
    parser.add_argument(     "path",  nargs="?", help="Path to input header", default=default_path )
    parser.add_argument(     "--inipath", default=None, help="When a path is provided an ini file will be written to it." ) 
    parser.add_argument(     "--quiet", action="store_true", default=False, help="Skip dumping" ) 
    parser.add_argument(     "--mskval", action="store_true", default=False, help="Store the mask value rather than the smaller power of two int" ) 
    parser.add_argument(     "--level", default="info", help="logging level" ) 
    args = parser.parse_args()

    fmt = '[%(asctime)s] p%(process)s {%(pathname)s:%(lineno)d} %(levelname)s - %(message)s'
    logging.basicConfig(level=getattr(logging,args.level.upper()), format=fmt)

    if args.path == default_path:
        log.info("using default path %s " % args.path)
    pass  

    d = Enum(args.path, mskval=args.mskval)

    if not args.quiet:
        print(d)
        print(d.ini)
    pass
    if not args.inipath is None:
        log.info("writing ini to inipath %s " % args.inipath)
        file(args.inipath, "w").write(d.ini)
    pass 




      

