#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/audio/audio_backend.hpp"
#include "hyperion/core/engine_loop.hpp"
#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/physics/physics_backend.hpp"
#include "hyperion/render/render_backend.hpp"
#include "hyperion/render/driver/render_driver_context_types.hpp"
#include "hyperion/render/pipelines/render_pipeline.hpp"
#include "hyperion/render/threading/render_threading_mode.hpp"

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

        String shader_path = "data/shaders/";
        String texture_path = "data/textures/";
    };

    struct RenderSettings {
        Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;

        Rendering::RenderPipeline pipeline = Rendering::RenderPipeline::Forward;
        Rendering::IRenderPipeline *custom_pipeline = nullptr;

        Rendering::VSyncMode vsync_mode = Rendering::VSyncMode::DontSync;

        Rendering::RenderThreadingMode threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
    };

    struct ScriptingSettings {
        String runtime_path = "data/mono/";
        String library_path = "data/managed/";

        bool8 wait_for_debugger = false;
        uint32 debugger_port = 12345;
        uint32 debugger_wait_timeout = 3000;
    };

    struct ApplicationSettings {
        CoreSettings core;
        WindowSettings window;
        AssetSettings assets;
        RenderSettings render;
        ScriptingSettings scripting;
    };

}