//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

#include "hyperion/render/assets/render_assets_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class OpenGLShaderUniformLocation {
    Model,
    Color,
    Texture,
    ObjectId,
    LightCount,
    LightIndices,
    LightSpace,
    ShadowMap,

    Last
  };
  
  struct OpenGLAsset {
    AssetHandle handle;
  };

  struct OpenGLShader : public OpenGLAsset {
    ShaderAttributes attributes;

    Array<GLint> material_locations;
    GLuint program;

    GLint fixed_locations[OpenGLShaderUniformLocation::Last];
  };

  struct OpenGLTexture : public OpenGLAsset {
    GLuint texture;
  };

  struct OpenGLRenderTextureAttachment {
    RenderTextureFormat format;

    GLuint attachment;
  };

  struct OpenGLRenderTexture : public OpenGLAsset {
    uint32 width;
    uint32 height;
    Array<OpenGLRenderTextureAttachment> attachments;

    GLuint framebuffer;
  };

  struct OpenGLMaterial : public OpenGLAsset {
    MaterialPropertyCollection properties;

    OpenGLShader *shader;
  };

  struct OpenGLMesh : public OpenGLAsset {
    Array<SubMesh> sub_meshes;

    GLsizei index_count;
    GLuint vertex_buffer;
    GLuint index_buffer;
    GLuint vertex_array;

    uint64 vertex_buffer_size;
    uint64 index_buffer_size;
  };

  struct OpenGLStaticStorage {
    OpenGLShader error_shader;

    OpenGLShader object_id_shader;

    OpenGLShader shadow_shader;
    
    GLuint fullscreen_shader;
    GLuint fullscreen_vertex_array;

    GLuint ui_shader;

    GLuint camera_uniform_buffer;

    GLuint render_bounds_vertex_buffer = -1;
    GLuint render_bounds_index_buffer;
    GLuint render_bounds_vertex_array;
  };

  struct OpenGLUniformBufferCamera {
    Matrix4x4 camera_view_matrix;
    Matrix4x4 camera_projection_matrix;
  };

  struct OpenGLImmediateVertex {
    Vector3 position;
    Color color;
  };

  class OpenGLStorage final {
  public:
    inline const OpenGLTexture &GetTexture2D(AssetHandle handle) const { return m_textures.Get(handle); }
    inline const OpenGLRenderTexture &GetRenderTexture(AssetHandle handle) const { return m_render_textures.Get(handle); }
    inline const OpenGLShader &GetShader(AssetHandle handle) const { return m_shaders.Get(handle); }
    inline const OpenGLMaterial &GetMaterial(AssetHandle handle) const { return m_materials.Get(handle); }
    inline const OpenGLMesh &GetMesh(AssetHandle handle) const { return m_meshes.Get(handle); }

    inline const OpenGLStaticStorage &GetStatic() const { return m_static; }

    inline auto FindTexture2D(AssetHandle handle) const { return m_textures.Find(handle); }
    inline auto GetTexture2DEnd() const { return m_textures.end(); }
    inline auto FindRenderTexture(AssetHandle handle) const { return m_render_textures.Find(handle); }
    inline auto GetRenderTextureEnd() const { return m_render_textures.end(); }
    inline auto FindShader(AssetHandle handle) const { return m_shaders.Find(handle); }
    inline auto GetShaderEnd() const { return m_shaders.end(); }

    void Initialize();

    void LoadAssets(RenderAssetContext &asset_context);
    void LoadTexture2D(RenderAssetTexture2D &texture_2d);
    void LoadRenderTexture(RenderAssetRenderTexture &render_texture);
    void LoadShader(RenderAssetShader &shader);
    void LoadMaterial(RenderAssetMaterial &material);
    void LoadMesh(RenderAssetMesh &mesh);
    void UnloadAssets(RenderAssetContext &asset_context);
    void UnloadTexture2D(AssetHandle texture_2d_handle);
    void UnloadRenderTexture(AssetHandle render_texture_handle);
    void UnloadShader(AssetHandle shader_handle);
    void UnloadMaterial(AssetHandle material_handle);
    void UnloadMesh(AssetHandle mesh_handle);

    void SetTextureAttributes(GLuint texture, TextureAttributes attributes);
  private:
    Map<AssetHandle, OpenGLTexture> m_textures;
    Map<AssetHandle, OpenGLRenderTexture> m_render_textures;
    Map<AssetHandle, OpenGLShader> m_shaders;
    Map<AssetHandle, OpenGLMaterial> m_materials;
    Map<AssetHandle, OpenGLMesh> m_meshes;

    OpenGLStaticStorage m_static = OpenGLStaticStorage();
  };

}
