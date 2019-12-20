#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/graphics_context.hpp"

namespace Hyperion::Rendering {

    struct SOpenGLContextLimits {
        s32 max_msaa_samples;
        s32 max_texture_units;
    };

    class COpenGLGraphicsContext : public CGraphicsContext {
    private:
        TVector<TString> m_extensions;
        SOpenGLContextLimits m_limits;
    public:
        inline const TVector<TString> &GetExtensions() const { return m_extensions; }
        inline const SOpenGLContextLimits &GetLimits() const { return m_limits; }
    protected:
        void Init() override;
    private:
        void CheckExtensions();
        void CheckExtension(const TString &extension);

        static void GLAPIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *user_pointer);
    };

}