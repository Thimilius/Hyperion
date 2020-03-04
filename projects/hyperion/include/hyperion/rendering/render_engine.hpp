#pragma once

#include "hyperion/rendering/render_backend.hpp"

namespace Hyperion::Rendering {

    class RenderEngine {
    private: 
        inline static RenderBackend s_render_backend;
    public:
        inline static RenderBackend GetBackend() { return s_render_backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;

        static void Init(RenderBackend backend);
        static void Shutdown();

        friend class Hyperion::Engine;
    };

}