#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class NullGraphicsContext : public IGraphicsContext {
    public:
        GraphicsContextProperties GetProperties() const override;
    public:
        void Initialize(const GraphicsContextDescriptor &descriptor) override;
        void Shutdown() override;

        void SwapBuffers() override { }
        void SetVSyncMode(VSyncMode vsync_mode) override { }
    };

}