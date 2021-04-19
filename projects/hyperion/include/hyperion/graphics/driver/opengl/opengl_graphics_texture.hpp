#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsTexture : public GraphicsTexture {
    public:
        OpenGLGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description);
        ~OpenGLGraphicsTexture();
    private:
        GLuint m_texture_id;
    };

}