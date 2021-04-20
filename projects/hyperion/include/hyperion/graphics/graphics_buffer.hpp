#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_device_object.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class GraphicsBuffer;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsBufferViewDescription : public GraphicsDeviceObjectDescription {
    
    };

    class GraphicsBufferView : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsBufferView() = default;
    protected:
        GraphicsBufferView(GraphicsDevice *device, const GraphicsBufferViewDescription &description, GraphicsBuffer *buffer);
    public:
        const GraphicsBufferViewDescription &GetDescription() const override { return m_description; }
        GraphicsBuffer *GetBuffer() const { return m_buffer; }
    protected:
        GraphicsBufferViewDescription m_description;
        GraphicsBuffer *m_buffer;
    };

    struct GraphicsBufferDescription : public GraphicsDeviceObjectDescription {
        
    };

    class GraphicsBuffer : public GraphicsDeviceObject {
    public:
        virtual ~GraphicsBuffer() = default;
    protected:
        GraphicsBuffer(GraphicsDevice *device, const GraphicsBufferDescription &description);
    public:
        const GraphicsBufferDescription &GetDescription() const override { return m_description; }

        virtual GraphicsBufferView *CreateView(const GraphicsBufferViewDescription &description) = 0;
    protected:
        GraphicsBufferDescription m_description;
    };

}