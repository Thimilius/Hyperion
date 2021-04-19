#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsDevice final : public GraphicsDevice {
    public:
        GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) { return nullptr; }
        GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) { return nullptr; }
        GraphicsShader *CreateShader(const GraphicsShaderDescription &description) { return nullptr; }
        GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description) { return nullptr; }
    };

}