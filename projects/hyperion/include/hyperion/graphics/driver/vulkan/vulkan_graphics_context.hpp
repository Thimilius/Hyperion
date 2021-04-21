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
        void InitDebug();

        void QueryExtensions();
        void CheckExtensions(Vector<const char *> &required_extension_names);
        void CheckExtension(Vector<const char *> &required_extension_names, const char *extension_name);

        void QueryLayers();
        void CheckLayers(Vector<const char *> &required_layer_names);
        void CheckLayer(Vector<const char *> &required_layer_names, const char *layer_name);
    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_pointer);
    private:
        VkInstance m_instance;

        VkDebugUtilsMessengerEXT m_debug_messenger;

        Vector<VkExtensionProperties> m_extensions;
        Vector<VkLayerProperties> m_layers;
    };

}