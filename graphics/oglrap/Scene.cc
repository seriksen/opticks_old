#include "Scene.hh"

#include <GL/glew.h>
#include "string.h"

// ggeo-
#include "GDrawable.hh"


// oglrap-
#include "Composition.hh"
#include "Renderer.hh"
#include "Device.hh"
#include "Rdr.hh"
#include "Colors.hh"

// npy-
#include "NumpyEvt.hpp"
#include "ViewNPY.hpp"
#include "MultiViewNPY.hpp"

#include "GLMPrint.hpp"
#include "GLMFormat.hpp"

#ifdef GUI_
#include <imgui.h>
#endif


#include <boost/lexical_cast.hpp>
#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL
// trace/debug/info/warning/error/fatal



const char* Scene::AXIS   = "axis" ; 
const char* Scene::PHOTON = "photon" ; 
const char* Scene::GENSTEP = "genstep" ; 
const char* Scene::GEOMETRY = "geometry" ; 
const char* Scene::RECORD   = "record" ; 

const char* Scene::REC_ = "point" ;   
const char* Scene::ALTREC_ = "line" ; 
const char* Scene::DEVREC_ = "vector" ; 

const char* Scene::getRecordStyleName(Scene::RecordStyle_t style)
{
   switch(style)
   {
      case    REC:return REC_ ; break; 
      case ALTREC:return ALTREC_ ; break; 
      case DEVREC:return DEVREC_ ; break; 
      case NUMSTYLE:assert(0) ; break ; 
      default: assert(0); break ; 
   } 
   return NULL ; 
}

const char* Scene::getRecordStyleName()
{
   return getRecordStyleName(getRecordStyle());
}
 

void Scene::gui()
{
#ifdef GUI_
     ImGui::Checkbox(GEOMETRY, &m_geometry_mode);

     ImGui::Checkbox(AXIS,     &m_axis_mode);
     ImGui::Checkbox(GENSTEP,  &m_genstep_mode);
     ImGui::Checkbox(PHOTON,   &m_photon_mode);
     ImGui::Checkbox(RECORD,   &m_record_mode);
     ImGui::Text(" target: %u ", m_target );
     ImGui::Text(" genstep %d photon %d record %d \n", 
             m_genstep_renderer->getCountDefault(),
             m_photon_renderer->getCountDefault(),
             m_record_renderer->getCountDefault()
     );



     int* record_style = (int*)&m_record_style ;       // address of enum cast to int*
     ImGui::RadioButton("rec",    record_style, REC); 
     ImGui::SameLine();
     ImGui::RadioButton("altrec", record_style, ALTREC); 
     ImGui::SameLine();
     ImGui::RadioButton("devrec", record_style, DEVREC); 


#endif    
}

const char* Scene::TARGET = "target" ; 

bool Scene::accepts(const char* name)
{
    return 
          strcmp(name, TARGET) == 0  ;
}  

std::vector<std::string> Scene::getTags()
{
    std::vector<std::string> tags ;
    tags.push_back(TARGET);
    return tags ; 
}


std::string Scene::get(const char* name)
{
    int v(0) ; 
    if(     strcmp(name,TARGET)==0) v = getTarget();
    else
         printf("Scene::get bad name %s\n", name);

    return gformat(v);
}

void Scene::set(const char* name, std::string& s)
{
    int v = gint_(s); 
    if(     strcmp(name,TARGET)==0)    setTarget(v);
    else
         printf("Scene::set bad name %s\n", name);
}

void Scene::configure(const char* name, const char* value_)
{
    std::string val(value_);
    int value = gint_(val); 
    configure(name, value);
}

void Scene::configureI(const char* name, std::vector<int> values)
{
    LOG(info) << "Scene::configureI";
    if(values.empty()) return ;
    int last = values.back();
    configure(name, last);
}

void Scene::configure(const char* name, int value)
{
    if(strcmp(name, TARGET) == 0)
    {
        setTarget(value);   
    }
    else
    {
        LOG(warning)<<"Scene::configure ignoring " << name << " " << value ;
    }
}




void Scene::init()
{
    LOG(info) << "Scene::init ";

    m_device = new Device();

    m_colors = new Colors(m_device);

    //LOG(info) << "Scene::init geometry_renderer ctor";
    m_geometry_renderer = new Renderer("nrm", m_shader_dir, m_shader_incl_path );
    //LOG(info) << "Scene::init geometry_renderer ctor DONE";

    m_axis_renderer = new Rdr(m_device, "axis", m_shader_dir, m_shader_incl_path );

    m_genstep_renderer = new Rdr(m_device, "p2l", m_shader_dir, m_shader_incl_path);

    m_photon_renderer = new Rdr(m_device, "pos", m_shader_dir, m_shader_incl_path );


    //
    // RECORD RENDERING USES AN UNPARTIONED BUFFER OF ALL RECORDS
    // SO THE GEOMETRY SHADERS HAVE TO THROW INVALID STEPS AS DETERMINED BY
    // COMPARING THE TIMES OF THE STEP PAIRS  
    // THIS MEANS SINGLE VALID STEPS WOULD BE IGNORED..
    // THUS MUST SUPPLY LINE_STRIP SO GEOMETRY SHADER CAN GET TO SEE EACH VALID
    // VERTEX IN A PAIR
    //
    // OTHERWISE WILL MISS STEPS
    //
    //  see explanations in gl/altrec/geom.glsl
    //
    m_record_renderer = new Rdr(m_device, "rec", m_shader_dir, m_shader_incl_path );
    m_record_renderer->setPrimitive(Rdr::LINE_STRIP);

    m_altrecord_renderer = new Rdr(m_device, "altrec", m_shader_dir, m_shader_incl_path);
    m_altrecord_renderer->setPrimitive(Rdr::LINE_STRIP);

    m_devrecord_renderer = new Rdr(m_device, "devrec", m_shader_dir, m_shader_incl_path);
    m_devrecord_renderer->setPrimitive(Rdr::LINE_STRIP);

    m_initialized = true ; 
}

void Scene::setComposition(Composition* composition)
{
    m_composition = composition ; 
    m_geometry_renderer->setComposition(composition);
    m_axis_renderer->setComposition(composition);
    m_genstep_renderer->setComposition(composition);
    m_photon_renderer->setComposition(composition);
    m_record_renderer->setComposition(composition);
    m_altrecord_renderer->setComposition(composition);
    m_devrecord_renderer->setComposition(composition);
}


void Scene::setGeometry(GDrawable* geometry)
{
    m_geometry = geometry ;
    m_geometry_renderer->setDrawable(m_geometry);  // upload would be better name than setDrawable
}

void Scene::uploadColorBuffer(GBuffer* colorbuffer)
{
    m_colorbuffer = colorbuffer ; 
    m_colors->setColorBuffer(colorbuffer);
    m_colors->upload();
}

Rdr* Scene::getRecordRenderer()
{
    return getRecordRenderer(m_record_style);
}

Rdr* Scene::getRecordRenderer(RecordStyle_t style)
{
    Rdr* rdr = NULL ; 
    switch(style)
    {
        case      REC:rdr = m_record_renderer     ; break ;
        case   ALTREC:rdr = m_altrecord_renderer  ; break ;
        case   DEVREC:rdr = m_devrecord_renderer  ; break ;
        case NUMSTYLE:                            ; break ;
    }
    return rdr ; 
}



void Scene::uploadAxis()
{
    LOG(info) << "Scene::uploadAxis  " ;
    m_axis_renderer->upload(m_composition->getAxisAttr());
}


void Scene::uploadEvt()
{
    if(!m_evt) 
    {
       LOG(fatal) << "Scene::uploadEvt no evt " ;
       assert(m_evt);
    }

    m_genstep_renderer->upload(m_evt->getGenstepAttr());

    m_photon_renderer->upload(m_evt->getPhotonAttr());
    m_photon_renderer->upload(m_evt->getSequenceAttr());
    m_photon_renderer->upload(m_evt->getPhoselAttr());

    uploadRecordAttr(m_evt->getRecordAttr());
}


void Scene::uploadSelection()
{
    // this was used after the slow SequenceNPY (CPU side std::map based photon history/material indexing)
    // following move to thrust is not done just after uploadEvt
    assert(m_evt);
    LOG(info)<<"Scene::uploadSelection";
    uploadRecordAttr(m_evt->getRecselAttr()); 
}


void Scene::uploadRecordAttr(MultiViewNPY* attr)
{
    if(!attr) return ;  
    //assert(attr);

    // all renderers ready to roll so can live switch between them, 
    // data is not duplicated thanks to Device register of uploads

    m_record_renderer->upload(attr);
    m_altrecord_renderer->upload(attr);
    m_devrecord_renderer->upload(attr);
}

void Scene::render()
{

    if(m_geometry_mode) m_geometry_renderer->render();
    if(m_axis_mode)     m_axis_renderer->render();
    if(m_genstep_mode)  m_genstep_renderer->render();  
    if(m_photon_mode)   m_photon_renderer->render();
    if(m_record_mode)
    {
        Rdr* rdr = getRecordRenderer();
        assert(rdr);
        rdr->render();
    }
}



unsigned int Scene::touch(int ix, int iy, float depth)
{
    glm::vec3 t = m_composition->unProject(ix,iy, depth);
    gfloat3 gt(t.x, t.y, t.z );

    unsigned int container = m_geometry->findContainer(gt);
    LOG(debug)<<"Scene::touch " 
             << " x " << t.x 
             << " y " << t.y 
             << " z " << t.z 
             << " container " << container
             ;

   if(container > 0) setTouch(container);
   return container ; 
}



void Scene::jump()
{
    if( m_touch > 0 && m_touch != m_target )
    {
        LOG(info)<<"Scene::jump-ing from  m_target -> m_touch  " << m_target << " -> " << m_touch  ;  
        setTarget(m_touch);
    }
}


void Scene::setTarget(unsigned int index, bool autocam)
{
    m_target = index ; 

    if( m_geometry == NULL )
    {
        LOG(fatal)<<"Scene::setTarget " << index << " finds no geometry : cannot set target  " ; 
        return ;  
    }

    gfloat4 ce = m_geometry->getCenterExtent(index);

    LOG(info)<<"Scene::setTarget " << index << " ce " 
             << " " << ce.x 
             << " " << ce.y 
             << " " << ce.z 
             << " " << ce.w 
             << " autocam " << autocam 
             ;

    m_composition->setCenterExtent(ce, autocam); 
}





