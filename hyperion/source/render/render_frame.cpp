//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  RenderFrame::RenderFrame() {
    m_asset_context.m_render_frame = this;
    m_object_context.m_render_frame = this;
    m_pipeline_context.m_render_frame = this;
  }
  
  //--------------------------------------------------------------
  void RenderFrame::Clear() {
    m_asset_context.Clear();
    m_object_context.Clear();
    m_pipeline_context.Clear();
  }

}
