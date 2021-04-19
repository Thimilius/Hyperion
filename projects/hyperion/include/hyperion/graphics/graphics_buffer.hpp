#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsBufferDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsBuffer : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsBuffer() = default;
    public:
        virtual const GraphicsBufferDescription &GetDescription() const override = 0;
    };

}