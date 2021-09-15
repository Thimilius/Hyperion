//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/threading/scope_lock.hpp"
#include "hyperion/graphics/graphics_context.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"
#include "hyperion/graphics/driver/opengl/opengl_shader_compiler.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Graphics;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct OpenGLShader {
        AssetId id;

        GLuint program;
    };
    Array<OpenGLShader> g_opengl_shaders;

    struct OpenGLMaterial {
        AssetId id;
        MaterialPropertyCollection properties;

        OpenGLShader *shader;
    };
    Array<OpenGLMaterial> g_opengl_materials;

    struct OpenGLMesh {
        AssetId id;
        Array<SubMesh> sub_meshes;

        GLsizei index_count;
        GLuint vertex_buffer;
        GLuint index_buffer;
        GLuint vertex_array;
    };
    Array<OpenGLMesh> g_opengl_meshes;

    GLuint g_bound_vertex_array;


    struct GroupedMesh {
        OpenGLMesh *mesh;

        Array<RenderFrameMeshObject> objects;
    };
    struct GroupedMaterial {
        OpenGLMaterial *material;

        Array<GroupedMesh> meshes;
    };
    struct GroupedShader {
        OpenGLShader *shader;

        Array<GroupedMaterial> materials;
    };

    //--------------------------------------------------------------
    Array<GroupedShader> GroupObjects(RenderFrame *render_frame) {
        Array<GroupedShader> grouped_shaders;

        for (const RenderFrameMeshObject &render_frame_mesh_object : render_frame->GetMeshObjects()) {
            AssetId material_id = render_frame_mesh_object.material->GetAssetInfo().id;
            AssetId shader_id = render_frame_mesh_object.material->GetShader()->GetAssetInfo().id;
            AssetId mesh_id = render_frame_mesh_object.mesh->GetAssetInfo().id;

            auto shaders_it = std::find_if(grouped_shaders.begin(), grouped_shaders.end(), [shader_id](const GroupedShader &grouped_shader) {
                return grouped_shader.shader->id == shader_id;
            });
            GroupedShader *grouped_shader = nullptr;
            if (shaders_it == grouped_shaders.end()) {
                auto opengl_shaders_it = std::find_if(g_opengl_shaders.begin(), g_opengl_shaders.end(), [shader_id](const OpenGLShader &opengl_shader) {
                    return opengl_shader.id == shader_id;
                });
                GroupedShader new_grouped_shader;
                new_grouped_shader.shader = &*opengl_shaders_it;
                grouped_shaders.Add(new_grouped_shader);
                grouped_shader = &grouped_shaders.GetLast();
            } else {
                grouped_shader = &*shaders_it;
            }

            Array<GroupedMaterial> &materials = grouped_shader->materials;
            auto materials_it = std::find_if(materials.begin(), materials.end(), [material_id](const GroupedMaterial &grouped_material) {
                return grouped_material.material->id == material_id;
            });
            GroupedMaterial *grouped_material = nullptr;
            if (materials_it == materials.end()) {
                auto opengl_material_it = std::find_if(g_opengl_materials.begin(), g_opengl_materials.end(), [material_id](const OpenGLMaterial &opengl_material) {
                    return opengl_material.id == material_id;
                });
                GroupedMaterial new_grouped_material;
                new_grouped_material.material = &*opengl_material_it;
                materials.Add(new_grouped_material);
                grouped_material = &materials.GetLast();
            } else {
                grouped_material = &*materials_it;
            }

            Array<GroupedMesh> &meshes = grouped_material->meshes;
            auto meshes_it = std::find_if(meshes.begin(), meshes.end(), [mesh_id](const GroupedMesh &grouped_mesh) {
                return grouped_mesh.mesh->id == mesh_id;
            });
            GroupedMesh *grouped_mesh = nullptr;
            if (meshes_it == meshes.end()) {
                auto opengl_mesh_it = std::find_if(g_opengl_meshes.begin(), g_opengl_meshes.end(), [mesh_id](const OpenGLMesh &opengl_mesh) {
                    return opengl_mesh.id == mesh_id;
                });
                GroupedMesh new_grouped_material;
                new_grouped_material.mesh = &*opengl_mesh_it;
                meshes.Add(new_grouped_material);
                grouped_mesh = &meshes.GetLast();
            } else {
                grouped_mesh = &*meshes_it;
            }

            grouped_mesh->objects.Add(render_frame_mesh_object);
        }

        return grouped_shaders;
    }

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
    GLenum GetGLMeshTopology(MeshTopology mesh_topology) {
        switch (mesh_topology) {
            case MeshTopology::Points: return GL_POINTS;
            case MeshTopology::Lines: return GL_LINES;
            case MeshTopology::LineStrip: return GL_LINE_STRIP;
            case MeshTopology::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Initialize(GraphicsContext *graphics_context) {
        graphics_context->CreateDeviceAndSwapChain(&m_device, &m_device_context, &m_swap_chain);

        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    Array<GroupedShader> g_grouped_shaders;

    //--------------------------------------------------------------
    void ForwardRenderPipeline::Render(RenderFrame *render_frame) {
        LoadAssets(render_frame);

        g_grouped_shaders = GroupObjects(render_frame);

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

        {
            glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
            Color background_color = render_frame_camera.background_color;
            glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }
        
        {
            for (const OpenGLShader &shader : g_opengl_shaders) {
                glProgramUniformMatrix4fv(shader.program, glGetUniformLocation(shader.program, "u_view"), 1, GL_FALSE, render_frame_camera.view_matrix.elements);
                glProgramUniformMatrix4fv(shader.program, glGetUniformLocation(shader.program, "u_projection"), 1, GL_FALSE, render_frame_camera.projection_matrix.elements);
            }
        }

        for (const GroupedShader &grouped_shader : g_grouped_shaders) {
            const OpenGLShader &opengl_shader = *grouped_shader.shader;
            glUseProgram(opengl_shader.program);

            for (const GroupedMaterial &grouped_material : grouped_shader.materials) {
                const OpenGLMaterial &opengl_material = *grouped_material.material;

                for (const MaterialProperty &property : opengl_material.properties) {
                    if (property.type == MaterialPropertyType::Color) {
                        Color color = property.storage.color;
                        glProgramUniform4f(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_color"), color.r, color.g, color.b, color.a);
                    }
                }

                for (const GroupedMesh &grouped_mesh : grouped_material.meshes) {
                    const OpenGLMesh &opengl_mesh = *grouped_mesh.mesh;
                    glBindVertexArray(opengl_mesh.vertex_array);

                    GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
                    for (const RenderFrameMeshObject &render_frame_mesh_object : grouped_mesh.objects) {
                        glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, render_frame_mesh_object.local_to_world.elements);

                        // TODO: We should also group by sub meshes.
                        SubMesh sub_mesh = opengl_mesh.sub_meshes[render_frame_mesh_object.sub_mesh_index];
                        void *index_offset = reinterpret_cast<void *>(static_cast<uint32>(sub_mesh.index_offset) * sizeof(uint32));
                        glDrawElementsBaseVertex(GetGLMeshTopology(sub_mesh.topology), sub_mesh.index_count, GL_UNSIGNED_INT, index_offset, sub_mesh.vertex_offset);
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::LoadAssets(RenderFrame *render_frame) {
        for (Asset *asset : render_frame->GetAssetsToLoad()) {
            Threading::ScopeLock lock(asset->GetLocker());

            switch (asset->GetAssetType()) {
                case AssetType::Material: LoadMaterial(static_cast<Material *>(asset)); break;
                case AssetType::Mesh: LoadMesh(static_cast<Mesh *>(asset)); break;
                case AssetType::Shader: LoadShader(static_cast<Shader *>(asset)); break;
            }
        }
    }

    //--------------------------------------------------------------
    void ForwardRenderPipeline::LoadMaterial(Material *material) {
        AssetId material_id = material->GetAssetInfo().id;
        auto material_it = std::find_if(g_opengl_materials.begin(), g_opengl_materials.end(), [material_id](const OpenGLMaterial &opengl_material) {
            return opengl_material.id == material_id;
        });
        bool8 update = material_it != g_opengl_materials.end();

        if (!update) {
            g_opengl_materials.Resize(g_opengl_materials.GetLength() + 1);
        }
        OpenGLMaterial &opengl_material = update ? *material_it : g_opengl_materials.GetLast();

        if (!update) {
            opengl_material.id = material_id;

            AssetId shader_id = material->GetShader()->GetAssetInfo().id;
            auto shader_it = std::find_if(g_opengl_shaders.begin(), g_opengl_shaders.end(), [shader_id](const OpenGLShader &opengl_shader) {
                return opengl_shader.id == shader_id;
            });
            OpenGLShader &opengl_shader = *shader_it;
            opengl_material.shader = &opengl_shader;
        }

        opengl_material.properties = material->GetProperties();
    }

    //--------------------------------------------------------------
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
        opengl_mesh.sub_meshes = mesh->GetSubMeshes();

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

    //--------------------------------------------------------------
    void ForwardRenderPipeline::LoadShader(Shader *shader) {
        g_opengl_shaders.Resize(g_opengl_shaders.GetLength() + 1);
        OpenGLShader &opengl_shader = g_opengl_shaders.GetLast();

        const ShaderData &data = shader->GetData();

        opengl_shader.id = shader->GetAssetInfo().id;
        opengl_shader.program = OpenGLShaderCompiler::Compile(data.vertex_source.c_str(), data.fragment_source.c_str()).program;
    }

}