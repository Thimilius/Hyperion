#pragma once

#include "hyperion/rendering/render_api.hpp"

namespace Hyperion::Rendering {

    class COpenGLRenderAPI : public CRenderAPI {
    public:
        void EnableFeature(EFeature feature) override;
        void DisableFeature(EFeature feature) override;

        void SetFrontFaceMode(EFrontFaceMode front_face_mode) override;
        void SetCullingMode(ECullingMode culling_mode) override;

        void SetClearColor(float r, float g, float b, float a) override;
        void Clear(EClearMask mask) override;

        void SetViewport(s32 x, s32 y, s32 width, s32 height) override;

        void DrawIndexed(const PRef<CVertexArray> &vertex_array) override;
    private:
        static u32 GetGLFeature(EFeature feature);
        static u32 GetGLFrontFaceMode(EFrontFaceMode front_face_mode);
        static u32 GetGLCullingMode(ECullingMode culling_mode);
        static u32 GetGLClearMask(EClearMask mask);
    };

}