#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsBufferDescription : public GraphicsObjectDescription {

    };

    class IGraphicsBuffer : public IGraphicsObject {
    public:
        virtual ~IGraphicsBuffer() = default;
    public:
        virtual const GraphicsBufferDescription &GetDescription() const override = 0;
    };

}