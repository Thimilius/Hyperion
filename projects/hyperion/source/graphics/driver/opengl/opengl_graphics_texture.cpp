//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    OpenGLGraphicsTexture::OpenGLGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description) : GraphicsTexture(device, description) {

    }

    //--------------------------------------------------------------
    OpenGLGraphicsTexture::~OpenGLGraphicsTexture() {
        glDeleteTextures(1, &m_texture_id);
    }

}