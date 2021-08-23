#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"
#include "hyperion/graphics/graphics_render_pass.hpp"
#include "hyperion/graphics/graphics_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsFramebufferDescription : public GraphicsDeviceObjectDescription {
        GraphicsRenderPass *render_pass;

        List<GraphicsTextureView *> attachments;
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