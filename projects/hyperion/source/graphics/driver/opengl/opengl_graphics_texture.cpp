//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    OpenGLGraphicsTextureView::OpenGLGraphicsTextureView(GraphicsDevice *device, const GraphicsTextureViewDescription &description, GraphicsTexture *texture) : GraphicsTextureView(device, description, texture) {

    }

    //--------------------------------------------------------------
    OpenGLGraphicsTexture::OpenGLGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description) : GraphicsTexture(device, description) {

    }

    //--------------------------------------------------------------
    OpenGLGraphicsTexture::~OpenGLGraphicsTexture() {

    }

    //--------------------------------------------------------------
    GraphicsTextureView *OpenGLGraphicsTexture::CreateView(const GraphicsTextureViewDescription &description) {
        return new OpenGLGraphicsTextureView(GetDevice(), description, this);
    }

}