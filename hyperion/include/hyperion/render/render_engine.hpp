//------------------------ Header Guard ------------------------
#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <atomic>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/core/app/window.hpp"
#include "hyperion/core/threading/thread.hpp"
#include "hyperion/render/render_context.hpp"
#include "hyperion/render/render_frame.hpp"
#include "hyperion/render/render_stats.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;

  namespace Rendering {
    class IRenderPipeline;
  }}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  class RenderEngine final {
  public:
    inline static RenderBackend GetBackend() { return s_render_settings.backend; }
    inline static RenderFrame *GetMainRenderFrame() { return s_main_frame; }
    inline static RenderStats GetStats() { return s_render_stats; }

    inline static IRenderPipeline *GetPipeline() { return s_render_pipeline; }

    inline static VSyncMode GetVSyncMode() { return s_vsync_mode; }
    static void SetVSyncMode(VSyncMode vsync_mode);

    inline static bool8 RenderShouldResize() { return s_render_should_resize; }
  private:
    RenderEngine() = delete;
    ~RenderEngine() = delete;
  private:
    static void PreInitialize(const RenderSettings &settings, Window *main_window);
    static void Initialize();
    static void Render();
    static void Present();
    static void Shutdown();

    static void RequestExit();

    static void InitializeGraphicsContext(Window *main_window);
    static void ShutdownGraphicsContext();

    static void RenderDriver();
    static void SwapRenderFrames();
    static void SwapBuffers();

    static void RT_Initialize(Window *main_window);
    static void RT_Loop(void *parameter);
    static void RT_Shutdown();
  private:
    inline static RenderSettings s_render_settings;
    inline static RenderStats s_render_stats;

    inline static IRenderPipeline *s_render_pipeline;
    inline static IRenderContext *s_render_driver_context;

    inline static Threading::Thread s_render_thread;
    inline static std::atomic<bool8> s_render_thread_should_exit;
    inline static std::atomic<VSyncMode> s_vsync_mode;
    inline static std::atomic<bool8> s_vsync_should_update;

    inline static RenderFrame s_first_frame;
    inline static RenderFrame s_second_frame;
    inline static RenderFrame *s_main_frame;
    inline static RenderFrame *s_render_frame;

    inline static bool8 s_main_should_resize;
    inline static bool8 s_render_should_resize;
  private:
    friend class Hyperion::Engine;
  };

}
