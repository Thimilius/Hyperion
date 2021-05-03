#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_buffer.hpp"
#include "hyperion/graphics/graphics_framebuffer.hpp"
#include "hyperion/graphics/graphics_pipeline_state.hpp"
#include "hyperion/graphics/graphics_render_pass.hpp"
#include "hyperion/graphics/graphics_shader.hpp"
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class GraphicsDevice {
    public:
        virtual ~GraphicsDevice() = default;
    public:
        virtual GraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) = 0;
        virtual GraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) = 0;
        virtual GraphicsShader *CreateShader(const GraphicsShaderDescription &description) = 0;
        virtual GraphicsRenderPass *CreateRenderPass(const GraphicsRenderPassDescription &description) = 0;
        virtual GraphicsFramebuffer *CreateFramebuffer(const GraphicsFramebufferDescription &description) = 0;
        virtual GraphicsPipelineState *CreatePipelineState(const GraphicsPipelineStateDescription &description) = 0;

        virtual void DestroyObject(GraphicsDeviceObject *object) = 0;
    };

}