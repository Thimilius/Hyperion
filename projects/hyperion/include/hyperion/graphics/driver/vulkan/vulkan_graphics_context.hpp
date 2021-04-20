#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsContext : public IGraphicsContext {
    public:
        GraphicsContextProperties GetProperties() const;

        void Initialize(const GraphicsContextDescriptor &descriptor);
        void Shutdown();

        void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain);

        void SwapBuffers();
        void SetVSyncMode(VSyncMode vsync_mode);
    private:
        void QueryExtensions();
        void CheckExtensions();
    private:
        VkInstance m_instance;

        Vector<VkExtensionProperties> m_extensions;
    };

}