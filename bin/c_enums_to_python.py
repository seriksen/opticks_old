#!/usr/bin/env python
"""
Usage example::

    c_enums_to_python.py boolean-solid.h > boolean_solid_h.py 

"""
import sys, datetime, os

def c_enums_to_python(eraw):
   etxt = eraw[eraw.index("{"):eraw.index("}")+1]
   elin = "\n".join(map(lambda l:l.strip().rstrip().replace(",",""),etxt[1:-1].split("\n"))) 
   return elin

if __name__ == '__main__':
   assert len(sys.argv) > 1

   hdr = sys.argv[1]

   txt = file(hdr).read()
   eraws = txt.split("enum")[1:5]

   now = datetime.datetime.now().strftime("%c")
   print "# generated %s " % (now)
   print "# from %s " % os.getcwd()
   print "# with command :  %s %s " % (sys.argv[0], hdr)
   print "\n\n".join(map(c_enums_to_python, eraws))

   



 

