//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"
#include "hyperion/graphics/driver/opengl/opengl_shader_compiler.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    GLuint g_shader_program;
    GLuint g_vertex_buffer;
    GLuint g_vertex_array;

    struct OpenGLMesh {
        AssetId id;

        GLsizei index_count;

        GLuint vertex_buffer;
        GLuint index_buffer;

        GLuint vertex_array;
    };
    Array<OpenGLMesh> g_opengl_meshes;

    GLuint g_bound_vertex_array;

    //--------------------------------------------------------------
    GLuint GetGLAttributeIndexForVertextAttributeSize(VertexAttributeKind kind) {
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
    GLenum GetGLVertexAttributeType(VertexAttributeType vertex_attribute_type) {
        switch (vertex_attribute_type) {
            case VertexAttributeType::Float32: return GL_FLOAT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    GLuint GetGLVertexAttributeSizeForVertexAttribute(VertexAttributeType vertex_attribute_type, uint32 dimension) {
        switch (vertex_attribute_type) {
            case VertexAttributeType::Float32: return sizeof(float32) * dimension;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }


    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize(GraphicsContext *graphics_context) {
        graphics_context->CreateDeviceAndSwapChain(&m_device, &m_device_context, &m_swap_chain);

        const char *vertex_source = R"(
            #version 450
            
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec3 a_normal;
            layout(location = 4) in vec2 a_texture0;

            out V2F {
	            vec4 color;
            } o_v2f;

            uniform mat4 u_model;
            uniform mat4 u_view;
            uniform mat4 u_projection;

            void main() {
                o_v2f.color = vec4(a_texture0, 0.0f, 1.0f);
    
                gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0);
            }
        )";
        const char *fragment_source = R"(
            #version 450

            layout(location = 0) out vec4 o_color;

            in V2F {
	            vec4 color;
            } i_v2f;

            void main() {
                o_color = i_v2f.color;
            }	
        )";
        g_shader_program = OpenGLShaderCompiler::Compile(vertex_source, fragment_source).program;

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        HandleAssets(render_frame);

        for (const RenderFrameCamera &render_frame_camera : render_frame->GetCameras()) {
            RenderCamera(render_frame_camera, render_frame);
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Shutdown() {

    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::RenderCamera(const RenderFrameCamera &render_frame_camera, RenderFrame *render_frame) {
        const CameraViewport &viewport = render_frame_camera.viewport;

        glProgramUniformMatrix4fv(g_shader_program, glGetUniformLocation(g_shader_program, "u_view"), 1, GL_FALSE, render_frame_camera.view_matrix.elements);
        glProgramUniformMatrix4fv(g_shader_program, glGetUniformLocation(g_shader_program, "u_projection"), 1, GL_FALSE, render_frame_camera.projection_matrix.elements);

        // TEMP: This is just so we see something on the screen.
        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
        Color background_color = render_frame_camera.background_color;
        glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        OpenGLMesh opengl_mesh = g_opengl_meshes[0];

        glUseProgram(g_shader_program);
        for (const RenderFrameMeshObject &render_frame_mesh_object : render_frame->GetMeshObjects()) {
            AssetId mesh_id = render_frame_mesh_object.mesh->GetAssetInfo().id;
            auto it = std::find_if(g_opengl_meshes.begin(), g_opengl_meshes.end(), [mesh_id](const OpenGLMesh &opengl_mesh) {
                return opengl_mesh.id == mesh_id;
            });
            if (it != g_opengl_meshes.end()) {
                if (it->vertex_array != g_bound_vertex_array) {
                    glBindVertexArray(it->vertex_array);
                    g_bound_vertex_array = it->vertex_array;
                }
            }

            glProgramUniformMatrix4fv(g_shader_program, glGetUniformLocation(g_shader_program, "u_model"), 1, GL_FALSE, render_frame_mesh_object.local_to_world.elements);
            glDrawElements(GL_TRIANGLES, opengl_mesh.index_count, GL_UNSIGNED_INT, nullptr);
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::HandleAssets(RenderFrame *render_frame) {
        for (Asset *asset : render_frame->GetAssetsToLoad()) {
            Threading::ScopeLock lock(asset->GetLocker());
            if (asset->GetAssetType() == AssetType::Mesh) {
                LoadMesh(static_cast<Mesh *>(asset));
            }
        }
    }

    void ForwardRenderPipeline::LoadMesh(Mesh *mesh) {
        g_opengl_meshes.Resize(g_opengl_meshes.GetLength() + 1);
        OpenGLMesh &opengl_mesh = g_opengl_meshes.GetLast();

        const MeshData &data = mesh->GetData();
        const MeshVertexFormat &vertex_format = mesh->GetVertexFormat();

        bool8 has_normals = data.normals.GetLength() > 0;
        bool8 has_colors = data.colors.GetLength() > 0;
        bool8 has_texture0 = data.texture0.GetLength() > 0;

        uint32 vertex_count = static_cast<uint32>(data.positions.GetLength());
        Array<uint8> vertices(vertex_count * vertex_format.stride);
        for (uint32 i = 0; i < vertex_count; i++) {
            uint32 index = i * vertex_format.stride;

            Vector3 &position = reinterpret_cast<Vector3 &>(vertices[index]);
            position = data.positions[i];
            index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_POSITION;

            if (has_normals) {
                Vector3 &normal = reinterpret_cast<Vector3 &>(vertices[index]);
                normal = data.normals[i];
                index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_NORMAL;
            }
            if (has_colors) {
                Vector4 &color = reinterpret_cast<Vector4 &>(vertices[index]);
                color = data.colors[i];
                index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_COLOR;
            }
            if (has_texture0) {
                Vector2 &texture0 = reinterpret_cast<Vector2 &>(vertices[index]);
                texture0 = data.texture0[i];
                index += MeshVertexFormat::VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
            }
        }

        opengl_mesh.id = mesh->GetAssetInfo().id;

        glCreateBuffers(1, &opengl_mesh.vertex_buffer);
        glNamedBufferData(opengl_mesh.vertex_buffer, vertices.GetLength(), vertices.GetData(), GL_STATIC_DRAW);

        glCreateBuffers(1, &opengl_mesh.index_buffer);
        glNamedBufferData(opengl_mesh.index_buffer, data.indices.GetLength() * sizeof(data.indices[0]), data.indices.GetData(), GL_STATIC_DRAW);
        opengl_mesh.index_count = static_cast<GLsizei>(data.indices.GetLength());

        GLsizei stride = vertex_format.stride;
        GLuint relative_offset = 0;
        glCreateVertexArrays(1, &opengl_mesh.vertex_array);
        glVertexArrayVertexBuffer(opengl_mesh.vertex_array, 0, opengl_mesh.vertex_buffer, 0, stride);
        glVertexArrayElementBuffer(opengl_mesh.vertex_array, opengl_mesh.index_buffer);

        for (VertexAttribute vertex_attribute : vertex_format.attributes) {
            GLuint attribute_index = GetGLAttributeIndexForVertextAttributeSize(vertex_attribute.kind);
            GLuint binding_index = 0;
            GLint size = vertex_attribute.dimension;
            GLenum type = GetGLVertexAttributeType(vertex_attribute.type);

            glEnableVertexArrayAttrib(opengl_mesh.vertex_array, attribute_index);
            glVertexArrayAttribFormat(opengl_mesh.vertex_array, attribute_index, size, type, false, relative_offset);
            glVertexArrayAttribBinding(opengl_mesh.vertex_array, attribute_index, binding_index);

            relative_offset += GetGLVertexAttributeSizeForVertexAttribute(vertex_attribute.type, vertex_attribute.dimension);
        }
    }

}