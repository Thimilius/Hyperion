//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/system/engine_loop.hpp"
#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/render/render_backend.hpp"
#include "hyperion/render/render_context_types.hpp"
#include "hyperion/render/pipeline/render_pipeline.hpp"
#include "hyperion/render/threading/render_threading_mode.hpp"
#include "hyperion/scripting/scripting_backend.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct CoreSettings {
    EngineLoopSystem engine_loop;

    float32 max_delta_time = 0.15f;
    float32 fixed_delta_time = 1.0f / 60.0f;

    bool8 allow_altf4 = true;
  };

  struct AssetSettings {
    bool8 hot_loading = true;

    String shader_path = "assets/shaders/";
    String texture_path = "assets/textures/";
  };

  struct RenderSettings {
    Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;

    Rendering::RenderPipeline pipeline = Rendering::RenderPipeline::Forward;
    Rendering::IRenderPipeline *custom_pipeline = nullptr;

    Rendering::VSyncMode vsync_mode = Rendering::VSyncMode::DontSync;

    Rendering::RenderThreadingMode threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
  };

  struct ScriptingSettings {
    Scripting::ScriptingBackend backend = Scripting::ScriptingBackend::Dotnet;

    String assembly_name;
  };
  
  struct ApplicationSettings {
    CoreSettings core;
    WindowSettings window;
    AssetSettings assets;
    RenderSettings render;
    ScriptingSettings scripting;
  };

}
