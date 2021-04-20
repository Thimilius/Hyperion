#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class GraphicsTexture;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsTextureViewDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsTextureView : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsTextureView() = default;
    protected:
        GraphicsTextureView(GraphicsDevice *device, const GraphicsTextureViewDescription &description, GraphicsTexture *texture);
    public:
        virtual const GraphicsTextureViewDescription &GetDescription() const override { return m_description; }
        GraphicsTexture *GetTexture() const { return m_texture; }
    protected:
        GraphicsTextureViewDescription m_description;
        GraphicsTexture *m_texture;
    };

    struct GraphicsTextureDescription : public GraphicsDeviceObjectDescription {

    };

    class GraphicsTexture : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsTexture() = default;
    protected:
        GraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description);
    public:
        virtual const GraphicsTextureDescription &GetDescription() const override { return m_description; }

        virtual GraphicsTextureView *CreateView(const GraphicsTextureViewDescription &description) = 0;
    protected:
        GraphicsTextureDescription m_description;
    };

}