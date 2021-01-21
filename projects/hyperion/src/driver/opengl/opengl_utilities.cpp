#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_utilities.hpp"

namespace Hyperion::Rendering {

    GLbitfield OpenGLUtilities::GetGLClearFlags(ClearFlags clear_flags) {
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

    GLenum OpenGLUtilities::GetGLDepthEquation(DepthEquation depth_equation) {
        switch (depth_equation) {
            case DepthEquation::Never: return GL_NEVER;
            case DepthEquation::Always: return GL_ALWAYS;
            case DepthEquation::Less: return GL_LESS;
            case DepthEquation::LessEqual: return GL_LEQUAL;
            case DepthEquation::Greater: return GL_GREATER;
            case DepthEquation::GreaterEqual: return GL_GEQUAL;
            case DepthEquation::Equal: return GL_EQUAL;
            case DepthEquation::NotEqual: return GL_NOTEQUAL;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLBlendingFactor(BlendingFactor blending_factor) {
        switch (blending_factor) {
            case BlendingFactor::Zero: return GL_ZERO;
            case BlendingFactor::One: return GL_ONE;
            case BlendingFactor::SourceAlpha: return GL_SRC_ALPHA;
            case BlendingFactor::SourceColor: return GL_SRC_COLOR;
            case BlendingFactor::DestinationAlpha: return GL_DST_ALPHA;
            case BlendingFactor::DestinationColor: return GL_DST_COLOR;
            case BlendingFactor::InverseSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case BlendingFactor::InverseSourceColor: return GL_ONE_MINUS_SRC_COLOR;
            case BlendingFactor::InverseDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
            case BlendingFactor::InverseDestinationColor: return GL_ONE_MINUS_DST_COLOR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLBlendingEquation(BlendingEquation blending_equation) {
        switch (blending_equation) {
            case BlendingEquation::Add: return GL_FUNC_ADD;
            case BlendingEquation::Subtract: return GL_FUNC_SUBTRACT;
            case BlendingEquation::ReverseSubract: return GL_FUNC_REVERSE_SUBTRACT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLCullingMode(CullingMode culling_mode) {
        switch (culling_mode) {
            case CullingMode::Back: return GL_BACK;
            case CullingMode::Front: return GL_FRONT;
            case CullingMode::FrontAndBack: return GL_FRONT_AND_BACK;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLCullingFrontFaceMode(CullingFrontFaceMode culling_front_face_mode) {
        switch (culling_front_face_mode) {
            case CullingFrontFaceMode::Clockwise: return GL_CW;
            case CullingFrontFaceMode::CounterClockwise: return GL_CCW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLPolygonMode(PolygonMode polygon_mode) {
        switch (polygon_mode) {
            case PolygonMode::Fill: return GL_FILL;
            case PolygonMode::Line: return GL_LINE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLIndexFormat(IndexFormat index_format) {
        switch (index_format) {
            case IndexFormat::UInt16: return GL_UNSIGNED_SHORT;
            case IndexFormat::UInt32: return GL_UNSIGNED_INT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLsizei OpenGLUtilities::GetGLIndexFormatSize(IndexFormat index_format) {
        switch (index_format) {
            case IndexFormat::UInt16: return sizeof(uint16);
            case IndexFormat::UInt32: return sizeof(uint32);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLMeshTopology(MeshTopology mesh_topology) {
        switch (mesh_topology) {
            case MeshTopology::Points: return GL_POINTS;
            case MeshTopology::Lines: return GL_LINES;
            case MeshTopology::LineStrip: return GL_LINE_STRIP;
            case MeshTopology::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLVertexAttributeType(VertexAttributeType vertex_attribute_type) {
        switch (vertex_attribute_type) {
            case VertexAttributeType::Float32: return GL_FLOAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLuint OpenGLUtilities::GetGLSizeForVertexAttribute(VertexAttributeType vertex_attribute_type, uint32 dimension) {
        switch (vertex_attribute_type) {
            case VertexAttributeType::Float32: return sizeof(float32) * dimension;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    void OpenGLUtilities::SetUnpackAlignmentForTextureFormat(TextureFormat format) {
        GLint alignment = 4;
        switch (format) {
            case Hyperion::Rendering::TextureFormat::RGB24: alignment = 4; break;
            case Hyperion::Rendering::TextureFormat::RGBA32: alignment = 4; break;
            case Hyperion::Rendering::TextureFormat::R8: alignment = 1; break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, alignment);
    }

    GLenum OpenGLUtilities::GetGLTextureFormat(TextureFormat format) {
        switch (format) {
            case TextureFormat::RGB24: return GL_RGB;
            case TextureFormat::RGBA32: return GL_RGBA;
            case TextureFormat::R8: return GL_RED;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLTextureFormatType(TextureFormat format_type) {
        switch (format_type) {
            case TextureFormat::RGB24: return GL_UNSIGNED_BYTE;
            case TextureFormat::RGBA32: return GL_UNSIGNED_BYTE;
            case TextureFormat::R8: return GL_UNSIGNED_BYTE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLUtilities::GetGLTextureInternalFormat(TextureFormat internal_format) {
        switch (internal_format) {
            case TextureFormat::RGB24: return GL_RGB8;
            case TextureFormat::RGBA32: return GL_RGBA8;
            case TextureFormat::R8: return GL_R8;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLint OpenGLUtilities::GetGLTextureWrapMode(TextureWrapMode wrap_mode) {
        switch (wrap_mode) {
            case TextureWrapMode::Clamp: return GL_CLAMP_TO_EDGE;
            case TextureWrapMode::Border: return GL_CLAMP_TO_BORDER;
            case TextureWrapMode::Repeat: return GL_REPEAT;
            case TextureWrapMode::MirroredRepeat: return GL_MIRRORED_REPEAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLint OpenGLUtilities::GetGLTextureMinFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Point: return GL_NEAREST_MIPMAP_NEAREST;
            case TextureFilter::Bilinear: return GL_LINEAR_MIPMAP_NEAREST;
            case TextureFilter::Trilinear: return GL_LINEAR_MIPMAP_LINEAR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST_MIPMAP_NEAREST;
        }
    }

    GLint OpenGLUtilities::GetGLTextureMaxFilter(TextureFilter filter) {
        switch (filter) {
            case TextureFilter::Point: return GL_NEAREST;
            case TextureFilter::Bilinear: return GL_LINEAR;
            case TextureFilter::Trilinear: return GL_LINEAR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return GL_NEAREST;
        }
    }

    GLfloat OpenGLUtilities::GetGLTextureAnisotropicFilter(TextureAnisotropicFilter anisotropic_filter) {
        switch (anisotropic_filter) {
            case TextureAnisotropicFilter::None: return 1.0f;
            case TextureAnisotropicFilter::Times2: return 2.0f;
            case TextureAnisotropicFilter::Times4: return 4.0f;
            case TextureAnisotropicFilter::Times8: return 8.0f;
            case TextureAnisotropicFilter::Times16: return 16.0f;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 1.0f;
        }
    }

}