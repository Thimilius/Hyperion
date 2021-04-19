//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_device.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/null/null_graphics_buffer.hpp"
#include "hyperion/graphics/driver/null/null_graphics_pipeline_state.hpp"
#include "hyperion/graphics/driver/null/null_graphics_shader.hpp"
#include "hyperion/graphics/driver/null/null_graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsBuffer *NullGraphicsDevice::CreateBuffer(const GraphicsBufferDescription &description) {
        return new NullGraphicsBuffer(this, description);
    }

    //--------------------------------------------------------------
    GraphicsTexture *NullGraphicsDevice::CreateTexture(const GraphicsTextureDescription &description) {
        return new NullGraphicsTexture(this, description);
    }

    //--------------------------------------------------------------
    GraphicsShader *NullGraphicsDevice::CreateShader(const GraphicsShaderDescription &description) {
        return new NullGraphicsShader(this, description);
    }

    //--------------------------------------------------------------
    GraphicsPipelineState *NullGraphicsDevice::CreatePipelineState(const GraphicsPipelineStateDescription &description) {
        return new NullGraphicsPipelineState(this, description);
    }

    //--------------------------------------------------------------
    void NullGraphicsDevice::DestroyObject(GraphicsDeviceObject *object) {
        delete object;
    }

}