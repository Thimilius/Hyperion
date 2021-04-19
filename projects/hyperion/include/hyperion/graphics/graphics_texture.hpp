#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsTextureDescription : public GraphicsObjectDescription {

    };

    class IGraphicsTexture : public IGraphicsObject {
    public:
        virtual ~IGraphicsTexture() = default;
    public:
        virtual const GraphicsTextureDescription &GetDescription() const override = 0;
    };

}