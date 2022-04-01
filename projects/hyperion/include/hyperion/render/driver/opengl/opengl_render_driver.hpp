//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/render_driver.hpp"
#include "hyperion/render/driver/opengl/opengl_storage.hpp"
#include "hyperion/render/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct GroupedObject {
    Matrix4x4 local_to_world;
    uint32 sub_mesh_index;

    uint32 light_count;
    uint32 light_indices[4];
  };

  struct GroupedMesh {
    const OpenGLMesh *mesh;

    Array<GroupedObject> objects;
  };

  struct GroupedMaterial {
    const OpenGLMaterial *material;

    Map<AssetId, GroupedMesh> meshes;
  };

  struct GroupedShader {
    const OpenGLShader *shader;

    Map<AssetId, GroupedMaterial> materials;
  };

  struct OpenGLState {
    uint64 camera_index;
    GLuint lighting_uniform_buffer = -1;
  };

  class OpenGLRenderDriver final : public RenderDriver {
  public:
    void Initialize() override;
    void HandleAssets(RenderAssetContext &asset_context) override;
    void Render(RenderFrame *render_frame) override;
    void Shutdown() override;
  private:
    void ExecuteRenderFrame(RenderFrame *render_frame);
    void ExecuteCommandBuffer(RenderFrame *render_frame, const RenderCommandBuffer &command_buffer);
    void PrepareObjects(RenderFrame *render_frame, const Array<uint32> &sorted_objects, DrawingParameters drawing_parameters);
    void SetupPerObjectLightIndices(const RenderFrameContext &render_frame_context, GroupedObject &grouped_object, Vector3 object_position);
    void DrawMeshes(const RenderFrameContextEnvironment &environment, const Array<RenderFrameContextLight> &lights, DrawingParameters drawing_parameters);
    void DrawSubMesh(const SubMesh &sub_mesh);
    void DrawUI(const Array<RenderFrameContextObjectUI> &elements);
    void DrawObjectIds(const RenderFrameContext &render_frame_context, RenderTargetId render_target_id);
    void DrawRenderBounds(const BoundingBox &bounds);

    void UseRenderTexture(RenderTargetId render_target_id);
    void UseShader(const OpenGLShader &opengl_shader);
    void UseMaterial(const OpenGLShader &opengl_shader, const OpenGLMaterial &opengl_material);
    void SetMaterialTextureProperty(ShaderPropertyStorage::Texture texture_property, uint32 texture_unit, GLuint program, GLuint location);
    void UseMesh(const OpenGLMesh &opengl_mesh);
  private:
    OpenGLState m_static;
    OpenGLStorage m_storage;

    Array<GroupedShader> m_grouped_shaders;
  };

}