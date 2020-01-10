#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_rasterizer_state.hpp"

#include <glad/glad.h>

namespace Hyperion {

    void OpenGLRasterizerState::EnableFeature(RenderFeature feature) {
        glEnable(GetGLFeature(feature));
    }

    void OpenGLRasterizerState::DisableFeature(RenderFeature feature) {
        glDisable(GetGLFeature(feature));
    }

    void OpenGLRasterizerState::SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) {
        glBlendFunc(GetGLBlendFactor(source_factor), GetGLBlendFactor(destination_factor));
    }

    void OpenGLRasterizerState::SetBlendEquation(BlendEquation blend_equation) {
        glBlendEquation(GetGLBlendEquation(blend_equation));
    }

    void OpenGLRasterizerState::SetFrontFaceMode(FrontFaceMode front_face_mode) {
        glFrontFace(GetGLFrontFaceMode(front_face_mode));
    }

    void OpenGLRasterizerState::SetCullingMode(CullingMode culling_mode) {
        glCullFace(GetGLCullingMode(culling_mode));
    }

    void OpenGLRasterizerState::SetPolygonMode(PolygonMode polygon_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GetGLPolygonMode(polygon_mode));
    }

    u32 OpenGLRasterizerState::GetGLFeature(RenderFeature feature) {
        switch (feature) {
            case RenderFeature::Blending: return GL_BLEND;
            case RenderFeature::Culling: return GL_CULL_FACE;
            case RenderFeature::DepthTesting: return GL_DEPTH_TEST;
            case RenderFeature::StencilTesting: return GL_STENCIL_TEST;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRasterizerState::GetGLBlendFactor(BlendFactor blend_factor) {
        switch (blend_factor) {
            case BlendFactor::Zero: return GL_ZERO;
            case BlendFactor::One: return GL_ONE;
            case BlendFactor::SourceAlpha: return GL_SRC_ALPHA;
            case BlendFactor::SourceColor: return GL_SRC_COLOR;
            case BlendFactor::DestinationAlpha: return GL_DST_ALPHA;
            case BlendFactor::DestinationColor: return GL_DST_COLOR;
            case BlendFactor::InverseSourceAlpha: return GL_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::InverseSourceColor: return GL_ONE_MINUS_SRC_COLOR;
            case BlendFactor::InverseDestinationAlpha: return GL_ONE_MINUS_DST_ALPHA;
            case BlendFactor::InverseDestinationColor: return GL_ONE_MINUS_DST_COLOR;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRasterizerState::GetGLBlendEquation(BlendEquation blend_equation) {
        switch (blend_equation) {
            case BlendEquation::Add: return GL_FUNC_ADD;
            case BlendEquation::Subtract: return GL_FUNC_SUBTRACT;
            case BlendEquation::ReverseSubract: return GL_FUNC_REVERSE_SUBTRACT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRasterizerState::GetGLFrontFaceMode(FrontFaceMode front_face_mode) {
        switch (front_face_mode) {
            case FrontFaceMode::Clockwise: return GL_CW;
            case FrontFaceMode::CounterClockwise: return GL_CCW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRasterizerState::GetGLCullingMode(CullingMode culling_mode) {
        switch (culling_mode) {
            case CullingMode::Back: return GL_BACK;
            case CullingMode::Front: return GL_FRONT;
            case CullingMode::FrontAndBack: return GL_FRONT_AND_BACK;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRasterizerState::GetGLPolygonMode(PolygonMode polygon_mode) {
        switch (polygon_mode) {
            case PolygonMode::Fill: return GL_FILL;
            case PolygonMode::Line: return GL_LINE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

}