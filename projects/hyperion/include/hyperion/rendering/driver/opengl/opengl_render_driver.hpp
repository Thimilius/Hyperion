#pragma once

//---------------------- Library Includes ----------------------
#include <glad/glad.h>

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/driver/render_driver.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct OpenGLAsset {
        AssetId id;
    };

    struct OpenGLShader : public OpenGLAsset {
        GLuint program;
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
    private:
        void ExecuteRenderFrameCommands(RenderFrame *render_frame);
        void RenderCamera(const RenderFrameContextCamera &render_frame_context_camera);
        void GroupObjects(const Array<RenderFrameContextObjectMesh> &mesh_objects, LayerMask visibility_mask);

        void LoadAssets(RenderFrameContext &render_frame_context);
        void LoadShader(RenderFrameContextAssetShader &shader);
        void LoadMaterial(RenderFrameContextAssetMaterial &material);
        void LoadMesh(RenderFrameContextAssetMesh &mesh);
        void UnloadAssets(RenderFrameContext &render_frame_context);
    private:
        Map<AssetId, OpenGLShader> m_opengl_shaders;
        Map<AssetId, OpenGLMaterial> m_opengl_materials;
        Map<AssetId, OpenGLMesh> m_opengl_meshes;

        OpenGLState state;

        Array<GroupedShader> m_grouped_shaders;
    };

}