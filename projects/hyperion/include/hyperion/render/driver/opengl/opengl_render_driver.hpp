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

    struct OpenGLState {
        uint64 camera_index;
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
        void RenderCamera(const Array<RenderFrameContextLight> &lights, const RenderFrameContextCamera &render_frame_context_camera);
        void RenderSubMesh(const SubMesh &sub_mesh);
        void GroupObjects(const Array<RenderFrameContextObjectMesh> &mesh_objects, LayerMask visibility_mask);

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