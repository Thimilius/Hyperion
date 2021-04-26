#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_render_pass.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class OpenGLGraphicsRenderPass : public GraphicsRenderPass {
    public:
        OpenGLGraphicsRenderPass(GraphicsDevice *device, const GraphicsRenderPassDescription &description);
    };

}