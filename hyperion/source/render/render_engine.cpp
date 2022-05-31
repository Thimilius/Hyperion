//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/render/render_engine.hpp"

//---------------------- Library Includes ----------------------
#include <glad/glad_wgl.h>

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application.hpp"
#include "hyperion/render/pipelines/forward/forward_render_pipeline.hpp"
#include "hyperion/render/threading/render_thread_synchronization.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void RenderEngine::SetVSyncMode(VSyncMode vsync_mode) {
    s_vsync_should_update = true;
    s_vsync_mode = vsync_mode;

    if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
      HYP_PROFILE_SCOPE("RenderEngine.SetVSyncMode")
      s_render_driver_context->SetVSyncMode(s_vsync_mode);
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::PreInitialize(const RenderSettings &settings, Window *main_window) {
    s_render_settings = settings;
    s_vsync_mode = settings.vsync_mode;

    switch (s_render_settings.pipeline) {
      case RenderPipeline::Forward: s_render_pipeline = new ForwardRenderPipeline(); break;
      case RenderPipeline::Custom: s_render_pipeline = s_render_settings.custom_pipeline; break;
      case RenderPipeline::Deferred:
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    s_main_frame = &s_first_frame;
    s_render_frame = &s_second_frame;

    switch (settings.threading_mode) {
      case RenderThreadingMode::SingleThreaded: {
        InitializeGraphicsContext(main_window);
        break;
      }
      case RenderThreadingMode::MultiThreaded: {
        s_render_thread.Start(RT_Loop, main_window);
        s_render_thread.SetName("Render Thread");

        RenderThreadSynchronization::NotifyMainReady();
        RenderThreadSynchronization::WaitForRenderReady();
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::Initialize() {
    RenderGizmos::Initialize();

    s_render_pipeline->Initialize();
  }

  //--------------------------------------------------------------
  void RenderEngine::Render() {
    HYP_PROFILE_SCOPE("RenderEngine.RenderPipeline")

    const Array<RenderFrameContextCamera> context_cameras = s_main_frame->GetContext().GetCameras();
    Array<const RenderFrameContextCamera *> cameras;
    cameras.Reserve(context_cameras.GetLength());
    for (const RenderFrameContextCamera &camera : context_cameras) {
      cameras.Add(&camera);
    }
    s_render_pipeline->Render(s_main_frame, cameras);
  }

  //--------------------------------------------------------------
  void RenderEngine::Present() {
    switch (s_render_settings.threading_mode) {
      case RenderThreadingMode::SingleThreaded: {
        SwapRenderFrames();
        RenderDriver();
        SwapBuffers();
        break;
      }
      case RenderThreadingMode::MultiThreaded: {
        {
          HYP_PROFILE_CATEGORY("RenderEngine.WaitForRenderDone", ProfileCategory::Wait)
          RenderThreadSynchronization::WaitForRenderDone();
        }
        SwapRenderFrames();
        RenderThreadSynchronization::NotifySwapDone();
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::Shutdown() {
    s_render_pipeline->Shutdown();
    delete s_render_pipeline;

    switch (s_render_settings.threading_mode) {
      case RenderThreadingMode::SingleThreaded: {
        ShutdownGraphicsContext();
        break;
      }
      case RenderThreadingMode::MultiThreaded: {
        s_render_thread_should_exit = true;
        RenderThreadSynchronization::NotifySwapDone();
        s_render_thread.Join();
        break;
      }
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::RequestExit() {
    s_render_thread_should_exit = true;
  }

  //--------------------------------------------------------------
  void RenderEngine::RenderDriver() {
    HYP_PROFILE_SCOPE("RenderEngine.RenderDriver")

    IRenderDriver *render_driver = s_render_driver_context->GetDriver();

    render_driver->HandleAssets(s_render_frame->GetAssetContext());
    render_driver->ResetStats();
    render_driver->Render(s_render_frame);
  }

  //--------------------------------------------------------------
  void RenderEngine::SwapRenderFrames() {
    HYP_PROFILE_SCOPE("RenderEngine.SwapRenderFrames")

    s_render_stats = s_render_driver_context->GetDriver()->GetStats();
    s_render_should_resize = s_main_should_resize;
    
    RenderFrame *temp = s_main_frame;
    s_main_frame = s_render_frame;
    s_render_frame = temp;

    // The main frame is now the old render frame which means the async request can be read back from there.
    for (const AsyncRequestResult &async_request_result : s_main_frame->GetAsyncRequestResults()) {
      async_request_result.callback(async_request_result.result);
    }

    s_main_frame->Clear();
  }

  //--------------------------------------------------------------
  void RenderEngine::SwapBuffers() {
    HYP_PROFILE_SCOPE("RenderEngine.Present")

    Window *main_window = Application::GetInstance()->GetMainWindow();
    s_render_driver_context->SwapBuffers(main_window);

    if (s_render_settings.threading_mode == RenderThreadingMode::MultiThreaded) {
      RenderThreadSynchronization::NotifyRenderDone();
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::InitializeGraphicsContext(Window *main_window) {
    s_render_driver_context = Window::CreateRenderDriverContext(s_render_settings.backend, main_window);
    s_render_driver_context->Initialize(main_window, RenderContextDescriptor());
    s_render_driver_context->SetVSyncMode(s_vsync_mode);
    s_render_driver_context->GetDriver()->Initialize();
  }

  //--------------------------------------------------------------
  void RenderEngine::ShutdownGraphicsContext() {
    s_render_driver_context->GetDriver()->Shutdown();

    s_render_driver_context->Shutdown();
    delete s_render_driver_context;
  }

  //--------------------------------------------------------------
  void RenderEngine::RT_Initialize(Window *main_window) {
    InitializeGraphicsContext(main_window);

    RenderThreadSynchronization::NotifyRenderReady();
    RenderThreadSynchronization::WaitForMainReady();
    RenderThreadSynchronization::NotifyRenderDone();
  }

  //--------------------------------------------------------------
  void RenderEngine::RT_Loop(void *parameter) {
    Window *main_window = static_cast<Window *>(parameter);

    RT_Initialize(main_window);

    HYP_PROFILE_THREAD("Render Thread")
    while (true) {
      {
        HYP_PROFILE_CATEGORY("RenderEngine.WaitForSwapDone", ProfileCategory::Wait)
        RenderThreadSynchronization::WaitForSwapDone();

        if (s_render_thread_should_exit) {
          break;
        }
      }

      {
        HYP_PROFILE_SCOPE("RenderEngine.SetVSyncMode")
        
        if (s_vsync_should_update) {
          s_render_driver_context->SetVSyncMode(s_vsync_mode);
          s_vsync_should_update = false;
        }
      }

      RenderDriver();

      SwapBuffers();
    }

    RT_Shutdown();
  }

  //--------------------------------------------------------------
  void RenderEngine::RT_Shutdown() {
    ShutdownGraphicsContext();
  }

}
