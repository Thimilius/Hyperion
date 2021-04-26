#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsRenderPassDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsRenderPass : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsRenderPass() = default;
    protected:
        GraphicsRenderPass(GraphicsDevice *device, const GraphicsRenderPassDescription &description);
    public:
        virtual const GraphicsRenderPassDescription &GetDescription() const override { return m_description; }
    protected:
        GraphicsRenderPassDescription m_description;
    };

}