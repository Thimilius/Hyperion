#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsDevice final : public GraphicsDevice {
    public:
        GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description);
        GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description);
        GraphicsShader *CreateShader(const GraphicsShaderDescription &description);
        GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description);

        void DestroyObject(GraphicsDeviceObject *object);
    };

}