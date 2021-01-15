#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Clear(ClearFlags clear_mask, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GetGLClearFlags(clear_mask));
    }

    void OpenGLRenderDriver::CreateShader(ResourceId id, const String &vertex, const String &fragment) {
        HYP_ASSERT(s_shaders.find(id) == s_shaders.end());

        OpenGLShaderCompilationResult compilation_result = OpenGLShaderCompiler::Compile(vertex, fragment);
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

    void OpenGLRenderDriver::CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) {
        HYP_ASSERT(s_meshes.find(id) == s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];

        glCreateBuffers(2, &mesh.vertex_buffer);
        glNamedBufferData(mesh.vertex_buffer, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
        glNamedBufferData(mesh.index_buffer, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        mesh.indices_count = static_cast<s32>(indices.size());

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

        GLsizei width = static_cast<GLsizei>(Display::GetWidth());
        GLsizei height = static_cast<GLsizei>(Display::GetHeight());
        glViewport(0, 0, width, height);

        glFrontFace(GL_CW);

        glUseProgram(shader.program);
        glBindVertexArray(mesh.vertex_array);

        glDrawElements(GL_TRIANGLES, mesh.indices_count, GL_UNSIGNED_INT, nullptr);
    }

    u32 OpenGLRenderDriver::GetGLClearFlags(ClearFlags clear_mask) {
        u32 result = 0;

        if ((clear_mask & ClearFlags::Color) == ClearFlags::Color) {
            result |= GL_COLOR_BUFFER_BIT;
        }
        if ((clear_mask & ClearFlags::Depth) == ClearFlags::Depth) {
            result |= GL_DEPTH_BUFFER_BIT;
        }
        if ((clear_mask & ClearFlags::Stencil) == ClearFlags::Stencil) {
            result |= GL_STENCIL_BUFFER_BIT;
        }

        return result;
    }

}