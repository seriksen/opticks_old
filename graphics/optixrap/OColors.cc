#include "OColors.hh"
#include "GColors.hh"

#include <boost/log/trivial.hpp>
#define LOG BOOST_LOG_TRIVIAL
// trace/debug/info/warning/error/fatal


void OColors::convert()
{
    GBuffer* buffer = m_colors->getCompositeBuffer();
    guint4 cd = m_colors->getCompositeDomain();

    optix::TextureSampler tex = makeColorSampler(buffer);
    m_context["color_texture"]->setTextureSampler(tex);
    m_context["color_domain"]->setUint(optix::make_uint4(cd.x, cd.y, cd.z, cd.w));

    // see cu/color_lookup.h
}


optix::TextureSampler OColors::makeColorSampler(GBuffer* buffer)
{
    unsigned int numElem  = buffer->getNumElements();
    unsigned int numItems = buffer->getNumItems();
    unsigned int numElemTot  = buffer->getNumElementsTotal();

    assert(numElem == 1 && numElemTot == numItems);

    unsigned int nx = numItems ;
    unsigned int ny = 1 ;

    LOG(info) << "OColors::makeColorSampler "
              << " nx " << nx 
              << " ny " << ny  ;

    optix::TextureSampler sampler = makeSampler(buffer, RT_FORMAT_UNSIGNED_BYTE4, nx, ny);
    return sampler ; 
}



optix::TextureSampler OColors::makeSampler(GBuffer* buffer, RTformat format, unsigned int nx, unsigned int ny)
{
    optix::Buffer optixBuffer = m_context->createBuffer(RT_BUFFER_INPUT, format, nx, ny );
    memcpy( optixBuffer->map(), buffer->getPointer(), buffer->getNumBytes() );
    optixBuffer->unmap(); 

    optix::TextureSampler sampler = m_context->createTextureSampler();
    sampler->setWrapMode(0, RT_WRAP_CLAMP_TO_EDGE ); 
    sampler->setWrapMode(1, RT_WRAP_CLAMP_TO_EDGE );

    RTfiltermode minification = RT_FILTER_LINEAR ;
    RTfiltermode magnification = RT_FILTER_LINEAR ;
    RTfiltermode mipmapping = RT_FILTER_NONE ;
    sampler->setFilteringModes(minification, magnification, mipmapping);

    sampler->setReadMode(RT_TEXTURE_READ_NORMALIZED_FLOAT);  
    sampler->setIndexingMode(RT_TEXTURE_INDEX_ARRAY_INDEX);  // by inspection : zero based array index offset by 0.5
    sampler->setMaxAnisotropy(1.0f);  
    sampler->setMipLevelCount(1u);     
    sampler->setArraySize(1u);        

    unsigned int texture_array_idx = 0u ;
    unsigned int mip_level = 0u ; 
    sampler->setBuffer(texture_array_idx, mip_level, optixBuffer);

    return sampler ; 
}


