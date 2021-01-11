#pragma once

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    enum class VSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

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
    protected:
        GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void Present() = 0;
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
    protected:
        ContextProperties m_properties;
        ContextLimits m_limits;
    private:
        friend class Hyperion::WindowsWindow;
        friend class Hyperion::Rendering::RenderEngine;
    };

}