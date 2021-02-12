#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class WindowsWindow;

    namespace Rendering {
        class RenderEngine;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class VSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    struct GraphicsContextProperties {
        String vendor;
        String renderer;
        String version;
    };
    
    struct GraphicsContextDescriptor {
        int32 color_bits = 32;
        int32 depth_bits = 24;
        int32 stencil_bits = 8;
        int32 msaa_samples = 0;
    };

    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
    public:
        inline const GraphicsContextProperties &GetProperties() const { return m_properties; }
    protected:
        GraphicsContext() = default;
    protected:
        virtual void Initialize(const GraphicsContextDescriptor &descriptor) = 0;
        virtual void Present() = 0;
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
    protected:
        GraphicsContextProperties m_properties;
    private:
        friend class Hyperion::WindowsWindow;
        friend class Hyperion::Rendering::RenderEngine;
    };

}