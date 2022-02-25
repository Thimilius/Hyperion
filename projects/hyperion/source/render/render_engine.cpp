﻿//----------------- Precompiled Header Include -----------------
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

  bool8 g_use_second_window = false;
  Window *g_second_window;

  //--------------------------------------------------------------
  void RenderEngine::SetVSyncMode(VSyncMode vsync_mode) {
    s_vsync_mode = vsync_mode;

    if (s_render_settings.threading_mode == RenderThreadingMode::SingleThreaded) {
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

    if (g_use_second_window) {
      g_second_window = new Window(WindowSettings());
      HDC device_context = GetDC(static_cast<HWND>(g_second_window->GetNativeHandle()));
      auto descriptor = RenderContextDescriptor();
      const int32 pixel_attributes[] = {
              WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
              WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
              WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
              WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
              WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
              WGL_COLOR_BITS_ARB, descriptor.color_bits,
              WGL_DEPTH_BITS_ARB, descriptor.depth_bits,
              WGL_STENCIL_BITS_ARB, descriptor.stencil_bits,
              WGL_ACCUM_BITS_ARB, 0,
              WGL_SAMPLE_BUFFERS_ARB, descriptor.msaa_samples > 0 ? 1 : 0,
              WGL_SAMPLES_ARB, descriptor.msaa_samples,
              0
      };
      int32 pixel_format;
      uint32 formats_count;
      wglChoosePixelFormatARB(device_context, pixel_attributes, nullptr, 1, &pixel_format, &formats_count);

      PIXELFORMATDESCRIPTOR pixel_format_descriptor;
      if (!DescribePixelFormat(device_context, pixel_format, sizeof(pixel_format_descriptor), &pixel_format_descriptor)) {
        HYP_PANIC_MESSAGE("OpenGL", "Failed to describe pixel format for context!");
      }
      if (!SetPixelFormat(device_context, pixel_format, &pixel_format_descriptor)) {
        HYP_PANIC_MESSAGE("OpenGL", "Failed to set pixel format for context!");
      }

      g_second_window->Show();
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::Initialize() {
    RenderGizmos::Initialize();

    s_render_pipeline->Initialize();
  }

  //--------------------------------------------------------------
  void RenderEngine::Render() {
    HYP_PROFILE_SCOPE("RenderEngine.RenderPipeline");

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
    Window *main_window = Application::GetInstance()->GetMainWindow();

    switch (s_render_settings.threading_mode) {
      case RenderThreadingMode::SingleThreaded:
      {
        SwapRenderFrames();
        RenderDriver();
        SwapBuffers();
        break;
      }
      case RenderThreadingMode::MultiThreaded:
      {
        {
          HYP_PROFILE_CATEGORY("RenderEngine.WaitForRenderDone", ProfileCategory::Wait);
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
    HYP_PROFILE_SCOPE("RenderEngine.RenderDriver");

    Window *main_window = Application::GetInstance()->GetMainWindow();
    s_render_driver_context->MakeCurrent(main_window);

    IRenderDriver *render_driver = s_render_driver_context->GetDriver();

    render_driver->HandleAssets(s_render_frame->GetAssetContext());
    render_driver->ResetStats();
    render_driver->Render(s_render_frame);

    if (g_use_second_window) {
      s_render_frame->GetAssetContext().Clear();

      s_render_driver_context->MakeCurrent(g_second_window);
      render_driver->Render(s_render_frame);
    }
  }

  //--------------------------------------------------------------
  void RenderEngine::SwapRenderFrames() {
    HYP_PROFILE_SCOPE("RenderEngine.SwapRenderFrames");

    RenderFrame *temp = s_main_frame;
    s_main_frame = s_render_frame;
    s_render_frame = temp;

    // The main frame is now the old render frame which means the async request can be read back from there.
    for (const AsyncRequest &async_request : s_main_frame->GetAsyncRequests()) {
      async_request.callback(async_request.result);
    }
    s_render_stats = s_render_driver_context->GetDriver()->GetStats();

    s_main_frame->Clear();
  }

  //--------------------------------------------------------------
  void RenderEngine::SwapBuffers() {
    HYP_PROFILE_SCOPE("RenderEngine.Present");

    Window *main_window = Application::GetInstance()->GetMainWindow();
    s_render_driver_context->SwapBuffers(main_window);

    if (g_use_second_window) {
      s_render_driver_context->SwapBuffers(g_second_window);
    }

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

    HYP_PROFILE_THREAD("Render Thread");
    while (true) {
      {
        HYP_PROFILE_CATEGORY("RenderEngine.WaitForSwapDone", ProfileCategory::Wait);
        RenderThreadSynchronization::WaitForSwapDone();

        if (s_render_thread_should_exit) {
          break;
        }
      }

      s_render_driver_context->SetVSyncMode(s_vsync_mode);

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