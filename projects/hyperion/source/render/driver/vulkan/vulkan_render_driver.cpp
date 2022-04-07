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
    
    VulkanQueueFamilyIndices indices = m_context->FindQueueFamilyIndices(physical_device);
    Set<int32> unique_queue_indices = { indices.graphics_family, indices.presentation_family };
    Array<VkDeviceQueueCreateInfo> queue_create_infos;
    queue_create_infos.Reserve(unique_queue_indices.GetLength());

    float32 queue_priority = 1.0f;
    for (int32 queue_family : unique_queue_indices) {
      VkDeviceQueueCreateInfo queue_create_info = { };
      queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
      queue_create_info.queueFamilyIndex = static_cast<uint32>(queue_family);
      queue_create_info.queueCount = 1;
      queue_create_info.pQueuePriorities = &queue_priority;
      queue_create_infos.Add(queue_create_info);
    }

    VkPhysicalDeviceFeatures physical_device_features = { };

    VkDeviceCreateInfo device_create_info = { };
    device_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pQueueCreateInfos = queue_create_infos.GetData();
    device_create_info.queueCreateInfoCount = static_cast<uint32>(queue_create_infos.GetLength());
    device_create_info.pEnabledFeatures = &physical_device_features;

    HYP_VULKAN_CHECK(vkCreateDevice(physical_device, &device_create_info, nullptr, &m_device), "Failed to create logical device!");

    vkGetDeviceQueue(m_device, indices.graphics_family, 0, &m_graphics_queue);
    vkGetDeviceQueue(m_device, indices.presentation_family, 0, &m_presentation_queue);
  }

}