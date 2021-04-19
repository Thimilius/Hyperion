#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsTextureDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsTexture : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsTexture() = default;
    public:
        virtual const GraphicsTextureDescription &GetDescription() const override = 0;
    };

}