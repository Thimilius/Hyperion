#include "hyppch.hpp"

#include "hyperion/core/app/application_settings.hpp"

#include "hyperion/core/io/file_utilities.hpp"

#include <nlohmann/json.hpp>

namespace Hyperion {

    // TODO: Is there a better way to convert strings to enums?!

    Audio::AudioBackend AudioBackendFromString(const String &string) {
        if (string == "fmod") {
            return Audio::AudioBackend::FMod;
        } else if (string == "none") {
            return Audio::AudioBackend::None;
        } else {
            return Audio::AudioBackend::None;
        }
    }

    WindowMode WindowModeFromString(const String &string) {
        if (string == "borderless") {
            return WindowMode::Borderless;
        } else if (string == "windowed") {
            return WindowMode::Windowed;
        } else {
            return WindowMode::Windowed;
        }
    }

    VSyncMode VSyncModeFromString(const String &string) {
        if (string == "dont_sync") {
            return VSyncMode::DontSync;
        } else if (string == "every_v_blank") {
            return VSyncMode::EveryVBlank;
        } else if (string == "every_second_v_blank") {
            return VSyncMode::EverySecondVBlank;
        } else {
            return VSyncMode::DontSync;
        }
    }

    ApplicationSettings ApplicationSettings::FromJsonFile(const String &path) {
        String json_text = FileUtilities::ReadAllText(path);
        auto json = nlohmann::json::parse(json_text, nullptr, false);

        ApplicationSettings settings;

        if (json.is_discarded()) {
            return settings;
        }

        auto json_time = json["time"];
        if (!json_time.is_null()) {
            auto json_time_max_delta_time = json_time["max_delta_time"];
            if (json_time_max_delta_time.is_number()) {
                settings.time.max_delta_time = json_time_max_delta_time.get<f32>();
            }
        }
        
        auto json_window = json["window"];
        if (!json_window.is_null()) {
            auto json_window_title = json_window["title"];
            if (json_window_title.is_string()) {
                settings.window.title = json_window_title.get<String>();
            }
            auto json_window_icon = json_window["icon"];
            if (json_window_icon.is_string()) {
                settings.window.icon = json_window_icon.get<String>();
            }
            auto json_window_width = json_window["width"];
            if (json_window_width.is_number_unsigned()) {
                settings.window.width = json_window_width.get<u32>();
            }
            auto json_window_height = json_window["height"];
            if (json_window_height.is_number_unsigned()) {
                settings.window.height = json_window_height.get<u32>();
            }
            auto json_window_min_width = json_window["min_width"];
            if (json_window_min_width.is_number_unsigned()) {
                settings.window.min_width = json_window_min_width.get<u32>();
            }
            auto json_window_min_height = json_window["min_height"];
            if (json_window_min_height.is_number_unsigned()) {
                settings.window.min_width = json_window_min_height.get<u32>();
            }
            auto json_window_window_mode = json_window["window_mode"];
            if (json_window_window_mode.is_string()) {
                settings.window.window_mode = WindowModeFromString(json_window_window_mode.get<String>());
            }
            auto json_window_vsync_mode = json_window["vsync_mode"];
            if (json_window_vsync_mode.is_string()) {
                settings.window.vsync_mode = VSyncModeFromString(json_window_vsync_mode.get<String>());
            }
        }

        auto json_asset = json["assets"];
        if (!json_asset.is_null()) {
            auto json_asset_hot_loading = json_asset["hot_loading"];
            if (json_asset_hot_loading.is_boolean()) {
                settings.assets.hot_loading = json_asset_hot_loading.get<bool>();
            }
            auto json_asset_shader_path = json_asset["shader_path"];
            if (json_asset_shader_path.is_string()) {
                settings.assets.shader_path = json_asset_shader_path.get<String>();
            }
            auto json_asset_texture_path = json_asset["texture_path"];
            if (json_asset_texture_path.is_string()) {
                settings.assets.texture_path = json_asset_texture_path.get<String>();
            }
        }

        auto json_audio = json["audio"];
        if (!json_audio.is_null()) {
            auto json_audio_backend = json_audio["backend"];
            if (json_audio_backend.is_string()) {
                settings.audio.backend = AudioBackendFromString(json_audio_backend.get<String>());
            }
        }

        return settings;
    }

}