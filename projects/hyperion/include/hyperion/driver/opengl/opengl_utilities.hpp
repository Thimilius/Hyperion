#pragma once

#include <glad/glad.h>

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLUtilities final {
    public:
        static GLbitfield GetGLClearFlags(ClearFlags clear_flags);

        static GLenum GetGLDepthEquation(DepthEquation depth_equation);
        static GLenum GetGLBlendingFactor(BlendingFactor blending_factor);
        static GLenum GetGLBlendingEquation(BlendingEquation blending_equation);
        static GLenum GetGLCullingMode(CullingMode culling_mode);
        static GLenum GetGLCullingFrontFaceMode(CullingFrontFaceMode culling_front_face_mode);
        static GLenum GetGLPolygonMode(PolygonMode polygon_mode);

        static void SetUnpackAlignmentForTextureFormat(TextureFormat format);
        static void FlipTextureHorizontally(TextureSize size, TextureFormat format, ArrayDescriptor<uint8> pixels);
        static GLenum GetGLTextureFormat(TextureFormat format);
        static GLenum GetGLTextureFormatType(TextureFormat format_type);
        static GLenum GetGLTextureInternalFormat(TextureFormat internal_format);
        static GLint GetGLTextureWrapMode(TextureWrapMode wrap_mode);
        static GLint GetGLTextureMinFilter(TextureFilter filter);
        static GLint GetGLTextureMaxFilter(TextureFilter filter);
        static GLfloat GetGLTextureAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter);

        static MaterialPropertyType GetMaterialPropertyTypeForGLShaderType(GLint type);

        static GLenum GetGLIndexFormat(IndexFormat index_format);
        static GLsizei GetGLIndexFormatSize(IndexFormat index_format);
        static GLenum GetGLMeshTopology(MeshTopology mesh_topology);
        static GLenum GetGLVertexAttributeType(VertexAttributeType vertex_attribute_type);
        static GLuint GetGLSizeForVertexAttribute(VertexAttributeType vertex_attribute_type, uint32 dimension);
    private:
        static uint32 GetBytesPerPixelForTextureFormat(TextureFormat format);
    };

}