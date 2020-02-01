#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include "hyperion/core/app/application.hpp"
#include "hyperion/driver/opengl/opengl_rasterizer_state.hpp"
#include "hyperion/driver/opengl/opengl_shader.hpp"

#include <glad/glad.h>

namespace Hyperion::Rendering {

    OpenGLRenderDriver::OpenGLRenderDriver() {
        m_rasterizer_state = std::make_unique<OpenGLRasterizerState>();

        // FIXME: Is this the right place to call that?
        OpenGLShader::Init();
    }

    void OpenGLRenderDriver::Clear(ClearMask mask) {
        glClear(GetGLClearMask(mask));
    }

    void OpenGLRenderDriver::Clear(ClearMask mask, Color color) {
        glClearColor(color.r, color.g, color.b, color.a);
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
            case RenderTextureTarget::DrawAndRead:
                glBindFramebuffer(GL_FRAMEBUFFER, id);
                break;
            case RenderTextureTarget::Draw:
                glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
                break;
            case RenderTextureTarget::Read:
                glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        if (id == 0) {
            Window *window = Application::GetInstance()->GetWindow();
            glViewport(0, 0, window->GetWidth(), window->GetHeight());
        } else {
            glViewport(0, 0, texture->GetWidth(), texture->GetHeight());
        }
    }

    void OpenGLRenderDriver::Blit(const Ref<RenderTexture> &destination, s32 dstX0, s32 dstY0, s32 dstX1, s32 dstY1, const Ref<RenderTexture> &source, s32 srcX0, s32 srcY0, s32 srcX1, s32 srcY1) {
        u32 draw_id = destination ? destination->GetID() : 0;
        s32 read_id = source ? source->GetID() : 0;
        // TODO: The clear mask of the blitting operation should get exposed
        glBlitNamedFramebuffer(read_id, draw_id, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void OpenGLRenderDriver::DrawIndexed(PrimitiveType primitive, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) {
        glDrawElementsBaseVertex(GetGLPrimitiveType(primitive), index_count, GetGLIndexFormat(format), (void*)(GetGLIndexSize(format) * index_offset), vertex_offset);
    }

    void OpenGLRenderDriver::Draw(PrimitiveType primitive, u32 vertex_count, u32 vertex_offset) {
        glDrawArrays(GetGLPrimitiveType(primitive), vertex_offset, vertex_count);
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

    u32 OpenGLRenderDriver::GetGLPrimitiveType(PrimitiveType primitive) {
        switch (primitive) {
            case PrimitiveType::Lines: return GL_LINES;
            case PrimitiveType::LineStrip: return GL_LINE_STRIP;
            case PrimitiveType::LineLoop: return GL_LINE_LOOP;
            case PrimitiveType::Triangles: return GL_TRIANGLES;
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