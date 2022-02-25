//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/frame/context/render_frame_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderFrameContext::Clear() {
    m_cameras.Clear();
    m_lights.Clear();
    m_mesh_objects.Clear();
    m_sprite_objects.Clear();
    m_ui_objects.Clear();
    m_editor_ui_objects.Clear();
  }

  //--------------------------------------------------------------
  RenderFrameContextCamera &RenderFrameContext::AddCamera() {
    m_cameras.Resize(m_cameras.GetLength() + 1);
    return m_cameras.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextLight &RenderFrameContext::AddLight() {
    m_lights.Resize(m_lights.GetLength() + 1);
    return m_lights.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectMesh &RenderFrameContext::AddMeshObject() {
    m_mesh_objects.Resize(m_mesh_objects.GetLength() + 1);
    return m_mesh_objects.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectSprite &RenderFrameContext::AddSpriteObject() {
    m_sprite_objects.Resize(m_sprite_objects.GetLength() + 1);
    return m_sprite_objects.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectUI &RenderFrameContext::AddUIObject() {
    m_ui_objects.Resize(m_ui_objects.GetLength() + 1);
    return m_ui_objects.GetLast();
  }

  //--------------------------------------------------------------
  RenderFrameContextObjectUI &RenderFrameContext::AddEditorUIObject() {
    m_editor_ui_objects.Resize(m_editor_ui_objects.GetLength() + 1);
    return m_editor_ui_objects.GetLast();
  }

}