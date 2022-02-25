//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/render/frame/context/render_frame_context_camera.hpp"
#include "hyperion/render/frame/context/render_frame_context_environment.hpp"
#include "hyperion/render/frame/context/render_frame_context_light.hpp"
#include "hyperion/render/frame/context/render_frame_context_objects.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderFrameContext final : public INonCopyable {
  public:
    void Clear();

    inline RenderFrameContextEnvironment &GetEnvironment() { return m_environment; }
    inline const RenderFrameContextEnvironment &GetEnvironment() const { return m_environment; }
    inline const Array<RenderFrameContextCamera> &GetCameras() const { return m_cameras; }
    inline const Array<RenderFrameContextLight> &GetLights() const { return m_lights; }
    inline const Array<RenderFrameContextObjectMesh> &GetMeshObjects() const { return m_mesh_objects; }
    inline const Array<RenderFrameContextObjectSprite> &GetSpriteObjects() const { return m_sprite_objects; }
    inline const Array<RenderFrameContextObjectUI> &GetUIObjects() const { return m_ui_objects; }
    inline const Array<RenderFrameContextObjectUI> &GetEditorUIObjects() const { return m_editor_ui_objects; }

    RenderFrameContextCamera &AddCamera();
    RenderFrameContextLight &AddLight();
    RenderFrameContextObjectMesh &AddMeshObject();
    RenderFrameContextObjectSprite &AddSpriteObject();
    RenderFrameContextObjectUI &AddUIObject();
    RenderFrameContextObjectUI &AddEditorUIObject();
  private:
    RenderFrameContextEnvironment m_environment;
    Array<RenderFrameContextCamera> m_cameras;
    Array<RenderFrameContextLight> m_lights;
    Array<RenderFrameContextObjectMesh> m_mesh_objects;
    Array<RenderFrameContextObjectSprite> m_sprite_objects;
    Array<RenderFrameContextObjectUI> m_ui_objects;
    Array<RenderFrameContextObjectUI> m_editor_ui_objects;
  };

}