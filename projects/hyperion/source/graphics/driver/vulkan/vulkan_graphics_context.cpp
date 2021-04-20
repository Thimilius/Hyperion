//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_context.hpp"

#define HYP_VULKAN_LOG_EXTENSIONS true

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    constexpr bool g_log_extensions = true;

    //--------------------------------------------------------------
    GraphicsContextProperties VulkanGraphicsContext::GetProperties() const {
        return GraphicsContextProperties();
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::Initialize(const GraphicsContextDescriptor &descriptor) {
        QueryExtensions();
        CheckExtensions();
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::Shutdown() {
        
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CreateDeviceAndSwapChain(GraphicsDevice **device, GraphicsDeviceContext **device_context, GraphicsSwapChain **swap_chain) {
        HYP_ASSERT(device);
        HYP_ASSERT(device_context);
        HYP_ASSERT(swap_chain);
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::SwapBuffers() {

    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::SetVSyncMode(VSyncMode vsync_mode) {

    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::QueryExtensions() {
        uint32 extension_count = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
        m_extensions = Vector<VkExtensionProperties>(extension_count);
        vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, m_extensions.data());

        if constexpr (g_log_extensions) {
            for (const VkExtensionProperties &extension : m_extensions) {
                HYP_LOG_INFO("Graphics", "Vulkan extension: '{}' available!", extension.extensionName);
            }
        }
    }

    //--------------------------------------------------------------
    void VulkanGraphicsContext::CheckExtensions() {

    }

}