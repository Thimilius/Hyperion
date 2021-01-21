#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

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
    
    struct GraphicsContextLimits {
        int32 max_msaa_samples;
        int32 max_texture_units;
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
        
        inline const GraphicsContextProperties &GetProperties() const { return m_properties; }
        inline const GraphicsContextLimits &GetLimits() const { return m_limits; }
    protected:
        GraphicsContext() = default;

        virtual void Init(const GraphicsContextDescriptor &descriptor) = 0;
        virtual void Present() = 0;
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
    protected:
        GraphicsContextProperties m_properties;
        GraphicsContextLimits m_limits;
    private:
        friend class Hyperion::WindowsWindow;
        friend class Hyperion::Rendering::RenderEngine;
    };

}