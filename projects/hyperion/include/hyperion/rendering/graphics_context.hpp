#pragma once

#include "hyperion/core/app/window.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    struct ContextProperties {
        String vendor;
        String renderer;
        String version;
    };
    
    struct ContextLimits {
        s32 max_msaa_samples;
        s32 max_texture_units;
    };

    class GraphicsContext {
    public:
        virtual ~GraphicsContext() = default;
        
        inline const ContextProperties &GetProperties() const { return m_properties; }
        inline const ContextLimits &GetLimits() const { return m_limits; }

        inline static GraphicsContext *GetInstance() { return s_instance; }
    protected:
        GraphicsContext();

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
    protected:
        ContextProperties m_properties;
        ContextLimits m_limits;

        inline static GraphicsContext *s_instance;
    private:
        friend class Hyperion::WindowsWindow;
    };

}