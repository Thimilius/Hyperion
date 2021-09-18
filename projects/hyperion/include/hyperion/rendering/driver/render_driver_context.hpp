#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/rendering/driver/render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class VSyncMode {
        DontSync,
        EveryVBlank,
        EverySecondVBlank
    };

    struct RenderDriverContextProperties {
        String vendor;
        String renderer;
        String version;
    };
    
    struct RenderDriverContextDescriptor {
        int32 color_bits = 32;
        int32 depth_bits = 24;
        int32 stencil_bits = 8;
        int32 msaa_samples = 0;
    };

    class IRenderDriverContext : public INonCopyable {
    public:
        virtual ~IRenderDriverContext() = default;
    public:
        virtual RenderDriverContextProperties GetProperties() const = 0;

        virtual IRenderDriver *GetDriver() = 0;

        virtual void Initialize(const RenderDriverContextDescriptor &descriptor) = 0;
        virtual void Shutdown() = 0;

        virtual void SwapBuffers() = 0; 
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
    };

}