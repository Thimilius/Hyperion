#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    class VulkanGraphicsContext : public GraphicsContext {
    public:
        inline VkInstance GetInstance() const { return m_instance; }
        inline VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; }

        GraphicsContextProperties GetProperties() const override;

        void Initialize(const GraphicsContextDescriptor &descriptor) override;
        void Shutdown() override;

        void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) override;

        void SwapBuffers() override;
        void SetVSyncMode(VSyncMode vsync_mode) override;
    protected:
        virtual VkResult CreateSurface(VkSurfaceKHR *surface) = 0;

        virtual void CheckExtensions(Vector<const char *> &required_extension_names);
        void CheckExtension(Vector<const char *> &required_extension_names, const char *extension_name);
        virtual void CheckLayers(Vector<const char *> &required_layer_names);
        void CheckLayer(Vector<const char *> &required_layer_names, const char *layer_name);
    private:
        void InitializeDebug();
        void InitializeInstance(const Vector<const char *> &required_extension_names, const Vector<const char *> &required_layer_names);
        void InitializeSurface();
        void InitializePhysicalDevice();
        void InitializeQueueFamilyIndices();

        void QueryExtensions();
        void QueryLayers();
    private:
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_pointer);
    protected:
        VkInstance m_instance = nullptr;
        VkPhysicalDevice m_physical_device = nullptr;
        VkSurfaceKHR m_surface = nullptr; // NOTE: The surface is not owned here.

        VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;

        struct QueueFamilyIndices {
            uint32 graphics_family_index;
            uint32 present_family_index;

            bool has_graphics_family_index = false;
            bool has_present_family_index = false;
        } m_queue_family_indices;

        Vector<VkExtensionProperties> m_extensions;
        Vector<VkLayerProperties> m_layers;
    };

}