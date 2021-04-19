#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsShader : public GraphicsShader {
    public:
        OpenGLGraphicsShader(GraphicsDevice *device, const GraphicsShaderDescription &description);
        ~OpenGLGraphicsShader();
    private:
        GLuint m_shader_id;
    };

}