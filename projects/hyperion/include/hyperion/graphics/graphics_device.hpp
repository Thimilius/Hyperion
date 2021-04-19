#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_buffer.hpp"
#include "hyperion/graphics/graphics_shader.hpp"
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class IGraphicsDevice {
    public:
        virtual ~IGraphicsDevice() = default;
    public:
        virtual IGraphicsBuffer *CreateBuffer(const GraphicsBufferDescription &description) = 0;
        virtual IGraphicsTexture *CreateTexture(const GraphicsTextureDescription &description) = 0;
        virtual IGraphicsShader *CreateShader(const GraphicsShaderDescription &description) = 0;
    };

}