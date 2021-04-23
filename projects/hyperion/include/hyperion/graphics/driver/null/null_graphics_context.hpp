#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsContext : public GraphicsContext {
    public:
        GraphicsContextProperties GetProperties() const override;
    public:
        void Initialize(const GraphicsContextDescriptor &descriptor) override;
        void Shutdown() override;

        void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) override;

        void SwapBuffers() override { }
        void SetVSyncMode(VSyncMode vsync_mode) override { }
    };

}