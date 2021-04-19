//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsTexture::GraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}