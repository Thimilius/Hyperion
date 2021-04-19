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
    protected:
        GraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description);
    public:
        const GraphicsBufferDescription &GetDescription() const override { return m_description; }
    protected:
        GraphicsBufferDescription m_description;
    };

}