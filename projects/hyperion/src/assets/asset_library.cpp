#include "hyppch.hpp"

#include "hyperion/assets/asset_library.hpp"

#include <filesystem>
#include "hyperion/core/io/file_utilities.hpp"

namespace Hyperion {

    Ref<Rendering::Shader> AssetLibrary::LoadShader(const String &name, const String &filepath) {
        String &source = FileUtilities::ReadTextFile(filepath);
        Ref<Rendering::Shader> &shader = Rendering::Shader::Create(name, source);
        AddShader(name, filepath, shader);
        return shader;
    }

    void AssetLibrary::AddShader(const String &name, const String &filepath, const Ref<Rendering::Shader> &shader) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) == s_shaders.end(), "Trying to add shader that is already in the library!");
        s_shaders[name] = { shader, filepath };
    }

    Ref<Rendering::Shader> AssetLibrary::GetShader(const String &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to get shader that is not in the library!");
        return s_shaders[name].shader;
    }

    void AssetLibrary::Init(bool hot_loading, const String &shader_path) {
        s_hot_loading = hot_loading;

        InitShaders(shader_path);
    }

    void AssetLibrary::Update() {
        if (s_hot_loading) {
            s_shader_watcher->Update();
        }
    }

    void AssetLibrary::Shutdown() {

    }

    void AssetLibrary::InitShaders(const String &shader_path) {
        for (auto &entry : std::filesystem::directory_iterator(shader_path)) {
            auto &path = entry.path();
            if (path.extension() == ".glsl") {
                auto &filename = path.filename().string();
                LoadShader(filename.substr(0, filename.length() - 5), path.string());
            }
        }

        if (s_hot_loading) {
            s_shader_watcher = FileWatcher::Create(shader_path, [](FileStatus status, const String &path, const String &filename, const String &extension) {
                if (extension == ".glsl") {
                    auto name = filename.substr(0, filename.length() - 5);
                    ReloadShader(name);
                }
            }, false);
        }
    }

    void AssetLibrary::ReloadShader(const String &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to reload a shader that is not in the library!");
        ShaderEntry entry = s_shaders[name];
        String &source = FileUtilities::ReadTextFile(entry.filepath);
        entry.shader->Recompile(source);
    }

}