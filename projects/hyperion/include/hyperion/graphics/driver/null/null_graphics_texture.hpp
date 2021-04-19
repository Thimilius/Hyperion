#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsTexture : public GraphicsTexture {
    public:
        NullGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description);
    };

}