//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class VulkanRenderContext : public IRenderContext {
  public:
    RenderContextProperties GetProperties() const override { return m_properties; }

    IRenderDriver *GetDriver() override { return &m_render_driver; }

    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
    void Shutdown() override;
  private:
    void QueryLayersAndExtensions();
    void CreateInstance();

    Array<const char *> GetAndValidateInstanceLayer();
    bool8 SupportsInstanceLayer(const char *layer_name);
    Array<const char *> GetAndValidateInstanceExtensions();
    bool8 SupportsInstanceExtension(const char *extension_name);

    void RegisterDebugMessageCallback();

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

    VkInstance m_instance;
#ifdef HYP_DEBUG
    VkDebugUtilsMessengerEXT m_debug_messenger;
#endif
  };

}