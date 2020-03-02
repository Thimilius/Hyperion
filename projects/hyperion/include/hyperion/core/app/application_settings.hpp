#pragma once

#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/rendering/render_backend.hpp"
#include "hyperion/rendering/render_path.hpp"
#include "hyperion/rendering/render_pipeline.hpp"
#include "hyperion/audio/audio_backend.hpp"

namespace Hyperion {

    class World;

    struct TimeSettings {
        f32 max_delta_time = 0.15f;
    };

    struct AssetSettings {
        bool hot_loading = true;

        String shader_path = "data/shaders";
        String texture_path = "data/textures";
    };

    struct RenderSettings {
        Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;
        Rendering::RenderPath path = Rendering::RenderPath::Forward;
        Rendering::RenderPipeline *custom_pipeline = nullptr;
    };

    struct AudioSettings {
        Audio::AudioBackend backend = Audio::AudioBackend::FMod;
    };

    struct EntitySettings {
        // FIXME: An actual world pointer is probably not the right way to determine the starting world
        World *start_world = nullptr;
    };

    struct ApplicationSettings {
        TimeSettings time;
        WindowSettings window;
        AssetSettings assets;
        RenderSettings render;
        AudioSettings audio;
        EntitySettings entity;

        static ApplicationSettings FromJsonFile(const String &path);
    };

}