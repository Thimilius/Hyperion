#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/graphics/graphics_context.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Window;

    namespace Rendering {
        class IRenderPipeline;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class RenderEngine final {
    public:
        inline static Graphics::GraphicsBackend GetBackend() { return s_render_settings.graphics_backend; }
    private:
        RenderEngine() = delete;
        ~RenderEngine() = delete;
    private:
        static void PreInitialize(const RenderSettings &settings, Window *window);
        static void Initialize();
        static void Render();
        static void Shutdown();
    private: 
        inline static RenderSettings s_render_settings;
        inline static IRenderPipeline *s_render_pipeline;

        inline static Graphics::IGraphicsContext *s_graphics_context;
        inline static Graphics::GraphicsDevice *s_graphics_device;
        inline static Graphics::GraphicsDeviceContext *s_graphics_device_context;
        inline static Graphics::GraphicsSwapChain *s_graphics_swap_chain;
    private:
        friend class Hyperion::Engine;
    };

}