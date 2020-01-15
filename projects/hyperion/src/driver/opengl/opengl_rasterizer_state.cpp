#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_rasterizer_state.hpp"

#include <glad/glad.h>

namespace Hyperion {

    OpenGLRasterizerState::OpenGLRasterizerState() {
        // Setup intial state
        SetDepthTestEnabled(true);
        SetDepthMaskEnabled(true);

        SetStencilTestEnabled(false);

        SetCullingEnabled(true);
        SetFrontFaceMode(FrontFaceMode::Clockwise);
        SetCullingMode(CullingMode::Back);

        SetBlendingEnabled(false);
        SetBlendingFunc(BlendFactor::SourceAlpha, BlendFactor::InverseSourceAlpha);
        SetBlendingEquation(BlendEquation::Add);

        SetPolygonMode(PolygonMode::Fill);
    }

    void OpenGLRasterizerState::SetDepthTestEnabled(bool enabled) {
        m_depth_test_enabled = enabled;
        if (enabled) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void OpenGLRasterizerState::SetDepthMaskEnabled(bool enabled) {
        m_depth_mask_enabled = enabled;
        if (enabled) {
            glDepthMask(GL_TRUE);
        } else {
            glDepthMask(GL_FALSE);
        }
    }

    void OpenGLRasterizerState::SetStencilTestEnabled(bool enabled) {
        m_stencil_test_enabled = enabled;
        if (enabled) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
    }

    void OpenGLRasterizerState::SetCullingEnabled(bool enabled) {
        m_culling_enabled = enabled;
        if (enabled) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    void OpenGLRasterizerState::SetCullingMode(CullingMode culling_mode) {
        m_culling_mode = culling_mode;
        glCullFace(GetGLCullingMode(culling_mode));
    }

    void OpenGLRasterizerState::SetFrontFaceMode(FrontFaceMode front_face_mode) {
        m_front_face_mode = front_face_mode;
        glFrontFace(GetGLFrontFaceMode(front_face_mode));
    }

    void OpenGLRasterizerState::SetBlendingEnabled(bool enabled) {
        m_blending_enabled = enabled;
        if (enabled) {
            glEnable(GL_BLEND);
        } else {
            glDisable(GL_BLEND);
        }

    }

    void OpenGLRasterizerState::SetBlendingFunc(BlendFactor source_factor, BlendFactor destination_factor) {
        m_blending_source_factor = source_factor;
        m_blending_destination_factor = destination_factor;
        glBlendFunc(GetGLBlendFactor(source_factor), GetGLBlendFactor(destination_factor));
    }

    void OpenGLRasterizerState::SetBlendingEquation(BlendEquation blend_equation) {
        m_blending_equation = blend_equation;
        glBlendEquation(GetGLBlendEquation(blend_equation));
    }

    void OpenGLRasterizerState::SetPolygonMode(PolygonMode polygon_mode) {
        m_polygon_mode = polygon_mode;
        glPolygonMode(GL_FRONT_AND_BACK, GetGLPolygonMode(polygon_mode));
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