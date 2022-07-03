//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/object/render_object_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderObjectContext::Clear() {
    m_cameras.Clear();
    m_lights.Clear();
    m_meshes.Clear();
    m_sprites.Clear();
    m_ui_elements.Clear();
    m_editor_ui_elements.Clear();
  }

  //--------------------------------------------------------------
  RenderObjectContextCamera &RenderObjectContext::AddCamera() {
    m_cameras.Resize(m_cameras.GetLength() + 1);
    return m_cameras.GetLast();
  }

  //--------------------------------------------------------------
  RenderObjectContextLight &RenderObjectContext::AddLight() {
    m_lights.Resize(m_lights.GetLength() + 1);
    return m_lights.GetLast();
  }

  //--------------------------------------------------------------
  RenderObjectContextMesh &RenderObjectContext::AddMesh() {
    m_meshes.Resize(m_meshes.GetLength() + 1);
    return m_meshes.GetLast();
  }

  //--------------------------------------------------------------
  RenderObjectContextSprite &RenderObjectContext::AddSprite() {
    m_sprites.Resize(m_sprites.GetLength() + 1);
    return m_sprites.GetLast();
  }

  //--------------------------------------------------------------
  RenderObjectContextUIElement &RenderObjectContext::AddUIElement() {
    m_ui_elements.Resize(m_ui_elements.GetLength() + 1);
    return m_ui_elements.GetLast();
  }

  //--------------------------------------------------------------
  RenderObjectContextUIElement &RenderObjectContext::AddEditorUIElement() {
    m_editor_ui_elements.Resize(m_editor_ui_elements.GetLength() + 1);
    return m_editor_ui_elements.GetLast();
  }

}
