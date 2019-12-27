#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include "hyperion/core/app/application.hpp"

namespace Hyperion::Rendering {

    void OpenGLRenderDriver::Init() {
        // Setup intial state
        SetClearColor(0, 0, 0, 1);

        EnableFeature(RenderFeature::DepthTesting);

        EnableFeature(RenderFeature::Culling);
        SetFrontFaceMode(FrontFaceMode::Clockwise);
        SetCullingMode(CullingMode::Back);

        EnableFeature(RenderFeature::Blending);
        SetBlendFunc(BlendFactor::SourceAlpha, BlendFactor::InverseSourceAlpha);

        SetPolygonMode(PolygonMode::Fill);
    }

    void OpenGLRenderDriver::Shutdown() {

    }

    void OpenGLRenderDriver::EnableFeature(RenderFeature feature) {
        glEnable(GetGLFeature(feature));
    }

    void OpenGLRenderDriver::DisableFeature(RenderFeature feature) {
        glDisable(GetGLFeature(feature));
    }

    void OpenGLRenderDriver::SetBlendFunc(BlendFactor source_factor, BlendFactor destination_factor) {
        glBlendFunc(GetGLBlendFactor(source_factor), GetGLBlendFactor(destination_factor));
    }

    void OpenGLRenderDriver::SetBlendEquation(BlendEquation blend_equation) {
        glBlendEquation(GetGLBlendEquation(blend_equation));
    }

    void OpenGLRenderDriver::SetFrontFaceMode(FrontFaceMode front_face_mode) {
        glFrontFace(GetGLFrontFaceMode(front_face_mode));
    }

    void OpenGLRenderDriver::SetCullingMode(CullingMode culling_mode) {
        glCullFace(GetGLCullingMode(culling_mode));
    }

    void OpenGLRenderDriver::SetPolygonMode(PolygonMode polygon_mode) {
        glPolygonMode(GL_FRONT_AND_BACK, GetGLPolygonMode(polygon_mode));
    }

    void OpenGLRenderDriver::SetClearColor(f32 r, f32 g, f32 b, f32 a) {
        glClearColor(r, g, b, a);
    }

    void OpenGLRenderDriver::Clear(ClearMask mask) {
        glClear(GetGLClearMask(mask));
    }

    void OpenGLRenderDriver::SetViewport(s32 x, s32 y, s32 width, s32 height) {
        glViewport(x, y, width, height);
    }

    void OpenGLRenderDriver::SetActiveRenderTarget(const Ref<RenderTexture> &texture, RenderTextureTarget target) {
        u32 id = 0;
        if (texture != nullptr) {
            id = texture->GetID();
        }

        switch (target) {
            case Hyperion::Rendering::RenderTextureTarget::DrawAndRead:
                glBindFramebuffer(GL_FRAMEBUFFER, id);
                break;
            case Hyperion::Rendering::RenderTextureTarget::Draw:
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
                break;
            case Hyperion::Rendering::RenderTextureTarget::Read:
                glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        if (id == 0) {
            Ref<Window> window = Application::GetInstance()->GetWindow();
            glViewport(0, 0, window->GetWidth(), window->GetHeight());
        } else {
            glViewport(0, 0, texture->GetWidth(), texture->GetHeight());
        }
    }

    void OpenGLRenderDriver::Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) {
        s32 draw_framebuffer_id;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &draw_framebuffer_id);
        s32 read_framebuffer_id;
        glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &read_framebuffer_id);

        if (destination == nullptr) {
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        } else {
            SetActiveRenderTarget(destination, RenderTextureTarget::Draw);
        }

        SetActiveRenderTarget(source, RenderTextureTarget::Read);
        glBlitFramebuffer(srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, draw_framebuffer_id);
        glBindFramebuffer(GL_READ_FRAMEBUFFER, read_framebuffer_id);
    }

    void OpenGLRenderDriver::DrawIndexed(Primitive primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) {
        glDrawElementsBaseVertex(GetGLPrimitive(primitive), index_count, GetGLIndexFormat(format), (void*)(GetGLIndexSize(format) * index_offset), vertex_offset);
    }

    void OpenGLRenderDriver::Draw(Primitive primitive, u32 vertex_count, u32 vertex_offset) {
        glDrawArrays(GetGLPrimitive(primitive), vertex_offset, vertex_count);
    }

    u32 OpenGLRenderDriver::GetGLFeature(RenderFeature feature) {
        switch (feature) {
            case Hyperion::Rendering::RenderFeature::Blending: return GL_BLEND;
            case Hyperion::Rendering::RenderFeature::Culling: return GL_CULL_FACE;
            case Hyperion::Rendering::RenderFeature::DepthTesting: return GL_DEPTH_TEST;
            case Hyperion::Rendering::RenderFeature::StencilTesting: return GL_STENCIL_TEST;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRenderDriver::GetGLBlendFactor(BlendFactor blend_factor) {
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

    u32 OpenGLRenderDriver::GetGLBlendEquation(BlendEquation blend_equation) {
        switch (blend_equation) {
            case BlendEquation::Add: return GL_FUNC_ADD;
            case BlendEquation::Subtract: return GL_FUNC_SUBTRACT;
            case BlendEquation::ReverseSubract: return GL_FUNC_REVERSE_SUBTRACT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRenderDriver::GetGLFrontFaceMode(FrontFaceMode front_face_mode) {
        switch (front_face_mode) {
            case FrontFaceMode::Clockwise: return GL_CW;
            case FrontFaceMode::CounterClockwise: return GL_CCW;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRenderDriver::GetGLPolygonMode(PolygonMode polygon_mode) {
        switch (polygon_mode) {
            case Hyperion::Rendering::PolygonMode::Fill: return GL_FILL;
            case Hyperion::Rendering::PolygonMode::Line: return GL_LINE;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRenderDriver::GetGLCullingMode(CullingMode culling_mode) {
        switch (culling_mode) {
            case CullingMode::Back: return GL_BACK;
            case CullingMode::Front: return GL_FRONT;
            case CullingMode::FrontAndBack: return GL_FRONT_AND_BACK;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRenderDriver::GetGLClearMask(ClearMask clear_mask) {
        u32 result = 0;

        if ((clear_mask & ClearMask::Color) == ClearMask::Color) {
            result |= GL_COLOR_BUFFER_BIT;
        }
        if ((clear_mask & ClearMask::Depth) == ClearMask::Depth) {
            result |= GL_DEPTH_BUFFER_BIT;
        }
        if ((clear_mask & ClearMask::Stencil) == ClearMask::Stencil) {
            result |= GL_STENCIL_BUFFER_BIT;
        }

        return result;
    }

    u32 OpenGLRenderDriver::GetGLPrimitive(Primitive primitive) {
        switch (primitive) {
            case Hyperion::Rendering::Primitive::Lines: return GL_LINES;
            case Hyperion::Rendering::Primitive::LineStrip: return GL_LINE_STRIP;
            case Hyperion::Rendering::Primitive::LineLoop: return GL_LINE_LOOP;
            case Hyperion::Rendering::Primitive::Triangles: return GL_TRIANGLES;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u32 OpenGLRenderDriver::GetGLIndexFormat(IndexFormat index_format) {
        switch (index_format) {
            case IndexFormat::UInt16: return GL_UNSIGNED_SHORT;
            case IndexFormat::UInt32: return GL_UNSIGNED_INT;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }

    u64 OpenGLRenderDriver::GetGLIndexSize(IndexFormat index_format) {
        switch (index_format) {
            case IndexFormat::UInt16: return sizeof(u16);
            case IndexFormat::UInt32: return sizeof(u32);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return 0;
        }
    }
}