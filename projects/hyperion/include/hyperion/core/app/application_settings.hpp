#pragma once

#include "hyperion/core/app/window.hpp"
#include "hyperion/rendering/render_backend.hpp"
#include "hyperion/audio/audio_backend.hpp"

namespace Hyperion {

    struct ApplicationSettings {
        struct {
            f32 max_delta_time = 0.15f;
        } app;

        struct {
            String title = "Hyperion";

            u32 width = 1280;
            u32 height = 720;

            WindowMode window_mode = WindowMode::Windowed;
            VSyncMode vsync_mode = VSyncMode::EveryVBlank;
        } window;

        struct {
            bool hot_loading = true;

            String shader_path = "data/shaders";
        } assets;

        struct {
            Rendering::RenderBackend backend = Rendering::RenderBackend::OpenGL;
        } renderer;

        struct {
            Audio::AudioBackend backend = Audio::AudioBackend::FMod;
        } audio;
    };

}