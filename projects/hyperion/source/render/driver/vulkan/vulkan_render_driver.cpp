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

  }

  //--------------------------------------------------------------
  void VulkanRenderDriver::Setup(VulkanRenderContext *context) {
    m_context = context;
  }

}