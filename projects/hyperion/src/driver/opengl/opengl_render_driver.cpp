#include "hyppch.hpp"

#include "hyperion/driver/opengl/opengl_render_driver.hpp"

#include <glad/glad.h>

#include "hyperion/core/app/application.hpp"
#include "hyperion/driver/opengl/opengl_rasterizer_state.hpp"
#include "hyperion/driver/opengl/opengl_shader_compiler.hpp"

namespace Hyperion::Rendering {

    OpenGLRenderDriver::OpenGLRenderDriver() {
        m_rasterizer_state = new OpenGLRasterizerState();

        // FIXME: Is this the right place to call that?
        OpenGLShaderCompiler::Init();
    }

    void OpenGLRenderDriver::Clear(ClearMask mask) {
        glClear(GetGLClearMask(mask));
    }

    void OpenGLRenderDriver::Clear(ClearMask mask, Color m_color) {
        glClearColor(m_color.r, m_color.g, m_color.b, m_color.a);
        glClear(GetGLClearMask(mask));
    }

    void OpenGLRenderDriver::SetViewport(u32 x, u32 y, u32 width, u32 height) {
        glViewport(x, y, width, height);
    }
    
    void OpenGLRenderDriver::SetActiveRenderTarget(RenderTexture *texture, RenderTextureTarget target) {
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
    }

    void OpenGLRenderDriver::Blit(RenderTexture *destination, u32 dstX0, u32 dstY0, u32 dstX1, u32 dstY1, RenderTexture *source, u32 srcX0, u32 srcY0, u32 srcX1, u32 srcY1) {
        u32 draw_id = destination ? destination->GetID() : 0;
        s32 read_id = source ? source->GetID() : 0;
        // TODO: The clear mask of the blitting operation should get exposed
        glBlitNamedFramebuffer(read_id, draw_id, srcX0, srcY0, srcX1, srcY1, dstX0, dstY0, dstX1, dstY1, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    }

    void OpenGLRenderDriver::DrawIndexed(MeshTopology topology, IndexFormat format, u32 index_count, u32 index_offset, u32 vertex_offset) {
        glDrawElementsBaseVertex(GetGLMeshTopology(topology), index_count, GetGLIndexFormat(format), reinterpret_cast<void*>(GetGLIndexSize(format) * index_offset), vertex_offset);
    }

    void OpenGLRenderDriver::Draw(MeshTopology topology, u32 vertex_count, u32 vertex_offset) {
        glDrawArrays(GetGLMeshTopology(topology), vertex_offset, vertex_count);
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

    u32 OpenGLRenderDriver::GetGLMeshTopology(MeshTopology topology) {
        switch (topology) {
            case MeshTopology::Triangles: return GL_TRIANGLES;
            case MeshTopology::Lines: return GL_LINES;
            case MeshTopology::LineStrip: return GL_LINE_STRIP;
            case MeshTopology::Points: return GL_POINTS;
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