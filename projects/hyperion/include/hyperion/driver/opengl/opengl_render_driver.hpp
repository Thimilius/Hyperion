#pragma once

#include <glad/glad.h>

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    class OpenGLRenderDriver : public IRenderDriver {
    public:
        void Clear(ClearFlags clear_flags, Color color) override;
        void Viewport(const Rendering::Viewport &viewport) override;

        void CreateShader(ResourceId id, const ShaderDescriptor &descriptor) override;
        void FreeShader(ResourceId id) override;

        void CreateMesh(ResourceId id, const MeshDescriptor &descriptor) override;
        void FreeMesh(ResourceId id) override;

        void DrawIndexed(ResourceId shader_id, ResourceId mesh_id) override;
    private:
        static GLbitfield GetGLClearFlags(ClearFlags clear_flags);
        static GLenum GetGLIndexFormat(IndexFormat index_format);
        static GLsizei GetGLIndexFormatSize(IndexFormat index_format);
        static GLenum GetGLMeshTopology(MeshTopology mesh_topology);
    private:
        struct OpenGLShader {
            GLuint program;
        };
        inline static Map<ResourceId, OpenGLShader> s_shaders;

        struct OpenGLMesh {
            // Vertex and index buffer have to be next to each other to support efficient creation and destruction
            GLuint vertex_buffer;
            GLuint index_buffer;
            GLuint vertex_array;

            IndexFormat index_format;
            Vector<SubMesh> sub_meshes;
        };
        inline static Map<ResourceId, OpenGLMesh> s_meshes;
    };

}