#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_render_pass.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsRenderPass : public GraphicsRenderPass {
    public:
        NullGraphicsRenderPass(GraphicsDevice *device, const GraphicsRenderPassDescription &description);
    };

}