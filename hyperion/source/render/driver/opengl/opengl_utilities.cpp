//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/driver/opengl/opengl_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  GLbitfield OpenGLUtilities::GetClearFlags(ClearFlags clear_flags) {
    GLbitfield result = 0;
    if ((clear_flags & ClearFlags::Color) == ClearFlags::Color) {
      result |= GL_COLOR_BUFFER_BIT;
    }
    if ((clear_flags & ClearFlags::Depth) == ClearFlags::Depth) {
      result |= GL_DEPTH_BUFFER_BIT;
    }
    if ((clear_flags & ClearFlags::Stencil) == ClearFlags::Stencil) {
      result |= GL_STENCIL_BUFFER_BIT;
    }
    return result;
  }

  //--------------------------------------------------------------
  uint32 OpenGLUtilities::GetBytesPerPixelForTextureFormat(TextureFormat format) {
    switch (format) {
      case TextureFormat::RGBA32: return 4;
      case TextureFormat::RGB24: return 3;
      case TextureFormat::R8: return 1;
      case TextureFormat::None:
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  void OpenGLUtilities::SetUnpackAlignmentForTextureFormat(TextureFormat format) {
    GLint alignment = 4;
    switch (format) {
      case TextureFormat::RGB24: alignment = 4; break;
      case TextureFormat::RGBA32: alignment = 4; break;
      case TextureFormat::R8: alignment = 1; break;
      case TextureFormat::None:
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
  }

  //--------------------------------------------------------------
  void OpenGLUtilities::FlipTextureHorizontally(uint32 width, uint32 height, TextureFormat format, Array<byte> &pixels) {
    uint32 stride = width * GetBytesPerPixelForTextureFormat(format);
    Array<byte> temp_buffer(stride);
    byte *temp_buffer_data = temp_buffer.GetData();

    byte *data = pixels.GetData();

    for (uint32 row = 0; row < height / 2; row++) {
      byte *source = data + (row * stride);
      byte *destination = data + (((height - 1) - row) * stride);

      std::memcpy(temp_buffer_data, source, stride);
      std::memcpy(source, destination, stride);
      std::memcpy(destination, temp_buffer_data, stride);
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetTextureFormat(TextureFormat format) {
    switch (format) {
      case TextureFormat::RGB24: return GL_RGB;
      case TextureFormat::RGBA32: return GL_RGBA;
      case TextureFormat::R8: return GL_RED;
      case TextureFormat::None:
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetTextureFormatType(TextureFormat format_type) {
    switch (format_type) {
      case TextureFormat::RGB24: return GL_UNSIGNED_BYTE;
      case TextureFormat::RGBA32: return GL_UNSIGNED_BYTE;
      case TextureFormat::R8: return GL_UNSIGNED_BYTE;
      case TextureFormat::None:
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetTextureInternalFormat(TextureFormat internal_format) {
    switch (internal_format) {
      case TextureFormat::RGB24: return GL_RGB8;
      case TextureFormat::RGBA32: return GL_RGBA8;
      case TextureFormat::R8: return GL_R8;
      case TextureFormat::None:
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLint OpenGLUtilities::GetTextureWrapMode(TextureWrapMode wrap_mode) {
    switch (wrap_mode) {
      case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
      case TextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
      case TextureWrapMode::Repeat: return GL_REPEAT;
      case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLint OpenGLUtilities::GetTextureMinFilter(TextureFilter filter) {
    switch (filter) {
      case TextureFilter::Point: return GL_NEAREST_MIPMAP_NEAREST;
      case TextureFilter::Bilinear: return GL_LINEAR_MIPMAP_NEAREST;
      case TextureFilter::Trilinear: return GL_LINEAR_MIPMAP_LINEAR;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST_MIPMAP_NEAREST;
    }
  }

  //--------------------------------------------------------------
  GLint OpenGLUtilities::GetTextureMaxFilter(TextureFilter filter) {
    switch (filter) {
      case TextureFilter::Point: return GL_NEAREST;
      case TextureFilter::Bilinear: return GL_LINEAR;
      case TextureFilter::Trilinear: return GL_LINEAR;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST;
    }
  }

  //--------------------------------------------------------------
  GLfloat OpenGLUtilities::GetTextureAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
    switch (anisotropic_filter) {
      case TextureAnisotropicFilter::None: return 1.0f;
      case TextureAnisotropicFilter::Times2: return 2.0f;
      case TextureAnisotropicFilter::Times4: return 4.0f;
      case TextureAnisotropicFilter::Times8: return 8.0f;
      case TextureAnisotropicFilter::Times16: return 16.0f;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return 1.0f;
    }
  }

  //--------------------------------------------------------------
  bool8 OpenGLUtilities::IsRenderTextureFormatAColor(RenderTextureFormat format) {
    switch (format) {
      case RenderTextureFormat::RGBA8:
      case RenderTextureFormat::R32UInt:
      case RenderTextureFormat::RG32UInt: return true;
      case RenderTextureFormat::Depth24:
      case RenderTextureFormat::Depth24Stencil8: return false;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return false;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetRenderTextureAttachmentType(RenderTextureFormat format) {
    switch (format) {
      case RenderTextureFormat::RGBA8:
      case RenderTextureFormat::R32UInt:
      case RenderTextureFormat::RG32UInt: return GL_COLOR_ATTACHMENT0;
      case RenderTextureFormat::Depth24: return GL_DEPTH_ATTACHMENT;
      case RenderTextureFormat::Depth24Stencil8: return GL_DEPTH_STENCIL_ATTACHMENT;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return false;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetRenderTextureInternalFormat(RenderTextureFormat internal_format) {
    switch (internal_format) {
      case RenderTextureFormat::RGBA8: return GL_RGBA8;
      case RenderTextureFormat::R32UInt: return GL_R32UI;
      case RenderTextureFormat::RG32UInt: return GL_RG32UI;
      case RenderTextureFormat::Depth24: return GL_DEPTH_COMPONENT24;
      case RenderTextureFormat::Depth24Stencil8: return GL_DEPTH24_STENCIL8;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return 0;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetRenderTextureFormat(RenderTextureFormat format) {
    // FIXME: Missing formats.
    switch (format) {
      case RenderTextureFormat::RGBA8: return GL_RGBA;
      case RenderTextureFormat::R32UInt: return GL_RED_INTEGER;
      case RenderTextureFormat::RG32UInt: return GL_RG_INTEGER;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return 0;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetRenderTextureFormatType(RenderTextureFormat format_type) {
    // FIXME: Missing formats.
    switch (format_type) {
      case RenderTextureFormat::RGBA8: return GL_UNSIGNED_BYTE;
      case RenderTextureFormat::R32UInt: return GL_UNSIGNED_INT;
      case RenderTextureFormat::RG32UInt: return GL_UNSIGNED_INT;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return 0;
    }
  }

  //--------------------------------------------------------------
  GLsizei OpenGLUtilities::GetRenderTextureBufferSize(RectInt region, RenderTextureFormat format) {
    // FIXME: Missing formats.
    GLsizei region_size = region.width * region.height;
    switch (format) {
      case RenderTextureFormat::RGBA8: return region_size * 4 * sizeof(byte);
      case RenderTextureFormat::R32UInt: return region_size * 1 * sizeof(uint32);
      case RenderTextureFormat::RG32UInt: return region_size * 2 * sizeof(uint32);
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return 0;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetBufferUsage(MeshUpdateType update_type) {
    switch (update_type) {
      case MeshUpdateType::Static: return GL_STATIC_DRAW;
      case MeshUpdateType::Dynamic: return GL_STREAM_DRAW;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_STATIC_DRAW;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetTopology(MeshTopology mesh_topology) {
    switch (mesh_topology) {
      case MeshTopology::Points: return GL_POINTS;
      case MeshTopology::Lines: return GL_LINES;
      case MeshTopology::LineStrip: return GL_LINE_STRIP;
      case MeshTopology::Triangles: return GL_TRIANGLES;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLuint OpenGLUtilities::GetAttributeIndexForVertexAttributeSize(VertexAttributeKind kind) {
    switch (kind) {
      case VertexAttributeKind::Position: return 0;
      case VertexAttributeKind::Normal: return 1;
      case VertexAttributeKind::Tangent: return 2;
      case VertexAttributeKind::Color: return 3;
      case VertexAttributeKind::Texture0: return 4;
      case VertexAttributeKind::Texture1: return 5;
      case VertexAttributeKind::Texture2: return 6;
      case VertexAttributeKind::Texture3: return 7;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLenum OpenGLUtilities::GetVertexAttributeType(VertexAttributeType vertex_attribute_type) {
    switch (vertex_attribute_type) {
      case VertexAttributeType::Float32: return GL_FLOAT;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

  //--------------------------------------------------------------
  GLuint OpenGLUtilities::GetVertexAttributeSizeForVertexAttribute(VertexAttributeType vertex_attribute_type, uint32 dimension) {
    switch (vertex_attribute_type) {
      case VertexAttributeType::Float32: return sizeof(float32) * dimension;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
    }
  }

}
