//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/null/null_graphics_context.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/null/null_graphics_device.hpp"
#include "hyperion/graphics/driver/null/null_graphics_device_context.hpp"
#include "hyperion/graphics/driver/null/null_graphics_swap_chain.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    GraphicsContextProperties NullGraphicsContext::GetProperties() const {
        GraphicsContextProperties properties;
        properties.renderer = "Null";
        properties.vendor = "Null";
        properties.version = "1.0.0";
        return properties;
    }

    //--------------------------------------------------------------
    void NullGraphicsContext::Initialize(const GraphicsContextDescriptor &descriptor) {
        HYP_LOG_INFO("Graphics", "Initialized Null graphics driver!");
    }

    //--------------------------------------------------------------
    void NullGraphicsContext::Shutdown() {

    }

    //--------------------------------------------------------------
    void NullGraphicsContext::CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) {
        HYP_ASSERT(device);
        HYP_ASSERT(device_context);
        HYP_ASSERT(swap_chain);

        *device = new NullGraphicsDevice();
        *device_context = new NullGraphicsDeviceContext();
        *swap_chain = new NullGraphicsSwapChain();
    }

}