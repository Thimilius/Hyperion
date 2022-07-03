//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/object/render_object_context_camera.hpp"
#include "hyperion/render/object/render_object_context_environment.hpp"
#include "hyperion/render/object/render_object_context_light.hpp"
#include "hyperion/render/object/render_object_context_objects.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
  class RenderFrame;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderObjectContext final : public INonCopyable {
  public:
    void Clear();

    inline RenderObjectContextEnvironment &GetEnvironment() { return m_environment; }
    inline const RenderObjectContextEnvironment &GetEnvironment() const { return m_environment; }
    inline const Array<RenderObjectContextCamera> &GetCameras() const { return m_cameras; }
    inline const Array<RenderObjectContextLight> &GetLights() const { return m_lights; }
    inline const Array<RenderObjectContextMesh> &GetMeshes() const { return m_meshes; }
    inline const Array<RenderObjectContextSprite> &GetSprites() const { return m_sprites; }
    inline const Array<RenderObjectContextUIElement> &GetUIElements() const { return m_ui_elements; }
    inline const Array<RenderObjectContextUIElement> &GetEditorUIElements() const { return m_editor_ui_elements; }

    RenderObjectContextCamera &AddCamera();
    RenderObjectContextLight &AddLight();
    RenderObjectContextMesh &AddMesh();
    RenderObjectContextSprite &AddSprite();
    RenderObjectContextUIElement &AddUIElement();
    RenderObjectContextUIElement &AddEditorUIElement();
  private:
    RenderFrame *m_render_frame = nullptr;
    
    RenderObjectContextEnvironment m_environment = { };
    Array<RenderObjectContextCamera> m_cameras;
    Array<RenderObjectContextLight> m_lights;
    Array<RenderObjectContextMesh> m_meshes;
    Array<RenderObjectContextSprite> m_sprites;
    Array<RenderObjectContextUIElement> m_ui_elements;
    Array<RenderObjectContextUIElement> m_editor_ui_elements;
  private:
    friend class Hyperion::Rendering::RenderFrame;
  };

}
