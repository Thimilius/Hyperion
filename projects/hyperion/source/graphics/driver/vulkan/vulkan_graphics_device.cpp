//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_device.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/driver/vulkan/vulkan_graphics_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Graphics {

    //--------------------------------------------------------------
    VulkanGraphicsDevice::VulkanGraphicsDevice(VkDevice device, VkQueue graphics_queue) {
        m_device = device;
        m_graphics_queue = graphics_queue;
    }

    //--------------------------------------------------------------
    VulkanGraphicsDevice::~VulkanGraphicsDevice() {
        vkDestroyDevice(m_device, nullptr);
    }

}