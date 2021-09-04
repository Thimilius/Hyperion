#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Graphics {
    class VulkanGraphicsDevice;
    class VulkanGraphicsDeviceContext;
    class VulkanGraphicsSwapChain;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    struct VulkanQueueFamilyIndices {
        uint32 graphics_family_index;
        uint32 present_family_index;

        bool8 has_graphics_family_index = false;
        bool8 has_present_family_index = false;
    };

    class VulkanGraphicsContext : public GraphicsContext {
    public:
        inline VkInstance GetInstance() const { return m_instance; }
        inline VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; }
        inline const VulkanQueueFamilyIndices &GetQueueFamilyIndices() const { return m_queue_family_indices; }

        inline VulkanGraphicsDevice *GetDevice() const { return m_device; }
        inline VulkanGraphicsDeviceContext *GetDeviceContext() const { return m_device_context; }
        inline VulkanGraphicsSwapChain *GetSwapChain() const { return m_swap_chain; }

        GraphicsContextProperties GetProperties() const override;

        void Initialize(const GraphicsContextDescriptor &descriptor) override;
        void Shutdown() override;

        void CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) override;

        void SwapBuffers() override;
        void SetVSyncMode(VSyncMode vsync_mode) override;
    protected:
        virtual VkResult CreateSurface(VkSurfaceKHR *surface) = 0;

        virtual void CheckExtensions(List<const char *> &required_extension_names);
        void CheckExtension(List<const char *> &required_extension_names, const char *extension_name);
        virtual void CheckLayers(List<const char *> &required_layer_names);
        void CheckLayer(List<const char *> &required_layer_names, const char *layer_name);
    private:
        void InitializeDebug();
        void InitializeInstance(const List<const char *> &required_extension_names, const List<const char *> &required_layer_names);
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

        VulkanGraphicsDevice *m_device = nullptr;
        VulkanGraphicsDeviceContext *m_device_context = nullptr;
        VulkanGraphicsSwapChain *m_swap_chain = nullptr;

        VkDebugUtilsMessengerEXT m_debug_messenger = nullptr;

        VulkanQueueFamilyIndices m_queue_family_indices;
        List<VkExtensionProperties> m_extensions;
        List<VkLayerProperties> m_layers;
    };

}