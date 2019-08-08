#pragma once

#include "hyperion/app/window.hpp"

namespace Hyperion::Rendering {

    class CGraphicsContext {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void SetVSyncMode(EVSyncMode vsync_mode) = 0;

        template<typename... Args>
        static CGraphicsContext *Do(Args... args) {
            return nullptr;
        }

        template<typename... Args>
        static CGraphicsContext *Create(Args... args) {
            switch (CRenderAPI::GetAPI()) {
                case ERenderAPI::OpenGL: return COpenGLGraphicsContext::Create();
                default: HYP_ASSERT_ENUM_OUT_OF_RAGE; return nullptr;
            }
        }
    };

}