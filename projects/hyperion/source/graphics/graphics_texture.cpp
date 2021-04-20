//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsTextureView::GraphicsTextureView(GraphicsDevice *device, const GraphicsTextureViewDescription &description, GraphicsTexture *texture) : GraphicsDeviceObject(device) {
        m_description = description;
        m_texture = texture;
    }

    //--------------------------------------------------------------
    GraphicsTexture::GraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description) : GraphicsDeviceObject(device) {
        m_description = description;
    }

}