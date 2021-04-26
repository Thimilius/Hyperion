#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsDevice final : public GraphicsDevice {
    public:
        GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) override;
        GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) override;
        GraphicsShader *CreateShader(const GraphicsShaderDescription &description) override;
        GraphicsRenderPass *CreateRenderPass(const GraphicsRenderPassDescription &description) override;
        GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description) override;

        void DestroyObject(GraphicsDeviceObject *object) override;
    };

}