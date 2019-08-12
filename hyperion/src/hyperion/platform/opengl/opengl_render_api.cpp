#include "hyppch.hpp"

#include "opengl_render_api.hpp"

namespace Hyperion::Rendering {

    void COpenGLRenderAPI::EnableFeature(EFeature feature) {
        glEnable(GetGLFeature(feature));
    }

    void COpenGLRenderAPI::DisableFeature(EFeature feature) {
        glDisable(GetGLFeature(feature));
    }

    void COpenGLRenderAPI::SetBlendFunc(EBlendFactor source_factor, EBlendFactor destination_factor) {
        glBlendFunc(GetGLBlendFactor(source_factor), GetGLBlendFactor(destination_factor));
    }

    void COpenGLRenderAPI::SetBlendEquation(EBlendEquation blend_equation) {
        glBlendEquation(GetGLBlendEquation(blend_equation));
    }

    void COpenGLRenderAPI::SetFrontFaceMode(EFrontFaceMode front_face_mode) {
        glFrontFace(GetGLFrontFaceMode(front_face_mode));
    }

    void COpenGLRenderAPI::SetCullingMode(ECullingMode culling_mode) {
        glCullFace(GetGLCullingMode(culling_mode));
    }

    void COpenGLRenderAPI::SetClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

    void COpenGLRenderAPI::Clear(EClearMask mask) {
        glClear(GetGLClearMask(mask));
    }

    void COpenGLRenderAPI::SetViewport(s32 x, s32 y, s32 width, s32 height) {
        glViewport(x, y, width, height);
    }

    void COpenGLRenderAPI::DrawIndexed(const PRef<CVertexArray> &vertex_array) {
        glDrawElements(GL_TRIANGLES, vertex_array->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
    }

    u32 COpenGLRenderAPI::GetGLFeature(EFeature feature) {
        switch (feature) {
            case Hyperion::Rendering::EFeature::Blending: return GL_BLEND;
            case Hyperion::Rendering::EFeature::Culling: return GL_CULL_FACE;
            case Hyperion::Rendering::EFeature::DepthTesting: return GL_DEPTH_TEST;
            case Hyperion::Rendering::EFeature::StencilTesting: return GL_STENCIL_TEST;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 COpenGLRenderAPI::GetGLBlendFactor(EBlendFactor blend_factor) {
        switch (blend_factor) {
            case EBlendFactor::Zero: return GL_ZERO;
            case EBlendFactor::One: return GL_ONE;
            case EBlendFactor::SourceAlpha: return GL_SRC_ALPHA;
            case EBlendFactor::SourceColor: return GL_SRC_COLOR;
            case EBlendFactor::DestinationAlpha: return GL_DST_ALPHA;
            case EBlendFactor::DestinationColor: return GL_DST_COLOR;
            case EBlendFactor::InverseSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case EBlendFactor::InverseSourceColor: return GL_ONE_MINUS_SRC_COLOR;
            case EBlendFactor::InverseDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
            case EBlendFactor::InverseDestinationColor: return GL_ONE_MINUS_DST_COLOR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 COpenGLRenderAPI::GetGLBlendEquation(EBlendEquation blend_equation) {
        switch (blend_equation) {
            case EBlendEquation::Add: return GL_FUNC_ADD;
            case EBlendEquation::Subtract: return GL_FUNC_SUBTRACT;
            case EBlendEquation::ReverseSubract: return GL_FUNC_REVERSE_SUBTRACT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 COpenGLRenderAPI::GetGLFrontFaceMode(EFrontFaceMode front_face_mode) {
        switch (front_face_mode) {
            case EFrontFaceMode::Clockwise: return GL_CW;
            case EFrontFaceMode::CounterClockwise: return GL_CCW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 COpenGLRenderAPI::GetGLCullingMode(ECullingMode culling_mode) {
        switch (culling_mode) {
            case ECullingMode::Back: return GL_BACK;
            case ECullingMode::Front: return GL_FRONT;
            case ECullingMode::FrontAndBack: return GL_FRONT_AND_BACK;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 COpenGLRenderAPI::GetGLClearMask(EClearMask mask) {
        u32 result = 0;

        if ((mask & EClearMask::Color) == EClearMask::Color) {
            result |= GL_COLOR_BUFFER_BIT;
        }
        if ((mask & EClearMask::Depth) == EClearMask::Depth) {
            result |= GL_DEPTH_BUFFER_BIT;
        }
        if ((mask & EClearMask::Stencil) == EClearMask::Stencil) {
            result |= GL_STENCIL_BUFFER_BIT;
        }

        return result;
    }
}