#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsFramebufferDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsFramebuffer : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsFramebuffer() = default;
    protected:
        GraphicsFramebuffer(GraphicsDevice * device, const GraphicsFramebufferDescription & description);
    public:
        virtual const GraphicsFramebufferDescription &GetDescription() const override { return m_description; }
    protected:
        GraphicsFramebufferDescription m_description;
    };

}