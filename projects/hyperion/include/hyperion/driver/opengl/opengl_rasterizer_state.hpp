#pragma once

#include "hyperion/rendering/rasterizer_state.hpp"

namespace Hyperion {

    class OpenGLRasterizerState : public RasterizerState {
    public:
        void EnableFeature(RenderFeature feature) override;
        void DisableFeature(RenderFeature feature) override;

        void SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) override;
        void SetBlendEquation(BlendEquation blend_equation) override;

        void SetFrontFaceMode(FrontFaceMode front_face_mode) override;
        void SetCullingMode(CullingMode culling_mode) override;

        void SetPolygonMode(PolygonMode polygon_mode) override;
    private:
        static u32 GetGLFeature(RenderFeature feature);
        static u32 GetGLBlendFactor(BlendFactor blend_factor);
        static u32 GetGLBlendEquation(BlendEquation blend_equation);
        static u32 GetGLFrontFaceMode(FrontFaceMode front_face_mode);
        static u32 GetGLCullingMode(CullingMode culling_mode);
        static u32 GetGLPolygonMode(PolygonMode polygon_mode);
    };

}