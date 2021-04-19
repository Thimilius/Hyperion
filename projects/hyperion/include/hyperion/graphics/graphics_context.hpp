#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/graphics/graphics_device.hpp"
#include "hyperion/graphics/graphics_device_context.hpp"
#include "hyperion/graphics/graphics_swap_chain.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

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

    class IGraphicsContext {
    public:
        virtual ~IGraphicsContext() = default;
    public:
        virtual GraphicsContextProperties GetProperties() const = 0;

        virtual void Initialize(const GraphicsContextDescriptor &descriptor) = 0;
        virtual void Shutdown() = 0;

        virtual void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) = 0;

        virtual void SwapBuffers() = 0; 
        virtual void SetVSyncMode(VSyncMode vsync_mode) = 0;
    };

}