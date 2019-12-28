#pragma once

#include "hyperion/core/app/window_settings.hpp"
#include "hyperion/rendering/render_backend.hpp"
#include "hyperion/audio/audio_backend.hpp"

namespace Hyperion {

    struct TimeSettings {
        f32 max_delta_time = 0.15f;
    };

    struct AssetSettings {
        bool hot_loading = true;

        String shader_path = "data/shaders";
        String texture_path = "data/textures";
    };

    struct RendererSettings {
        Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;
    };

    struct AudioSettings {
        Audio::AudioBackend backend = Audio::AudioBackend::FMod;
    };

    struct ApplicationSettings {
        TimeSettings time;
        WindowSettings window;
        AssetSettings assets;
        RendererSettings renderer;
        AudioSettings audio;
    };

}