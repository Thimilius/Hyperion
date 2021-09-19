#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/driver/render_driver.hpp"
#include "hyperion/rendering/driver/render_driver_context_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

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