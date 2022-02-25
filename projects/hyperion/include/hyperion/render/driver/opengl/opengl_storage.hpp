//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

#include "hyperion/render/assets/render_assets_context.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  struct OpenGLAsset {
    AssetId id;
  };

  struct OpenGLShader : public OpenGLAsset {
    ShaderAttributes attributes;

    Array<GLint> locations;
    GLuint program;
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
    uint32 color_attachment_count = 0;
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
  };

  struct OpenGLStaticStorage {
    OpenGLShader error_shader;

    OpenGLShader object_id_shader;

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
    inline const OpenGLTexture &GetTexture2D(AssetId id) const { return m_textures.Get(id); };
    inline const OpenGLRenderTexture &GetRenderTexture(AssetId id) const { return m_render_textures.Get(id); };
    inline const OpenGLShader &GetShader(AssetId id) const { return m_shaders.Get(id); };
    inline const OpenGLMaterial &GetMaterial(AssetId id) const { return m_materials.Get(id); };
    inline const OpenGLMesh &GetMesh(AssetId id) const { return m_meshes.Get(id); };

    inline const OpenGLStaticStorage &GetStatic() const { return m_static; }

    inline auto FindTexture2D(AssetId id) const { return m_textures.Find(id); }
    inline auto GetTexture2DEnd() const { return m_textures.end(); }
    inline auto FindRenderTexture(AssetId id) const { return m_render_textures.Find(id); }
    inline auto GetRenderTextureEnd() const { return m_render_textures.end(); }
    inline auto FindShader(AssetId id) const { return m_shaders.Find(id); }
    inline auto GetShaderEnd() const { return m_shaders.end(); }

    void Initialize();

    void LoadAssets(RenderAssetContext &asset_context);
    void LoadTexture2D(RenderAssetTexture2D &texture_2d);
    void LoadRenderTexture(RenderAssetRenderTexture &render_texture);
    void LoadShader(RenderAssetShader &shader);
    void LoadMaterial(RenderAssetMaterial &material);
    void LoadMesh(RenderAssetMesh &mesh);
    void UnloadAssets(RenderAssetContext &asset_context);
    void UnloadRenderTexture(AssetId render_texture_id);

    void SetTextureAttributes(GLuint texture, TextureAttributes attributes);
  private:
    Map<AssetId, OpenGLTexture> m_textures;
    Map<AssetId, OpenGLRenderTexture> m_render_textures;
    Map<AssetId, OpenGLShader> m_shaders;
    Map<AssetId, OpenGLMaterial> m_materials;
    Map<AssetId, OpenGLMesh> m_meshes;

    OpenGLStaticStorage m_static;
  };

}
