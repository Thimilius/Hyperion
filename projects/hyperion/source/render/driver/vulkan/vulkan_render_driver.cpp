//------------------------ Header Guard ------------------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/vulkan/vulkan_render_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/vulkan/vulkan_render_context.hpp"
#include "hyperion/render/driver/vulkan/vulkan_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void VulkanRenderDriver::Initialize() {

  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::HandleAssets(RenderAssetContext &asset_context) {

  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Render(RenderFrame *render_frame) {

  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Shutdown() {
    vkDestroyDevice(m_device, nullptr);
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Setup(VulkanRenderContext *context) {
    m_context = context;

    CreateLogicalDevice();
  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::CreateLogicalDevice() {
    VkPhysicalDevice physical_device = m_context->GetPhysicalDevice();
    auto indices = m_context->FindQueueFamilyIndices(physical_device);
    
    VkDeviceQueueCreateInfo queue_create_info = { };
    queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_create_info.queueFamilyIndex = indices.graphics_family;
    queue_create_info.queueCount = 1;
    float32 queue_priority = 1.0f;
    queue_create_info.pQueuePriorities = &queue_priority;

    VkPhysicalDeviceFeatures physical_device_features = { };

    VkDeviceCreateInfo device_create_info = { };
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = &queue_create_info;
    device_create_info.queueCreateInfoCount = 1;
    device_create_info.pEnabledFeatures = &physical_device_features;

    HYP_VULKAN_CHECK(vkCreateDevice(physical_device, &device_create_info, nullptr, &m_device), "Failed to create logical device!");

    vkGetDeviceQueue(m_device, indices.graphics_family, 0, &m_graphics_queue);
  }

}