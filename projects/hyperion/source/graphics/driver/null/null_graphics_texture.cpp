//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    NullGraphicsTextureView::NullGraphicsTextureView(GraphicsDevice *device, const GraphicsTextureViewDescription &description, GraphicsTexture *texture) : GraphicsTextureView(device, description, texture) {

    }

    //--------------------------------------------------------------
    NullGraphicsTexture::NullGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description) : GraphicsTexture(device, description) {

    }

    //--------------------------------------------------------------
    GraphicsTextureView *NullGraphicsTexture::CreateView(const GraphicsTextureViewDescription &description) {
        return new NullGraphicsTextureView(GetDevice(), description, this);
    }

}