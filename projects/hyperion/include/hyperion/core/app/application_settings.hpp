#pragma once

#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/rendering/render_backend.hpp"
#include "hyperion/rendering/render_threading_mode.hpp"
#include "hyperion/rendering/pipelines/render_pipeline.hpp"
#include "hyperion/audio/audio_backend.hpp"
#include "hyperion/physics/physics_backend.hpp"

namespace Hyperion {

    struct CoreSettings {
        f32 max_delta_time = 0.15f;
        f32 fixed_delta_time = 1.0f / 60.0f;
        bool allow_altf4 = true;
    };

    struct AssetSettings {
        bool hot_loading = true;

        String shader_path = "data/shaders";
        String texture_path = "data/textures";
    };

    struct RenderSettings {
        Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;
        Rendering::RenderThreadingMode threading_mode = Rendering::RenderThreadingMode::MultiThreaded;
        Rendering::RenderPipeline pipeline = Rendering::RenderPipeline::Forward;
        Rendering::IRenderPipeline *custom_pipeline = nullptr;
    };

    struct ApplicationSettings {
        CoreSettings core;
        WindowSettings window;
        AssetSettings assets;
        RenderSettings render;

        static ApplicationSettings FromJsonFile(const String &path);
    };

}