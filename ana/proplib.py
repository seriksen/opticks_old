#!/usr/bin/env python
"""
PropLib : Geocache Access
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*PropLib* provides access to optical properties of materials and surfaces 
as a function of wavelength.

Material properties::

    REFRACTIVE_INDEX 
    ABSORPTION_LENGTH 
    SCATTERING_LENGTH 
    REEMISSION_PROB 

Surface properties::

    DETECT 
    ABSORB 
    REFLECT_SPECULAR 
    REFLECT_DIFFUSE 


Example data shapes::

    In [5]: mlib.data.shape
    Out[5]: (38, 2, 39, 4)

    In [6]: slib.data.shape
    Out[6]: (48, 2, 39, 4)

    In [7]: blib.data.shape
    Out[7]: (123, 4, 2, 39, 4)


"""
import os, logging, numpy as np

idp_ = lambda _:os.path.expandvars("$IDPATH/%s" % _ )

# 1st set of 4 [0] 
M_REFRACTIVE_INDEX = 0
M_ABSORPTION_LENGTH = 1
M_SCATTERING_LENGTH = 2
M_REEMISSION_PROB = 3

# 2nd set of 4 [0] startswith GROUPVEL, currently not used

S_DETECT = 0 
S_ABSORB = 1 
S_REFLECT_SPECULAR = 2 
S_REFLECT_DIFFUSE = 3 

B_OMAT = 0
B_OSUR = 1
B_ISUR = 2
B_IMAT = 3


class PropLib(object):
    def __init__(self, kls="GMaterialLib"):
        fpath=idp_("%(kls)s/%(kls)s.npy" % locals())
        npath=idp_("GItemList/%(kls)s.txt" % locals())
        data = np.load(fpath)
        names = map(lambda _:_[:-1],file(npath).readlines())
        assert len(names) == data.shape[0]
        pass
        self.data = data
        self.names = names
        self.domain = np.linspace(60.,810., 39)

    def index(self, name):
        return self.names.index(name)

    def interp(self, name, wavelengths, k=0):
        idx = self.names.index(name)
        return np.interp( wavelengths, self.domain, self.data[idx][0][:,k] ) 
 
    def __call__(self, name):
        idx = self.names.index(name)
        return self.data[idx]
   

class Material(object):
    def __init__(self, name):
        self.name = name
        self.mlib = None

    def lookup(self, prop, wavelength):
        if self.mlib is None:
            self.mlib = PropLib("GMaterialLib")
        pass
        return self.mlib.interp(self.name,wavelength,prop)
 
    def refractive_index(self, wavelength):
        return self.lookup(M_REFRACTIVE_INDEX, wavelength)


class Boundary(object):
    def __init__(self, spec):
        elem = spec.split("/")
        assert len(elem) == 4
        omat, osur, isur, imat = elem

        self.omat = Material(omat)
        self.osur = osur
        self.isur = isur
        self.imat = Material(imat)
        self.spec = spec

    def title(self):
        return self.spec

    def __repr__(self):
        return "%s %s " % (  self.__class__.__name__ , self.spec )



if __name__ == '__main__':

    logging.basicConfig(level=logging.INFO)

    mlib = PropLib("GMaterialLib") 
    slib = PropLib("GSurfaceLib") 
    blib = PropLib("GBndLib") 

    idx = mlib.index("Water")

    m1 = "Water"
    wavelength = 442  
    ri = mlib.interp(m1, wavelength, M_REFRACTIVE_INDEX) 

    print "m1 %s wl %s ri %s " % (m1, wavelength, ri)



    mat = Material("GlassSchottF2")
    wl = np.arange(10, dtype=np.float32)*70. + 100.
    ri = mat.refractive_index(wl)
    print wl,"\n", ri


