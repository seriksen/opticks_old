#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>  

// bcfg-
class Cfg ;

// npy-
template<typename T> class NPY ; 
class MultiViewNPY ; 
class NState ; 
class NumpyEvt ; 
#include "NConfigurable.hpp"

// opticks-
class Camera ;
class OrbitalView ; 
class TrackView ; 
class Light ;
class Trackball ; 
class Clipper ; 
class Animator ; 
class Bookmarks ; 
#include "View.hh"

// ggeo-
class GGeo ; 

// oglrap-


class Composition : public NConfigurable {
   public:
      friend class GUI ; 
   public:
      static const char* PREFIX ;
      const char* getPrefix();
   public:
      // see CompositionCfg.hh
      static const char* PRINT ;  
      static const char* SELECT ;  
      static const char* RECSELECT ;  
      static const char* PICKPHOTON ;  
      static const char* PICKFACE ;  
      static const char* EYEW ; 
      static const char* LOOKW ; 
      static const char* UPW ; 

      friend class Interactor ;   
      friend class Bookmarks ;   
  public:
      static const char* WHITE_ ; 
      static const char* MAT1_ ; 
      static const char* MAT2_ ; 
      static const char* FLAG1_ ; 
      static const char* FLAG2_ ; 
      static const char* POL1_ ; 
      static const char* POL2_ ; 
  public:
      static const char* DEF_GEOMETRY_ ; 
      static const char* NRMCOL_GEOMETRY_ ; 
      static const char* VTXCOL_GEOMETRY_ ; 
      static const char* FACECOL_GEOMETRY_ ; 
  public:
      static const glm::vec3 X ; 
      static const glm::vec3 Y ; 
      static const glm::vec3 Z ; 
  public:
      Composition();
      void addConstituentConfigurables(NState* state);
      virtual ~Composition();
   public:
      void setAnimatorPeriod(int period);
      Animator* getAnimator();
      void nextAnimatorMode(unsigned int modifiers);
      void nextRotatorMode(unsigned int modifiers);
      void nextViewMode(unsigned int modifiers);
      void changeView(unsigned int modifiers);
      unsigned int tick();
      unsigned int getCount();
   private:
      void swapView();
      void initAnimator();
      void initRotator();
   public:
      void setOrbitalViewPeriod(int ovperiod);
      void setTrackViewPeriod(int tvperiod);
      void setTrack(NPY<float>* track);
      OrbitalView* makeOrbitalView();
      TrackView* makeTrackView();
   public:
       void scrub_to(float x, float y, float dx, float dy); // Interactor:K scrub_mode
   public:
      void nextViewType(unsigned int modifiers);
      void setViewType(View::View_t type);
      View::View_t getViewType();
   private:
       void applyViewType();
   public:
       typedef enum { WHITE, MAT1, MAT2, FLAG1, FLAG2, POL1, POL2, NUM_COLOR_STYLE } ColorStyle_t ;
       static const char* getColorStyleName(Composition::ColorStyle_t style);
       const char* getColorStyleName();
       void nextColorStyle();
       void setColorStyle(Composition::ColorStyle_t style);
       Composition::ColorStyle_t getColorStyle();
   public:
       typedef enum { DEF_NORMAL, FLIP_NORMAL, NUM_NORMAL_STYLE } NormalStyle_t ;
       void nextNormalStyle();
       void setNormalStyle(Composition::NormalStyle_t style);
       Composition::NormalStyle_t getNormalStyle();
   public:
       typedef enum { DEF_GEOMETRY, NRMCOL_GEOMETRY, VTXCOL_GEOMETRY, FACECOL_GEOMETRY, NUM_GEOMETRY_STYLE } GeometryStyle_t ;
       static const char* getGeometryStyleName(Composition::GeometryStyle_t style);
       const char* getGeometryStyleName();
       void nextGeometryStyle();   // changes m_nrmparam
       void setGeometryStyle(Composition::GeometryStyle_t style);
       Composition::GeometryStyle_t getGeometryStyle();
   public:
       typedef enum { SHOW, HIDE, NUM_PICKPHOTON_STYLE } PickPhotonStyle_t ;
       void nextPickPhotonStyle();
       void setPickPhotonStyle(Composition::PickPhotonStyle_t style);
       Composition::PickPhotonStyle_t getPickPhotonStyle();
  private:
      void init();
      void initAxis();
  public:
      // Configurable : for bookmarking 
      static bool accepts(const char* name);
      void configure(const char* name, const char* value);
      std::vector<std::string> getTags();
      void set(const char* name, std::string& s);
      std::string get(const char* name);

      // for cli/live updating 
      void configureI(const char* name, std::vector<int> values );
      void configureS(const char* name, std::vector<std::string> values);

  public: 
      void aim(glm::vec4& ce, bool verbose=false);
      void setCenterExtent(const glm::vec4& ce, bool aim=false); // effectively points at what you want to look at 
      void setFaceTarget(unsigned int face_index, unsigned int solid_index, unsigned int mesh_index);
      void setFaceRangeTarget(unsigned int face_index0, unsigned int face_index1, unsigned int solid_index, unsigned int mesh_index);
  public: 
      void setDomainCenterExtent(const glm::vec4& ce); // typically whole geometry domain
      void setColorDomain(const glm::uvec4& cd);
      void setTimeDomain(const glm::vec4& td);
  public:
      // avaiable as uniform inside shaders allowing GPU-side selections 
      void setSelection(glm::ivec4 sel);
      void setSelection(std::string sel);
      glm::ivec4& getSelection();

  public:
      void setRecSelect(glm::ivec4 sel);
      void setRecSelect(std::string sel);
      glm::ivec4& getRecSelect();

  public:
      void setPickPhoton(glm::ivec4 pp);
      void setPickPhoton(std::string pp);
      glm::ivec4& getPickPhoton();
      int* getPickPtr();
  public:
      void setPickFace(glm::ivec4 pf);
      void setPickFace(std::string pf);
      glm::ivec4& getPickFace();

  public:
      void setColorParam(glm::ivec4 cp);
      void setColorParam(std::string cp);
      glm::ivec4& getColorParam();
      int* getColorParamPtr();
  public:
      void setParam(glm::vec4 par);
      void setParam(std::string par);
      glm::vec4&  getParam();
      float*      getParamPtr();

  public:
      glm::vec4&  getScanParam();
      float*      getScanParamPtr();

  public:
      //void setNrmParam(glm::ivec4 par);
      //void setNrmParam(std::string par);
      glm::ivec4&  getNrmParam();
      int*         getNrmParamPtr();

  public:
      void setFlags(glm::ivec4 flags);
      void setFlags(std::string flags);
      glm::ivec4& getFlags();

  public:
      void setPick(glm::ivec4 pick);
      void setPick(std::string pick);
      glm::ivec4& getPick();
  public:
      void setEvt(NumpyEvt* evt);
      NumpyEvt* getEvt();
      void setGeometry(GGeo* ggeo);
      GGeo* getGeometry();
  public:
      void addConfig(Cfg* cfg);
  public:
      void setLookAngle(float phi);
      float* getLookAnglePtr();
  public:
      void setLookW(glm::vec4 lookw);
      void setLookW(std::string lookw);
  public:
      void setEyeW(glm::vec4 eyew);
      void setEyeW(std::string eyew);
  public:
      void setUpW(glm::vec4 upw);
      void setUpW(std::string upw);
  public:
      void setEyeGUI(glm::vec3 gui);
  public: 
      void home();
      void update();

  public: 
      void test_getEyeUVW();
      bool getParallel();
      void getEyeUVW(glm::vec3& eye, glm::vec3& U, glm::vec3& V, glm::vec3& W, glm::vec4& ZProj);
      void getEyeUVW_no_trackball(glm::vec3& eye, glm::vec3& U, glm::vec3& V, glm::vec3& W);
      void getLookAt(glm::mat4& lookat);
  public: 
      glm::vec3 getNDC(const glm::vec4& position_world);
      glm::vec3 getNDC2(const glm::vec4& position_world);
      float getNDCDepth(const glm::vec4& position_world);
      float getClipDepth(const glm::vec4& position_world);
  private: 
      // invoked from Interactor 
      void commitView();
  public: 
      // private getters of residents : usable by friend class

      Camera*    getCamera(); 
      Trackball* getTrackball(); 
      View*      getView(); 
      Light*     getLight(); 
      Clipper*   getClipper(); 


      
      void setCamera(Camera* camera);
      void setView(View* view);
      void resetView();
      void setBookmarks(Bookmarks* bookmarks);

  public: 
      // getters of inputs 
      glm::mat4& getModelToWorld();
      glm::mat4& getWorldToModel();
      float getExtent();
      float getNear();
      float getFar();
  public:
      // position of the observer "Viewpoint" and the observed "Lookpoint" using m_eye_to_world/m_world_to_eye
      glm::vec4 transformWorldToEye(const glm::vec4& world);
      glm::vec4 transformEyeToWorld(const glm::vec4& eye);
      glm::vec4 getLookpoint();
      glm::vec4 getViewpoint();
      glm::vec4 getUpdir();
  public: 
      unsigned int getWidth();
      unsigned int getHeight();
      unsigned int getPixelWidth(); // width*pixel_factor
      unsigned int getPixelHeight();
      unsigned int getPixelFactor();
      void setSize(unsigned int width, unsigned int height, unsigned int pixelfactor=1);
      void setSize(glm::uvec4 size); // x, y will be scaled down by the pixelfactor
      void setFramePosition(const glm::uvec4& position);
      glm::uvec4& getFramePosition();
  public:
      glm::vec3 unProject(unsigned int x, unsigned int y, float z);

  public:
      // getters of the derived properties : need to call update first to make them current
      glm::vec4& getGaze();
      glm::vec4& getCenterExtent();
      glm::vec4& getDomainCenterExtent();
      glm::vec4& getTimeDomain();
      glm::uvec4& getColorDomain();
      glm::vec4& getLightPosition();
      float*     getLightPositionPtr();
      glm::vec4& getLightDirection();
      float*     getLightDirectionPtr();
      float&     getGazeLength();
      glm::mat4& getWorld2Eye();  // ModelView  including trackballing
      float*     getWorld2EyePtr();  // ModelView  including trackballing
      glm::mat4& getEye2World();
      glm::mat4& getWorld2Camera();
      glm::mat4& getCamera2World();
      glm::mat4& getEye2Look();
      glm::mat4& getLook2Eye();
   public:
      std::string getEyeString();
      std::string getLookString();
      std::string getGazeString();
   public:
      bool hasChanged(); // based on View, Camera, Trackball
      bool hasChangedGeometry(); // excludes event animation
      void setChanged(bool changed);
   public:
      // ModelViewProjection including trackballing
      glm::mat4& getWorld2Clip();   
      float*     getWorld2ClipPtr();  
      glm::mat4& getDomainISNorm();
      glm::mat4& getWorld2ClipISNorm();      // with initial domain_isnorm
      float*     getWorld2ClipISNormPtr();   
      glm::mat4& getProjection(); 
      float*     getProjectionPtr();  
   public:
      float*     getIdentityPtr(); 

  private:
      glm::mat4& getTrackballing(); 
      glm::mat4& getITrackballing(); 

  public:
      int        getClipMode();
      glm::vec4& getClipPlane();
      float*     getClipPlanePtr();
  public:
      // analog to NumpyEvt for the axis
      void setAxisData(NPY<float>* axis_data);
      NPY<float>* getAxisData();
      MultiViewNPY* getAxisAttr();
      void dumpAxisData(const char* msg="Composition::dumpAxisData");

  private:
      // inputs 
      glm::mat4 m_model_to_world ; 
      glm::mat4 m_world_to_model ; 
      float     m_extent ; 
      glm::vec4 m_center_extent ; 
      glm::vec4 m_domain_center_extent ; 
      glm::mat4 m_domain_isnorm ;     
      glm::vec4 m_domain_time ; 
      glm::uvec4 m_domain_color ; 
      glm::vec4 m_light_position  ; 
      glm::vec4 m_light_direction ; 

  private:
      glm::ivec4 m_pickphoton ;  // see CompositionCfg.hh 
      glm::ivec4 m_pickface ;
      glm::ivec4 m_recselect ;
      glm::ivec4 m_colorparam ;
      glm::ivec4 m_selection ;
      glm::ivec4 m_flags ;
      glm::ivec4 m_pick ;
      glm::vec4  m_pick_f ; // for inputing pick using float slider 
      glm::vec4  m_param ;
      glm::vec4  m_scanparam ;
      glm::ivec4  m_nrmparam ;
      bool       m_animated ; 

  private:
      // residents
      Animator*   m_animator ; 
      Animator*   m_rotator ; 
      Camera*    m_camera ;
      Trackball* m_trackball ;
      Bookmarks* m_bookmarks ; 

      View*      m_view ;
      View*      m_standard_view ;
      View::View_t  m_viewtype ; 
      int         m_animator_period ; 
      int         m_ovperiod ; 
      int         m_tvperiod ; 
      NPY<float>* m_track ; 

      Light*     m_light ;
      Clipper*   m_clipper ;
      unsigned int m_count ; 
      NPY<float>*  m_axis_data ; 
      MultiViewNPY* m_axis_attr ;
      bool          m_changed ; 

  private:
      // visitors
      NumpyEvt*     m_evt ; 
      GGeo*         m_ggeo ; 

  private:
      // updated by *update* based on inputs and residents
      glm::vec4 m_viewport ; 
      glm::vec4 m_gaze ; 
      glm::vec4 m_clipplane ; 
      float     m_gazelength ;
      glm::mat4 m_world2eye ;     
      glm::mat4 m_eye2world ;     
      glm::mat4 m_world2camera ; 
      glm::mat4 m_camera2world ; 
      glm::mat4 m_eye2look ;     
      glm::mat4 m_look2eye ;     
      glm::mat4 m_world2clip ;     
      glm::mat4 m_world2clip_isnorm ;     
      glm::mat4 m_projection ;     

      glm::mat4 m_lookrotation ;     
      glm::mat4 m_ilookrotation ;
      float     m_lookphi ;  // degrees
     
      glm::mat4 m_trackballing ;     
      glm::mat4 m_itrackballing ;     

      glm::mat4 m_trackballrot ;     
      glm::mat4 m_itrackballrot ;     
      glm::mat4 m_trackballtra ;     
      glm::mat4 m_itrackballtra ;     

      glm::mat4 m_identity ;     
      glm::vec4 m_axis_x ; 
      glm::vec4 m_axis_y ; 
      glm::vec4 m_axis_z ; 
      glm::vec4 m_axis_x_color ; 
      glm::vec4 m_axis_y_color ; 
      glm::vec4 m_axis_z_color ; 

      std::string   m_command ; 
      unsigned int  m_command_length ; 

  private:
      glm::uvec4 m_frame_position ; 


  public: 
      void Summary(const char* msg);
      void Details(const char* msg);



};      

inline Composition::Composition()
  :
  m_model_to_world(),
  m_world_to_model(),
  m_extent(1.0f),
  m_center_extent(),
  m_pickphoton(0,0,0,0), 
  m_pickface(0,0,0,0), 
  m_recselect(), 
  m_colorparam(int(POL1),0,0,0), 
  m_selection(-INT_MAX,-INT_MAX,-INT_MAX,-INT_MAX),  // not 0, as that is liable to being meaningful
  m_pick( 1,0,0,0),      // initialize modulo scaledown to 1, 0 causes all invisible 
  m_param(25.f,0.030f,0.f,0.f),   // x: arbitrary scaling of genstep length, y: vector length dfrac
  m_scanparam(0.f,1.0f,0.5f,0.01f),   // ct scan  x:clip-z-cut y:slice-width
  m_animator(NULL),
  m_rotator(NULL),
  m_camera(NULL),
  m_trackball(NULL),
  m_bookmarks(NULL),
  m_view(NULL),
  m_standard_view(NULL),
  m_viewtype(View::STANDARD),
  m_animator_period(200),
  m_ovperiod(180),
  m_tvperiod(100),
  m_track(NULL), 
  m_light(NULL),
  m_clipper(NULL),
  m_count(0),
  m_axis_data(NULL),
  m_axis_attr(NULL),
  m_changed(true), 
  m_evt(NULL), 
  m_ggeo(NULL), 
  m_lookphi(0.f), 
  m_axis_x(1000.f,    0.f,    0.f, 0.f),
  m_axis_y(0.f   , 1000.f,    0.f, 0.f),
  m_axis_z(0.f   ,    0.f, 1000.f, 0.f),
  m_axis_x_color(1.f,0.f,0.f,1.f),
  m_axis_y_color(0.f,1.f,0.f,1.f),
  m_axis_z_color(0.f,0.f,1.f,1.f),
  m_command_length(256),
  m_frame_position(0,0,0,0)
{
    init();
}


inline void Composition::setFramePosition(const glm::uvec4& position)
{
    m_frame_position = position ; 
}
inline glm::uvec4& Composition::getFramePosition()
{
    return m_frame_position ; 
}


inline Camera* Composition::getCamera()
{
    return m_camera ;
}

inline View* Composition::getView()
{
    return m_view ;
}
inline Light* Composition::getLight()
{
    return m_light ;
}
inline Trackball* Composition::getTrackball()
{
    return m_trackball ;
}
inline Clipper* Composition::getClipper()
{
    return m_clipper ;
}

inline void Composition::setCamera(Camera* camera)
{
    m_camera = camera ; 
}


inline void Composition::setBookmarks(Bookmarks* bookmarks)
{
    m_bookmarks = bookmarks ; 
}



inline NumpyEvt* Composition::getEvt()
{
    return m_evt ; 
}
inline void Composition::setEvt(NumpyEvt* evt)
{
    m_evt = evt ; 
}



inline GGeo* Composition::getGeometry()
{
    return m_ggeo ; 
}
inline void Composition::setGeometry(GGeo* ggeo)
{
    m_ggeo = ggeo ; 
}


inline glm::vec4& Composition::getCenterExtent()
{
    return m_center_extent ; 
}
inline glm::vec4& Composition::getDomainCenterExtent()
{
    return m_domain_center_extent ; 
}
inline glm::vec4& Composition::getTimeDomain()
{
    return m_domain_time ; 
}
inline glm::uvec4& Composition::getColorDomain()
{
    return m_domain_color ; 
}
inline glm::vec4& Composition::getLightPosition()
{
    return m_light_position ; 
}
inline glm::vec4& Composition::getLightDirection()
{
    return m_light_direction ; 
}

inline void Composition::setOrbitalViewPeriod(int ovperiod)
{
    m_ovperiod = ovperiod ; 
}
inline void Composition::setAnimatorPeriod(int period)
{
    m_animator_period = period ; 
}


inline void Composition::setTrackViewPeriod(int tvperiod)
{
    m_tvperiod = tvperiod ; 
}
inline void Composition::setTrack(NPY<float>* track)
{
    m_track = track ; 
}






inline glm::mat4& Composition::getDomainISNorm()
{
    return m_domain_isnorm ; 
}


inline glm::ivec4& Composition::getPickPhoton()
{
    return m_pickphoton ; 
}

inline glm::ivec4& Composition::getPickFace()
{
    return m_pickface ; 
}



inline glm::ivec4& Composition::getRecSelect()
{
    return m_recselect ; 
}

inline glm::ivec4& Composition::getColorParam()
{
    return m_colorparam ; 
}

inline glm::ivec4& Composition::getSelection()
{
    return m_selection ; 
}

inline glm::ivec4& Composition::getFlags()
{
    return m_flags ; 
}
inline glm::ivec4& Composition::getPick()
{
    return m_pick; 
}
inline glm::vec4& Composition::getParam()
{
    return m_param ; 
}
inline glm::mat4& Composition::getModelToWorld()
{
    return m_model_to_world ; 
}
inline glm::mat4& Composition::getWorldToModel()
{
    return m_world_to_model ; 
}


inline float Composition::getExtent()
{
    return m_extent ; 
}



inline unsigned int Composition::getCount()
{
    return m_count ; 
}

inline NPY<float>* Composition::getAxisData()
{
    return m_axis_data ; 
}

inline MultiViewNPY* Composition::getAxisAttr()
{
    return m_axis_attr ; 
}

inline void Composition::nextColorStyle()
{
    int next = (getColorStyle() + 1) % NUM_COLOR_STYLE ; 
    setColorStyle( (ColorStyle_t)next ) ; 
}



inline void Composition::nextNormalStyle()
{
    int next = (getNormalStyle() + 1) % NUM_NORMAL_STYLE ; 
    setNormalStyle( (NormalStyle_t)next ) ; 
}
inline void Composition::setNormalStyle(NormalStyle_t style)
{
    m_nrmparam.x = int(style) ;
}
inline Composition::NormalStyle_t Composition::getNormalStyle()
{
    return (NormalStyle_t)m_nrmparam.x ;
}




inline void Composition::nextGeometryStyle()
{
    int next = (getGeometryStyle() + 1) % NUM_GEOMETRY_STYLE ; 
    setGeometryStyle( (GeometryStyle_t)next ) ; 
}
inline void Composition::setGeometryStyle(GeometryStyle_t style)
{
    m_nrmparam.y = int(style) ;
}
inline Composition::GeometryStyle_t Composition::getGeometryStyle()
{
    return (GeometryStyle_t)m_nrmparam.y ;
}
inline const char* Composition::getGeometryStyleName()
{
    return Composition::getGeometryStyleName(getGeometryStyle());
}





inline void Composition::nextViewType(unsigned int modifiers)
{
    int next = (getViewType() + 1) % View::NUM_VIEW_TYPE ; 
    setViewType( (View::View_t)next ) ; 
}

inline void Composition::setViewType(View::View_t type)
{
    m_viewtype = type ;
    applyViewType();
}

inline View::View_t Composition::getViewType()
{
    return m_viewtype ;
}



inline void Composition::nextPickPhotonStyle()
{
    int next = (getPickPhotonStyle() + 1) % NUM_PICKPHOTON_STYLE ; 
    setPickPhotonStyle( (PickPhotonStyle_t)next ) ; 
}
inline void Composition::setPickPhotonStyle(PickPhotonStyle_t style)
{
    m_pickphoton.y = int(style) ;
}
inline Composition::PickPhotonStyle_t Composition::getPickPhotonStyle()
{
    return (PickPhotonStyle_t)m_pickphoton.y ;
}



inline void Composition::setColorStyle(ColorStyle_t style)
{
    m_colorparam.x = int(style);
}
inline Composition::ColorStyle_t Composition::getColorStyle()
{
    return (ColorStyle_t)m_colorparam.x ; 
}

inline const char* Composition::getColorStyleName()
{
    return Composition::getColorStyleName(getColorStyle());
}




inline void Composition::setLookAngle(float phi)
{
    m_lookphi = phi ; 
}
inline float* Composition::getLookAnglePtr()
{
    return &m_lookphi ; 
}



