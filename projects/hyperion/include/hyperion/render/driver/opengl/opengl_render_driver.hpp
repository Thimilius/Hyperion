#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/render/driver/render_driver.hpp"
#include "hyperion/render/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct OpenGLAsset {
        AssetId id;
    };
    
    struct OpenGLShader : public OpenGLAsset {
        Array<GLint> locations;

        GLuint program;
    };

    struct OpenGLTexture : public OpenGLAsset {
        GLuint texture;
    };

    struct OpenGLMaterial : public OpenGLAsset {
        MaterialPropertyCollection properties;

        OpenGLShader *shader;
    };

    struct OpenGLMesh : public OpenGLAsset {
        Array<SubMesh> sub_meshes;

        GLsizei index_count;
        GLuint vertex_buffer;
        GLuint index_buffer;
        GLuint vertex_array;
    };
    
    struct GroupedObject {
        Matrix4x4 local_to_world;
        uint32 sub_mesh_index;

        uint32 light_count;
        uint32 light_indices[4];
    };

    struct GroupedMesh {
        OpenGLMesh *mesh;

        Array<GroupedObject> objects;
    };

    struct GroupedMaterial {
        OpenGLMaterial *material;

        Map<AssetId, GroupedMesh> meshes;
    };

    struct GroupedShader {
        OpenGLShader *shader;

        Map<AssetId, GroupedMaterial> materials;
    };

    struct OpenGLUniformBufferCamera {
        Matrix4x4 camera_view_matrix;
        Matrix4x4 camera_projection_matrix;
    };

    struct OpenGLLight {
        Color color;
        Vector3 direction;
        float32 intensity;
        Vector3 position;
        float32 range;
        float32 spot_inner_radius;
        float32 spot_outer_radius;

        float32 padding[2];
    };

    struct OpenGLUniformBufferLighting {
        Color ambient_color;

        OpenGLLight main_light;

        OpenGLLight point_lights[128];
    };

    struct OpenGLState {
        uint64 camera_index;
        GLuint camera_uniform_buffer;

        GLuint lighting_uniform_buffer;

        GLuint render_bounds_vertex_buffer;
        GLuint render_bounds_index_buffer;
        GLuint render_bounds_vertex_array;
    };

    class OpenGLRenderDriver final : public IRenderDriver {
    public:
        void Initialize() override;
        void Render(RenderFrame *render_frame) override;
        void Shutdown() override;

        inline void ResetStats() override { m_stats = RenderStats(); }
        inline RenderStats GetStats() const override { return m_stats; }
    private:
        void ExecuteRenderFrameCommands(RenderFrame *render_frame);
        void GroupObjects(RenderFrame *render_frame, CullingResults culling_results, DrawingParametes drawing_parameters);
        void SetupPerObjectLightIndices(const RenderFrameContext &render_frame_context, GroupedObject &grouped_object, Vector3 object_position);
        void RenderCamera(const RenderFrameContextEnvironment &environment, const Array<RenderFrameContextLight> &lights, const RenderFrameContextCamera &camera, DrawingParametes drawing_parameters);
        void CopyFrameLightToOpenGLLight(const RenderFrameContextLight &frame_light, OpenGLLight &opengl_light);
        void UseMaterial(const OpenGLShader &opengl_shader, const OpenGLMaterial &opengl_material);
        void DrawSubMesh(const SubMesh &sub_mesh);
        void DrawRenderBounds(const BoundingBox &bounds);

        void LoadAssets(RenderFrameContext &render_frame_context);
        void LoadTexture2D(RenderFrameContextAssetTexture2D &texture_2d);
        void LoadShader(RenderFrameContextAssetShader &shader);
        void LoadMaterial(RenderFrameContextAssetMaterial &material);
        void LoadMesh(RenderFrameContextAssetMesh &mesh);
        void UnloadAssets(RenderFrameContext &render_frame_context);

        void SetTextureAttributes(GLuint texture, TextureAttributes attributes);
    private:
        RenderStats m_stats;

        Map<AssetId, OpenGLShader> m_opengl_shaders;
        Map<AssetId, OpenGLTexture> m_opengl_textures;
        Map<AssetId, OpenGLMaterial> m_opengl_materials;
        Map<AssetId, OpenGLMesh> m_opengl_meshes;

        OpenGLState m_state;

        Array<GroupedShader> m_grouped_shaders;
    };

}