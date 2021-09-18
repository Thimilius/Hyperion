#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/rendering/driver/opengl/opengl_render_driver.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/frame/render_frame.hpp"
#include "hyperion/rendering/driver/opengl/opengl_render_driver_shader_compiler.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct OpenGLShader {
        AssetId id;

        GLuint program;
    };
    Map<AssetId, OpenGLShader> g_opengl_shaders;

    struct OpenGLMaterial {
        AssetId id;
        MaterialPropertyCollection properties;

        OpenGLShader *shader;
    };
    Map<AssetId, OpenGLMaterial> g_opengl_materials;

    struct OpenGLMesh {
        AssetId id;
        Array<SubMesh> sub_meshes;

        GLsizei index_count;
        GLuint vertex_buffer;
        GLuint index_buffer;
        GLuint vertex_array;
    };
    Map<AssetId, OpenGLMesh> g_opengl_meshes;

    GLuint g_bound_vertex_array;

    struct GroupedMesh {
        OpenGLMesh *mesh;

        Array<RenderFrameContextObjectMesh> objects;
    };
    struct GroupedMaterial {
        OpenGLMaterial *material;

        Map<AssetId, GroupedMesh> meshes;
    };
    struct GroupedShader {
        OpenGLShader *shader;

        Map<AssetId, GroupedMaterial> materials;
    };
    Array<GroupedShader> g_grouped_shaders;

    //--------------------------------------------------------------
    GLenum GetGLTopology(MeshTopology mesh_topology) {
        switch (mesh_topology) {
            case MeshTopology::Points: return GL_POINTS;
            case MeshTopology::Lines: return GL_LINES;
            case MeshTopology::LineStrip: return GL_LINE_STRIP;
            case MeshTopology::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
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
    void LoadShader(RenderFrameContextAssetShader &shader) {
        HYP_PROFILE_SCOPE("LoadShader");

        // FIXME: Handle shader compilation errors.
        OpenGLShader opengl_shader;
        opengl_shader.id = shader.id;
        opengl_shader.program = OpenGLRenderDriverShaderCompiler::Compile(shader.data.vertex_source.c_str(), shader.data.fragment_source.c_str()).program;

        g_opengl_shaders.Insert(shader.id, opengl_shader);
    }

    //--------------------------------------------------------------
    void LoadMaterial(RenderFrameContextAssetMaterial &material) {
        HYP_PROFILE_SCOPE("LoadMaterial");

        auto material_it = g_opengl_materials.Find(material.id);
        if (material_it == g_opengl_materials.end()) {
            OpenGLMaterial opengl_material;
            opengl_material.id = material.id;
            opengl_material.shader = &g_opengl_shaders.Get(material.shader_id);;
            opengl_material.properties = std::move(material.properties);

            g_opengl_materials.Insert(material.id, opengl_material);
        } else {
            OpenGLMaterial &opengl_material = material_it->second;
            opengl_material.properties = std::move(material.properties);
        }
    }

    //--------------------------------------------------------------
    void LoadMesh(RenderFrameContextAssetMesh &mesh) {
        HYP_PROFILE_SCOPE("LoadMesh");

        const MeshData &data = mesh.data;
        const MeshVertexFormat &vertex_format = mesh.vertex_format;

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

        OpenGLMesh opengl_mesh;
        opengl_mesh.id = mesh.id;
        opengl_mesh.sub_meshes = std::move(mesh.sub_meshes);

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

        g_opengl_meshes.Insert(mesh.id, opengl_mesh);
    }

    //--------------------------------------------------------------
    void LoadAssets(RenderFrameContext &render_frame_context) {
        HYP_PROFILE_SCOPE("LoadAssets");

        // The order in which we load the assets is important.
        // For example a material might reference a shader which should always be loaded first.

        for (RenderFrameContextAssetShader &shader : render_frame_context.GetShadersToLoad()) {
            LoadShader(shader);
        }
        for (RenderFrameContextAssetMaterial &material : render_frame_context.GetMaterialsToLoad()) {
            LoadMaterial(material);
        }
        for (RenderFrameContextAssetMesh &mesh : render_frame_context.GetMeshesToLoad()) {
            LoadMesh(mesh);
        }
    }

    //--------------------------------------------------------------
    Array<GroupedShader> GroupObjects(const Array<RenderFrameContextObjectMesh> &mesh_objects, LayerMask visibility_mask) {
        HYP_PROFILE_CATEGORY("GroupObjects", ProfileCategory::Rendering);

        Array<GroupedShader> grouped_shaders;

        for (const RenderFrameContextObjectMesh &render_frame_context_object_mesh : mesh_objects) {
            if ((render_frame_context_object_mesh.layer_mask & visibility_mask) != render_frame_context_object_mesh.layer_mask) {
                continue;
            }

            AssetId material_id = render_frame_context_object_mesh.material->GetAssetInfo().id;
            AssetId shader_id = render_frame_context_object_mesh.material->GetShader()->GetAssetInfo().id;
            AssetId mesh_id = render_frame_context_object_mesh.mesh->GetAssetInfo().id;

            auto shaders_it = std::find_if(grouped_shaders.begin(), grouped_shaders.end(), [shader_id](const GroupedShader &grouped_shader) {
                return grouped_shader.shader->id == shader_id;
                });
            GroupedShader *grouped_shader = nullptr;
            if (shaders_it == grouped_shaders.end()) {
                GroupedShader new_grouped_shader;
                new_grouped_shader.shader = &g_opengl_shaders.Get(shader_id);
                grouped_shaders.Add(new_grouped_shader);
                grouped_shader = &grouped_shaders.GetLast();
            } else {
                grouped_shader = &*shaders_it;
            }

            Map<AssetId, GroupedMaterial> &materials = grouped_shader->materials;
            auto materials_it = materials.Find(material_id);
            GroupedMaterial *grouped_material = nullptr;
            if (materials_it == materials.end()) {
                GroupedMaterial new_grouped_material;
                new_grouped_material.material = &g_opengl_materials.Get(material_id);
                materials.Insert(material_id, new_grouped_material);
                grouped_material = &materials.Get(material_id);
            } else {
                grouped_material = &materials.Get(material_id);
            }

            Map<AssetId, GroupedMesh> &meshes = grouped_material->meshes;
            auto meshes_it = meshes.Find(mesh_id);
            GroupedMesh *grouped_mesh = nullptr;
            if (meshes_it == meshes.end()) {
                GroupedMesh new_grouped_material;
                new_grouped_material.mesh = &g_opengl_meshes.Get(mesh_id);
                meshes.Insert(mesh_id, new_grouped_material);
                grouped_mesh = &meshes.Get(mesh_id);
            } else {
                grouped_mesh = &meshes.Get(mesh_id);
            }

            grouped_mesh->objects.Add(render_frame_context_object_mesh);
        }

        return grouped_shaders;
    }

    //--------------------------------------------------------------
    void RenderCamera(const RenderFrameContextCamera &render_frame_context_camera, RenderFrame *render_frame) {
        HYP_PROFILE_CATEGORY("RenderCamera", ProfileCategory::Rendering);

        const CameraViewport &viewport = render_frame_context_camera.viewport;

        {
            glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
            Color background_color = render_frame_context_camera.background_color;
            glClearColor(background_color.r, background_color.g, background_color.b, background_color.a);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        }

        for (const GroupedShader &grouped_shader : g_grouped_shaders) {
            HYP_PROFILE_SCOPE("RenderGroupedShader");

            const OpenGLShader &opengl_shader = *grouped_shader.shader;
            glUseProgram(opengl_shader.program);
            glProgramUniformMatrix4fv(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_view"), 1, GL_FALSE, render_frame_context_camera.view_matrix.elements);
            glProgramUniformMatrix4fv(opengl_shader.program, glGetUniformLocation(opengl_shader.program, "u_projection"), 1, GL_FALSE, render_frame_context_camera.projection_matrix.elements);

            for (const auto [material_id, grouped_material] : grouped_shader.materials) {
                HYP_PROFILE_SCOPE("RenderGroupedMaterial");

                const OpenGLMaterial &opengl_material = *grouped_material.material;

                GLint color_location = glGetUniformLocation(opengl_shader.program, "u_color");
                for (const MaterialProperty &property : opengl_material.properties) {
                    if (property.type == MaterialPropertyType::Color) {
                        Color color = property.storage.color;
                        glProgramUniform4f(opengl_shader.program, color_location, color.r, color.g, color.b, color.a);
                    }
                }

                for (const auto [mesh_id, grouped_mesh] : grouped_material.meshes) {
                    HYP_PROFILE_SCOPE("RenderGroupedMesh");

                    const OpenGLMesh &opengl_mesh = *grouped_mesh.mesh;
                    glBindVertexArray(opengl_mesh.vertex_array);

                    GLint model_location = glGetUniformLocation(opengl_shader.program, "u_model");
                    for (const RenderFrameContextObjectMesh &render_frame_context_mesh_object : grouped_mesh.objects) {
                        HYP_PROFILE_SCOPE("RenderFrameMeshObject");

                        glProgramUniformMatrix4fv(opengl_shader.program, model_location, 1, GL_FALSE, render_frame_context_mesh_object.local_to_world.elements);

                        // NOTE: We may also want to group by sub meshes.
                        SubMesh sub_mesh = opengl_mesh.sub_meshes[render_frame_context_mesh_object.sub_mesh_index];
                        void *index_offset = reinterpret_cast<void *>(static_cast<uint32>(sub_mesh.index_offset) * sizeof(uint32));
                        GLenum topology = GetGLTopology(sub_mesh.topology);
                        glDrawElementsBaseVertex(topology, sub_mesh.index_count, GL_UNSIGNED_INT, index_offset, sub_mesh.vertex_offset);
                    }
                }
            }
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Initialize() {
        glEnable(GL_DEPTH_TEST);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Render(RenderFrame *render_frame) {
        HYP_PROFILE_CATEGORY("Render", ProfileCategory::Rendering);

        LoadAssets(render_frame->GetContext());

        for (const RenderFrameContextCamera &render_frame_context_camera : render_frame->GetContext().GetCameras()) {
            g_grouped_shaders = GroupObjects(render_frame->GetContext().GetMeshObjects(), render_frame_context_camera.visibility_mask);
            RenderCamera(render_frame_context_camera, render_frame);
        }
    }

    //--------------------------------------------------------------
    void OpenGLRenderDriver::Shutdown() {

    }

}