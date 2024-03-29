//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <vulkan/vulkan.h>
#include <vk_mem_alloc.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class VulkanRenderDriver;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct VulkanQueueFamilyIndices {
    int32 graphics_family = -1;
    int32 presentation_family = -1;

    bool IsValid() {
      return graphics_family >= 0 && presentation_family >= 0;
    }
  };

  struct VulkanSwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    Array<VkSurfaceFormatKHR> formats;
    Array<VkPresentModeKHR> present_modes;
  };

  struct VulkanUniformBufferObject {
    Matrix4x4 view;
    Matrix4x4 projection;
  };

  struct VulkanPushConstant {
    Matrix4x4 model;
  };
  
  class VulkanRenderContext : public IRenderContext {
  public:
    RenderContextProperties GetProperties() const override { return m_properties; }

    IRenderDriver *GetDriver() override { return &m_render_driver; }
    VkInstance GetInstance() const { return m_instance; }
    VkPhysicalDevice GetPhysicalDevice() const { return m_physical_device; }

    void Initialize(Window *main_window, const RenderContextDescriptor &descriptor) override;
    void Shutdown() override;

    void SwapBuffers(Window *window) override;
  protected:
    virtual Array<const char *> GetRequiredInstanceExtensions() const;
    virtual Array<const char *> GetRequiredDeviceExtensions() const;

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
    VulkanQueueFamilyIndices FindQueueFamilyIndices(VkPhysicalDevice device);
    bool8 CheckDeviceExtensionSupport(VkPhysicalDevice device);
    VulkanSwapchainSupportDetails QuerySwapchainSupportDetails(VkPhysicalDevice device);

    void CreateLogicalDevice();

    void CreateSwapChain();
    VkSurfaceFormatKHR ChooseSwapChainFormat(const Array<VkSurfaceFormatKHR> &formats);
    VkPresentModeKHR ChoosePresentMode(const Array<VkPresentModeKHR> &present_modes);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

    void CreateImageViews();

    void CreateRenderPass();

    void CreateDescriptorSetLayout();

    void CreateGraphicsPipeline();

    void CreateDepthObjects();
    VkFormat FindDepthFormat();
    VkFormat FindSupportedFormat(const Array<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    
    void CreateFramebuffers();

    void CreateCommandPool();

    void CreateCommandBuffers();

    void CreateSyncObjects();

    void CreateAllocator();

    void CreateVertexBuffer();
    void CreateIndexBuffer();
    uint32 FindMemoryType(uint32 type_filter, VkMemoryPropertyFlags properties);
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memory_usage, VkBuffer &buffer, VmaAllocation &buffer_memory);
    void CopyBuffer(VkBuffer source_buffer, VkBuffer destination_buffer, VkDeviceSize size);

    void CreateUniformBuffers();
    void CreateDescriptorPool();
    void CreateDescriptorSets();

    void RecreateSwapchain();
    void CleanupSwapchain();

    void Cleanup();

    PFN_vkVoidFunction LoadFunction(const char *name);
  private:
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type,
                                                               const VkDebugUtilsMessengerCallbackDataEXT *callback_data, void *user_data);
  public:
    static constexpr uint32 MAX_FRAMES_IN_FLIGHT = 2;
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

    VkDevice m_device;
    VkQueue m_graphics_queue;
    VkQueue m_presentation_queue;

    VkSurfaceKHR m_surface;
    VkSwapchainKHR m_swapchain;
    VkFormat m_swapchain_image_format;
    VkExtent2D m_swapchain_extent;
    Array<VkImage> m_swapchain_images;
    Array<VkImageView> m_swapchain_image_views;
    Array<VkFramebuffer> m_swapchain_framebuffers;

    VkRenderPass m_render_pass;
    VkDescriptorSetLayout m_descriptor_set_layout;
    VkPipelineLayout m_pipeline_layout;
    VkPipeline m_graphics_pipeline;

    VkCommandPool m_command_pool;
    Array<VkCommandBuffer> m_command_buffers;

    Array<VkSemaphore> m_image_available_semaphores;
    Array<VkSemaphore> m_render_finished_semaphores;
    Array<VkFence> m_in_flight_fences;

    VmaAllocator m_allocator;

    VkBuffer m_vertex_buffer;
    VmaAllocation m_vertex_buffer_memory;
    VkBuffer m_index_buffer;
    VmaAllocation m_index_buffer_memory;

    VkDescriptorPool m_descriptor_pool;
    Array<VkDescriptorSet> m_descriptor_sets;

    Array<VkBuffer> m_uniform_buffers;
    Array<VmaAllocation> m_uniform_buffer_memories;
  private:
    friend class VulkanRenderDriver;
  };

}
