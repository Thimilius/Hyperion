#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsTextureView : public GraphicsTextureView {
    public:
        OpenGLGraphicsTextureView(GraphicsDevice *device, const GraphicsTextureViewDescription &description, GraphicsTexture *texture);
    };

    class OpenGLGraphicsTexture : public GraphicsTexture {
    public:
        OpenGLGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description);
        ~OpenGLGraphicsTexture();
    public:
        GraphicsTextureView *CreateView(const GraphicsTextureViewDescription &description) override;
    private:
        GLuint m_texture_id;
    };

}