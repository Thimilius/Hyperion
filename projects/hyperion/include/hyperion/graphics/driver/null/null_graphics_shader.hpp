#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_shader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsShader : public GraphicsShader {
    public:
        NullGraphicsShader(GraphicsDevice *device, const GraphicsShaderDescription &description);
    };

}