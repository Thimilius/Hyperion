//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_device.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/opengl/opengl_graphics_buffer.hpp"
#include "hyperion/graphics/driver/opengl/opengl_graphics_pipeline_state.hpp"
#include "hyperion/graphics/driver/opengl/opengl_graphics_render_pass.hpp"
#include "hyperion/graphics/driver/opengl/opengl_graphics_shader.hpp"
#include "hyperion/graphics/driver/opengl/opengl_graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsBuffer *OpenGLGraphicsDevice::CreateBuffer(const GraphicsBufferDescription &description) {
        return new OpenGLGraphicsBuffer(this, description);
    }

    //--------------------------------------------------------------
    GraphicsTexture *OpenGLGraphicsDevice::CreateTexture(const GraphicsTextureDescription &description) {
        return new OpenGLGraphicsTexture(this, description);
    }

    //--------------------------------------------------------------
    GraphicsShader *OpenGLGraphicsDevice::CreateShader(const GraphicsShaderDescription &description) {
        return new OpenGLGraphicsShader(this, description);
    }

    //--------------------------------------------------------------
    GraphicsRenderPass *OpenGLGraphicsDevice::CreateRenderPass(const GraphicsRenderPassDescription &description) {
        return new OpenGLGraphicsRenderPass(this, description);
    }

    //--------------------------------------------------------------
    GraphicsPipelineState *OpenGLGraphicsDevice::CreatePipelineState(const GraphicsPipelineStateDescription &description) {
        return new OpenGLGraphicsPipelineState(this, description);
    }

    //--------------------------------------------------------------
    void OpenGLGraphicsDevice::DestroyObject(GraphicsDeviceObject *object) {
        delete object;
    }

}