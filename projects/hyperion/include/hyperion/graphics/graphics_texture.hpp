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
    protected:
        GraphicsTexture(GraphicsDevice *device, const GraphicsTextureDescription &description);
    public:
        virtual const GraphicsTextureDescription &GetDescription() const override { return m_description; }
    protected:
        GraphicsTextureDescription m_description;
    };

}