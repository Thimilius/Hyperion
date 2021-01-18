#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Clear(ClearFlags clear_flags, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GetGLClearFlags(clear_flags));
    }

    void OpenGLRenderDriver::Viewport(const Rendering::Viewport &viewport) {
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
    }

    void OpenGLRenderDriver::SetRasterizerState(const RasterizerState &rasterizer_state) {
        // NOTE: Currently we are setting the whole rasterizer state at once,
        // regardless of wether or not we actually have new values we want to change.
        // So this is very expensive.

        // Depth
        {
            if (rasterizer_state.depth_test_enabled) {
                glEnable(GL_DEPTH_TEST);
            } else {
                glDisable(GL_DEPTH_TEST);
            }
            if (rasterizer_state.depth_mask_enabled) {
                glDepthMask(GL_TRUE);
            } else {
                glDepthMask(GL_FALSE);
            }
            glDepthFunc(GetGLDepthEquation(rasterizer_state.depth_equation));
        }
        
        // Stencil
        {
            if (rasterizer_state.stencil_test_enabled) {
                glEnable(GL_STENCIL_TEST);
            } else {
                glDisable(GL_STENCIL_TEST);
            }
        }

        // Blending
        {
            if (rasterizer_state.blending_enabled) {
                glEnable(GL_BLEND);
            } else {
                glDisable(GL_BLEND);
            }
            glBlendFunc(GetGLBlendingFactor(rasterizer_state.blending_function.source_factor), GetGLBlendingFactor(rasterizer_state.blending_function.destination_factor));
            glBlendEquation(GetGLBlendingEquation(rasterizer_state.blending_equation));
        }

        // Culling
        {
            if (rasterizer_state.culling_enabled) {
                glEnable(GL_CULL_FACE);
            } else {
                glDisable(GL_CULL_FACE);
            }
            glCullFace(GetGLCullingMode(rasterizer_state.culling_mode));
            glFrontFace(GetGLCullingFrontFaceMode(rasterizer_state.culling_front_face_mode));
        }

        // Misc
        {
            glPolygonMode(GL_FRONT_AND_BACK, GetGLPolygonMode(rasterizer_state.polygon_mode));
        }
    }

    void OpenGLRenderDriver::CreateShader(ResourceId id, const ShaderDescriptor &descriptor) {
        HYP_ASSERT(s_shaders.find(id) == s_shaders.end());

        OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(descriptor.vertex, descriptor.fragment);
        if (compilation_result.succes) {
            OpenGLShader &shader = s_shaders[id];
            shader.program = compilation_result.program;
        } else {
            // TODO: How do we handle shader errors here?
        }
    }

    void OpenGLRenderDriver::FreeShader(ResourceId id) {
        HYP_ASSERT(s_shaders.find(id) != s_shaders.end());

        OpenGLShader &shader = s_shaders[id];
        glDeleteProgram(shader.program);
        
        s_shaders.erase(id);
    }

    void OpenGLRenderDriver::CreateMesh(ResourceId id, const MeshDescriptor &descriptor) {
        HYP_ASSERT(s_meshes.find(id) == s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        mesh.index_format = descriptor.index_format;
        mesh.sub_meshes = descriptor.sub_meshes;

        glCreateBuffers(2, &mesh.vertex_buffer);
        glNamedBufferData(mesh.vertex_buffer, descriptor.vertex_data.size(), descriptor.vertex_data.data(), GL_STATIC_DRAW);
        glNamedBufferData(mesh.index_buffer, descriptor.index_data.size(), descriptor.index_data.data(), GL_STATIC_DRAW);

        glCreateVertexArrays(1, &mesh.vertex_array);

        glVertexArrayElementBuffer(mesh.vertex_array, mesh.index_buffer);

        // FIXME: These are hardcoded attribute bindings
        glEnableVertexArrayAttrib(mesh.vertex_array, 0);
        glVertexArrayAttribFormat(mesh.vertex_array, 0, 3, GL_FLOAT, false, 0);
        glVertexArrayVertexBuffer(mesh.vertex_array, 0, mesh.vertex_buffer, 0, 32);
        glVertexArrayAttribBinding(mesh.vertex_array, 0, 0);
    }

    void OpenGLRenderDriver::FreeMesh(ResourceId id) {
        HYP_ASSERT(s_meshes.find(id) != s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        glDeleteBuffers(2, &mesh.vertex_buffer);
        glDeleteVertexArrays(1, &mesh.vertex_array);

        s_meshes.erase(id);
    }

    void OpenGLRenderDriver::DrawIndexed(ResourceId shader_id, ResourceId mesh_id) {
        HYP_ASSERT(s_shaders.find(shader_id) != s_shaders.end());
        HYP_ASSERT(s_meshes.find(mesh_id) != s_meshes.end());

        OpenGLShader &shader = s_shaders[shader_id];
        OpenGLMesh &mesh = s_meshes[mesh_id];

        glUseProgram(shader.program);
        glBindVertexArray(mesh.vertex_array);

        GLenum index_format = GetGLIndexFormat(mesh.index_format);
        GLsizei index_format_size = GetGLIndexFormatSize(mesh.index_format);
        for (SubMesh &sub_mesh : mesh.sub_meshes) {
            const void *offset = reinterpret_cast<const void *>(static_cast<uint64>(sub_mesh.index_offset) * index_format_size);
            glDrawElementsBaseVertex(GetGLMeshTopology(sub_mesh.topology), sub_mesh.index_count, index_format, offset, sub_mesh.vertex_offset);
        }
    }

    GLbitfield OpenGLRenderDriver::GetGLClearFlags(ClearFlags clear_flags) {
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

    GLenum OpenGLRenderDriver::GetGLDepthEquation(DepthEquation depth_equation) {
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

    GLenum OpenGLRenderDriver::GetGLBlendingFactor(BlendingFactor blending_factor) {
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

    GLenum OpenGLRenderDriver::GetGLBlendingEquation(BlendingEquation blending_equation) {
        switch (blending_equation) {
            case BlendingEquation::Add: return GL_FUNC_ADD;
            case BlendingEquation::Subtract: return GL_FUNC_SUBTRACT;
            case BlendingEquation::ReverseSubract: return GL_FUNC_REVERSE_SUBTRACT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLRenderDriver::GetGLCullingMode(CullingMode culling_mode) {
        switch (culling_mode) {
            case CullingMode::Back: return GL_BACK;
            case CullingMode::Front: return GL_FRONT;
            case CullingMode::FrontAndBack: return GL_FRONT_AND_BACK;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLRenderDriver::GetGLCullingFrontFaceMode(CullingFrontFaceMode culling_front_face_mode) {
        switch (culling_front_face_mode) {
            case CullingFrontFaceMode::Clockwise: return GL_CW;
            case CullingFrontFaceMode::CounterClockwise: return GL_CCW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLRenderDriver::GetGLPolygonMode(PolygonMode polygon_mode) {
        switch (polygon_mode) {
            case PolygonMode::Fill: return GL_FILL;
            case PolygonMode::Line: return GL_LINE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLRenderDriver::GetGLIndexFormat(IndexFormat index_format) {
        switch (index_format) {
            case IndexFormat::UInt16: return GL_UNSIGNED_SHORT;
            case IndexFormat::UInt32: return GL_UNSIGNED_INT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLsizei OpenGLRenderDriver::GetGLIndexFormatSize(IndexFormat index_format) {
        switch (index_format) {
            case IndexFormat::UInt16: return sizeof(uint16);
            case IndexFormat::UInt32: return sizeof(uint32);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    GLenum OpenGLRenderDriver::GetGLMeshTopology(MeshTopology mesh_topology) {
        switch (mesh_topology) {
            case MeshTopology::Points: return GL_POINTS;
            case MeshTopology::Lines: return GL_LINES;
            case MeshTopology::LineStrip: return GL_LINE_STRIP;
            case MeshTopology::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}