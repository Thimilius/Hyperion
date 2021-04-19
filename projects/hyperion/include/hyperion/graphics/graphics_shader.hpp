#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsShaderDescription : public GraphicsObjectDescription {

    };

    class IGraphicsShader : public IGraphicsObject {
    public:
        virtual ~IGraphicsShader() = default;
    public:
        virtual const GraphicsShaderDescription &GetDescription() const override = 0;
    };

}