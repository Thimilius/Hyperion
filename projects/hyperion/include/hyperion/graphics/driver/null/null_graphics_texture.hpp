#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsTextureView : public GraphicsTextureView {
    public:
        NullGraphicsTextureView(GraphicsDevice *device, const GraphicsTextureViewDescription &description, GraphicsTexture *texture);
    };

    class NullGraphicsTexture : public GraphicsTexture {
    public:
        NullGraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description);
    public:
        GraphicsTextureView *CreateView(const GraphicsTextureViewDescription &description) override;
    };

}