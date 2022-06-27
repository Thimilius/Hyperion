//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/types/render_types_general.hpp"
#include "hyperion/render/types/render_types_mesh.hpp"
#include "hyperion/render/types/render_types_texture.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class OpenGLUtilities final {
  public:
    static GLbitfield GetClearFlags(ClearFlags clear_flags);

    static uint32 GetBytesPerPixelForTextureFormat(TextureFormat format);
    static void SetUnpackAlignmentForTextureFormat(TextureFormat format);
    static void FlipTextureHorizontally(uint32 width, uint32 height, TextureFormat format, Array<byte> &pixels);
    static GLenum GetTextureFormat(TextureFormat format);
    static GLenum GetTextureFormatType(TextureFormat format_type);
    static GLenum GetTextureInternalFormat(TextureFormat internal_format);
    static GLint GetTextureWrapMode(TextureWrapMode wrap_mode);
    static GLint GetTextureMinFilter(TextureFilter filter);
    static GLint GetTextureMaxFilter(TextureFilter filter);
    static GLfloat GetTextureAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter);

    static bool8 IsRenderTextureFormatAColor(RenderTextureFormat format);
    static GLenum GetRenderTextureAttachmentType(RenderTextureFormat format);
    static GLenum GetRenderTextureInternalFormat(RenderTextureFormat internal_format);
    static GLenum GetRenderTextureFormat(RenderTextureFormat format);
    static GLenum GetRenderTextureFormatType(RenderTextureFormat format_type);
    static GLsizei GetRenderTextureBufferSize(RectInt region, RenderTextureFormat format);

    static GLenum GetTopology(MeshTopology mesh_topology);
    static GLuint GetAttributeIndexForVertexAttributeSize(VertexAttributeKind kind);
    static GLenum GetVertexAttributeType(VertexAttributeType vertex_attribute_type);
    static GLuint GetVertexAttributeSizeForVertexAttribute(VertexAttributeType vertex_attribute_type, uint32 dimension);
  private:
    OpenGLUtilities() = delete;
    ~OpenGLUtilities() = delete;
  };

}
