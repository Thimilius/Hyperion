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
        void InitializeDebug();
        void InitializeInstance(const Vector<const char *> &required_extension_names, const Vector<const char *> &required_layer_names);
        void InitializePhysicalDevice();
        void InitializeQueueFamilyIndices();

        void QueryExtensions();
        void CheckExtensions(Vector<const char *> &required_extension_names);
        void CheckExtension(Vector<const char *> &required_extension_names, const char *extension_name);

        void QueryLayers();
        void CheckLayers(Vector<const char *> &required_layer_names);
        void CheckLayer(Vector<const char *> &required_layer_names, const char *layer_name);
    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_pointer);
    private:
        VkInstance m_instance = VK_NULL_HANDLE;
        VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;

        VkDebugUtilsMessengerEXT m_debug_messenger = VK_NULL_HANDLE;

        struct QueueFamilyIndices {
            uint32 graphics_family_index;

            bool has_graphics_family_index = false;
        } m_queue_family_indices;

        Vector<VkExtensionProperties> m_extensions;
        Vector<VkLayerProperties> m_layers;
    };

}