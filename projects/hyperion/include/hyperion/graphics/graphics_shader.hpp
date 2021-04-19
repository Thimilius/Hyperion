#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsShaderDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsShader : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsShader() = default;
    public:
        virtual const GraphicsShaderDescription &GetDescription() const override = 0;
    };

}