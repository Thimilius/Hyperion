#pragma once

#include "hyperion/rendering/rasterizer_state.hpp"

namespace Hyperion {

    class OpenGLRasterizerState : public RasterizerState {
    public:
        OpenGLRasterizerState();

        void SetDepthTestEnabled(bool enabled) override;

        void SetStencilTestEnabled(bool enabled) override;

        void SetCullingEnabled(bool enabled) override;
        void SetCullingMode(CullingMode culling_mode) override;
        void SetFrontFaceMode(FrontFaceMode front_face_mode) override;

        void SetBlendingEnabled(bool enabled) override;
        void SetBlendingFunc(BlendFactor source_factor, BlendFactor destination_factor) override;
        void SetBlendingEquation(BlendEquation blend_equation) override;

        void SetPolygonMode(PolygonMode polygon_mode) override;
    private:
        static u32 GetGLBlendFactor(BlendFactor blend_factor);
        static u32 GetGLBlendEquation(BlendEquation blend_equation);
        static u32 GetGLFrontFaceMode(FrontFaceMode front_face_mode);
        static u32 GetGLCullingMode(CullingMode culling_mode);
        static u32 GetGLPolygonMode(PolygonMode polygon_mode);
    };

}