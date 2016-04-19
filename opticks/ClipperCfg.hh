#pragma once
#include "Cfg.hh"

template <class Listener>
class ClipperCfg : public Cfg {
public:
   ClipperCfg(const char* name, Listener* listener, bool live) : Cfg(name, live) 
   {
       addOptionI<Listener>(listener, Listener::CUTPRINT, "Print debug info, when 1 is provided  ");
       addOptionI<Listener>(listener, Listener::CUTMODE,  "Integer cutmode control, -1 for disabled  ");
       addOptionS<Listener>(listener, Listener::CUTPOINT, "Comma delimited x,y,z clipping plane point in the plane in model-extent coordinates, eg 0,0,0  ");
       addOptionS<Listener>(listener, Listener::CUTNORMAL,"Comma delimited x,y,z clipping plane normal to the plane in model-extent coordinates, eg 1,0,0  ");
       addOptionS<Listener>(listener, Listener::CUTPLANE, "Comma delimited x,y,z,w world frame plane equation eg 1,0,0,16520  ");
   }
};


