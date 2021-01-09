#include "hyppch.hpp"

#include "hyperion/core/app/application_settings.hpp"

#include "hyperion/core/io/file_system.hpp"

namespace Hyperion {

    // TODO: Is there a better way to convert strings to enums?!

    WindowMode WindowModeFromString(const String &string, const String &path) {
        if (string == "windowed") {
            return WindowMode::Windowed;
        } else if (string == "borderless") {
            return WindowMode::Borderless;
        } else {
            HYP_LOG_ERROR("Engine", "Failed to read '{}' in '{}' as window mode!", string, path);
            return WindowMode::Windowed;
        }
    }

    ApplicationSettings ApplicationSettings::FromJsonFile(const String &path) {
        String json_text = FileSystem::ReadAllText(path);
        auto json = nlohmann::json::parse(json_text, nullptr, false);

        ApplicationSettings settings;

        if (json.is_discarded()) {
            HYP_LOG_ERROR("Engine", "Failed to load application settings from file: '{}'!", path);
            return settings;
        }

        auto &json_time = json["core"];
        if (!json_time.is_null()) {
            auto &json_time_max_delta_time = json_time["max_delta_time"];
            if (json_time_max_delta_time.is_number()) {
                settings.core.max_delta_time = json_time_max_delta_time.get<f32>();
            }
        }
        
        auto &json_window = json["window"];
        if (!json_window.is_null()) {
            auto &json_window_title = json_window["title"];
            if (json_window_title.is_string()) {
                settings.window.title = json_window_title.get<String>();
            }
            auto &json_window_icon = json_window["icon"];
            if (json_window_icon.is_string()) {
                settings.window.icon = json_window_icon.get<String>();
            }
            auto &json_window_width = json_window["width"];
            if (json_window_width.is_number_unsigned()) {
                settings.window.width = json_window_width.get<u32>();
            }
            auto &json_window_height = json_window["height"];
            if (json_window_height.is_number_unsigned()) {
                settings.window.height = json_window_height.get<u32>();
            }
            auto &json_window_min_width = json_window["min_width"];
            if (json_window_min_width.is_number_unsigned()) {
                settings.window.min_width = json_window_min_width.get<u32>();
            }
            auto &json_window_min_height = json_window["min_height"];
            if (json_window_min_height.is_number_unsigned()) {
                settings.window.min_height = json_window_min_height.get<u32>();
            }
            auto &json_window_window_mode = json_window["window_mode"];
            if (json_window_window_mode.is_string()) {
                settings.window.window_mode = WindowModeFromString(json_window_window_mode.get<String>(), path);
            }
        }

        auto &json_asset = json["assets"];
        if (!json_asset.is_null()) {
            auto &json_asset_hot_loading = json_asset["hot_loading"];
            if (json_asset_hot_loading.is_boolean()) {
                settings.assets.hot_loading = json_asset_hot_loading.get<bool>();
            }
            auto &json_asset_shader_path = json_asset["shader_path"];
            if (json_asset_shader_path.is_string()) {
                settings.assets.shader_path = json_asset_shader_path.get<String>();
            }
            auto &json_asset_texture_path = json_asset["texture_path"];
            if (json_asset_texture_path.is_string()) {
                settings.assets.texture_path = json_asset_texture_path.get<String>();
            }
        }

        return settings;
    }

}