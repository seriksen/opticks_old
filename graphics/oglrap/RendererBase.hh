#pragma once

class Prog ;

class RendererBase {
   public:
      RendererBase(const char* tag, const char* dir=NULL, const char* incl_path=NULL);
      char* getShaderDir(); 
      char* getShaderTag(); 
  public:
      void make_shader();   
  protected:
      Prog*   m_shader ;
      GLuint  m_program ;
  private:
      char* m_shaderdir ; 
      char* m_shadertag ; 
      char* m_incl_path ; 

};


inline char* RendererBase::getShaderDir()
{
    return m_shaderdir ;
}
inline char* RendererBase::getShaderTag()
{
    return m_shadertag ;
}

