#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"


namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Clear(ClearFlags clear_mask, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
        glClear(GetGLClearFlags(clear_mask));
    }

    void OpenGLRenderDriver::CreateMesh(ResourceId id, const Vector<VertexMesh> &vertices, const Vector<u32> &indices) {
        HYP_ASSERT(s_meshes.find(id) == s_meshes.end());

        OpenGLMesh mesh;
        mesh.id = id;

        glCreateBuffers(2, &mesh.vertex_buffer);
        glNamedBufferData(mesh.vertex_buffer, vertices.size() * sizeof(vertices[0]), vertices.data(), GL_STATIC_DRAW);
        glNamedBufferData(mesh.index_buffer, indices.size() * sizeof(indices[0]), indices.data(), GL_STATIC_DRAW);

        glCreateVertexArrays(1, &mesh.vertex_array);

        // TODO: Setup buffer layout

        s_meshes[id] = mesh;
    }

    void OpenGLRenderDriver::FreeMesh(ResourceId id) {
        HYP_ASSERT(s_meshes.find(id) != s_meshes.end());

        OpenGLMesh &mesh = s_meshes[id];
        glDeleteBuffers(2, &mesh.vertex_buffer);
        glDeleteVertexArrays(1, &mesh.vertex_array);

        s_meshes.erase(id);
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