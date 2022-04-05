//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class VulkanRenderDriver;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class VulkanRenderContext : public IRenderContext {
  private:
    struct QueueFamilyIndices {
      int32 graphics_family = -1;

      bool IsValid() {
        return graphics_family >= 0;
      }
    };
  public:
    RenderContextProperties GetProperties() const override { return m_properties; }

    IRenderDriver *GetDriver() override { return &m_render_driver; }
    VkInstance GetInstance() const { return m_instance; }
    VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; }

    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
    void Shutdown() override;
  protected:
    virtual Array<const char *> GetRequiredExtensions() const;

    virtual VkResult CreateSurface(Window *window, VkInstance instance, VkSurfaceKHR *surface) = 0;
  private:
    void QueryLayersAndExtensions();
    void CreateInstance();
#ifdef HYP_DEBUG
    void RegisterDebugMessageCallback();
    void FillDebugMessengerCreateInfoStruct(VkDebugUtilsMessengerCreateInfoEXT *messenger_create_info);
#endif
    void PickPhysicalDevice();

    Array<const char *> GetAndValidateInstanceLayer();
    bool8 SupportsInstanceLayer(const char *layer_name);
    Array<const char *> GetAndValidateInstanceExtensions();
    bool8 SupportsInstanceExtension(const char *extension_name);

    bool8 IsDeviceSuitable(VkPhysicalDevice device);
    QueueFamilyIndices FindQueueFamilyIndices(VkPhysicalDevice device);

    void *LoadFunction(const char *name);
  private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT severity,
      VkDebugUtilsMessageTypeFlagsEXT type,
      const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
      void *user_data);
  private:
    RenderContextProperties m_properties;

    VulkanRenderDriver m_render_driver;

    Array<VkLayerProperties> m_instance_layers;
    Array<VkExtensionProperties> m_instance_extensions;

    VkInstance m_instance = VK_NULL_HANDLE;
    VkPhysicalDevice m_physical_device = VK_NULL_HANDLE;
#ifdef HYP_DEBUG
    VkDebugUtilsMessengerEXT m_debug_messenger = VK_NULL_HANDLE;
#endif
    VkSurfaceKHR m_surface;
  private:
    friend class VulkanRenderDriver;
  };

}