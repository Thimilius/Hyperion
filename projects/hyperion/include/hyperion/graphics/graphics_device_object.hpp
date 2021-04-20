#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class GraphicsDevice;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct GraphicsDeviceObjectDescription {
        String name = "";
    };

    class GraphicsDeviceObject : public INonCopyable {
    public:
        virtual ~GraphicsDeviceObject() = default;
    protected:
        GraphicsDeviceObject(GraphicsDevice *device);
    public:
        GraphicsDevice *GetDevice() const { return m_device; }

        virtual const GraphicsDeviceObjectDescription &GetDescription() const = 0;
    private:
        GraphicsDevice *m_device;
    };

}