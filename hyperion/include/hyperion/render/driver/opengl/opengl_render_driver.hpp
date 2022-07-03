//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/render_frame.hpp"
#include "hyperion/render/driver/render_driver.hpp"
#include "hyperion/render/driver/opengl/opengl_storage.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct GroupedObject {
    Matrix4x4 local_to_world;
    uint32 sub_mesh_index = 0;

    uint32 light_count = 0;
    uint32 light_indices[4] = { };
  };

  struct GroupedMesh {
    const OpenGLMesh *mesh = nullptr;

    Array<GroupedObject> objects;
  };

  struct GroupedMaterial {
    const OpenGLMaterial *material = nullptr;

    Map<AssetHandle, GroupedMesh> meshes;
  };

  struct GroupedShader {
    const OpenGLShader *shader = nullptr;

    Map<AssetHandle, GroupedMaterial> materials;
  };

  struct OpenGLGlobalBuffer {
    ShaderPropertyId id = 0;
    GLuint buffer = 0;
    uint64 buffer_size = 0;
  };
  
  struct OpenGLGlobalProperties {
    Map<ShaderPropertyId, OpenGLGlobalBuffer> buffers;
  };
  
  struct OpenGLState {
    uint64 camera_index = 0;

    OpenGLGlobalProperties global_properties = { };
  };

  class OpenGLRenderDriver final : public RenderDriver {
  public:
    void Initialize() override;
    void HandleAssets(RenderAssetContext &asset_context) override;
    void Render(RenderFrame *render_frame) override;
    void Shutdown() override;
  private:
    void ExecuteRenderFrame(RenderFrame *render_frame);
    
    void ExecuteFrameCommandSetCamera(const RenderPipelineCommandSetCamera &command, const RenderObjectContext &context);
    void ExecuteFrameCommandExecuteCommandBuffer(const RenderPipelineCommandExecuteCommandBuffer &command, RenderFrame *render_frame);
    void ExecuteFrameCommandDrawMeshes(const RenderPipelineCommandDrawMeshes &command, RenderFrame *render_frame, const RenderObjectContext &context);
    void ExecuteFrameCommandDrawShadows(const RenderPipelineCommandDrawShadows &command, const RenderObjectContext &context);
    void ExecuteFrameCommandDrawUI(const RenderPipelineCommandDrawUI &command, const RenderObjectContext &context);
    void ExecuteFrameCommandDrawObjectIds(const RenderPipelineCommandDrawObjectIds &command, const RenderObjectContext &context);
    void ExecuteFrameCommandDrawGizmos(const RenderPipelineCommandDrawGizmos &command, const RenderObjectContext &context);
    void ExecuteFrameCommandDrawEditorUI(const RenderPipelineCommandDrawEditorUI &command, const RenderObjectContext &context);

    void ExecuteBufferCommandClearRenderTarget(const RenderCommandBufferCommandClearRenderTarget &command);
    void ExecuteBufferCommandSetRenderTarget(const RenderCommandBufferCommandSetRenderTarget &command);
    void ExecuteBufferCommandBlit(const RenderCommandBufferCommandBlit &command);
    void ExecuteBufferCommandSetGlobalBuffer(const RenderCommandBufferCommandSetGlobalBuffer &command);
    void ExecuteBufferCommandRequestAsyncReadback(const RenderCommandBufferCommandRequestAsyncReadback &command, RenderFrame *render_frame);
    
    void PrepareObjects(RenderFrame *render_frame, const Array<uint32> &sorted_objects, DrawingParameters drawing_parameters);
    void SetupPerObjectLightIndices(const RenderObjectContext &render_object_context, GroupedObject &grouped_object, Vector3 object_position);
    void DrawMeshes(const RenderObjectContextEnvironment &environment, const Array<RenderObjectContextLight> &lights, DrawingParameters drawing_parameters);
    void DrawSubMesh(const SubMesh &sub_mesh);
    void DrawUI(const Array<RenderObjectContextUIElement> &elements);
    void DrawRenderBounds(const BoundingBox &bounds);

    void UseRenderTexture(RenderTargetId render_target_id);
    void UseShader(const OpenGLShader &opengl_shader);
    void UseMaterial(const OpenGLShader &opengl_shader, const OpenGLMaterial &opengl_material);
    void SetMaterialTextureProperty(ShaderPropertyStorage::Texture texture_property, uint32 texture_unit, GLuint program, GLint location);
    void UseMesh(const OpenGLMesh &opengl_mesh);
  private:
    OpenGLState m_state = OpenGLState();
    OpenGLStorage m_storage = OpenGLStorage();

    Array<GroupedShader> m_grouped_shaders;
  };

}
