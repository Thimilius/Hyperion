#pragma once

namespace Hyperion::Rendering {

    enum class ERenderAPI {
        None,
        OpenGL
    };

    class CRenderAPI {
    private:
        // TODO: Who sets this?
        inline static ERenderAPI m_render_api = ERenderAPI::OpenGL;
    public:
        virtual void SetClearColor(float r, float g, float b, float a) = 0;
        virtual void Clear() = 0;
        virtual void SetViewport(s32 x, s32 y, s32 width, s32 height) = 0;
    public:
        inline static ERenderAPI GetAPI() { return m_render_api; }
    };

}