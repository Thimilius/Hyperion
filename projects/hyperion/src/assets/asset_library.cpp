#include "hyppch.hpp"

#include "hyperion/assets/asset_library.hpp"

#include <filesystem>
#include "hyperion/core/io/file_utilities.hpp"

// TODO: Move image loading code to dedicated loader abstraction

using namespace Hyperion::Rendering;

namespace Hyperion {

    Ref<Shader> AssetLibrary::LoadShader(const String &name, const String &filepath) {
        String &source = FileUtilities::ReadTextFile(filepath);
        Ref<Shader> &shader = Shader::Create(name, source);
        AddShader(name, filepath, shader);
        return shader;
    }

    void AssetLibrary::AddShader(const String &name, const String &filepath, const Ref<Shader> &shader) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) == s_shaders.end(), "Trying to add shader that is already in the library!");
        s_shaders[name] = { shader, filepath };
    }

    Ref<Shader> AssetLibrary::GetShader(const String &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to get shader that is not in the library!");
        return s_shaders[name].asset;
    }

    Ref<Texture2D> AssetLibrary::LoadTexture2D(const String &name, const String &filepath) {
        // TODO: Add ability to specify the texture parameters

        stbi_set_flip_vertically_on_load(true);

        s32 width;
        s32 height;
        s32 channels;
        u8 *pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        Ref<Texture2D> texture;
        if (pixels) {
            TextureFormat format;
            switch (channels) {
                case 3: format = TextureFormat::RGB; break;
                case 4: format = TextureFormat::RGBA; break;
            }

            texture = Texture2D::Create(width, height, format, TextureWrapMode::Clamp, TextureFilter::Bilinear, TextureAnisotropicFilter::None, pixels);
        } else {
            HYP_LOG_ERROR("Assets", "Failed to load 2D texture from path: {}!", filepath);
        }

        stbi_image_free(pixels);
        
        AddTexture2D(name, filepath, texture);

        return texture;
    }

    void AssetLibrary::AddTexture2D(const String &name, const String &filepath, const Ref<Texture2D> &texture) {
        HYP_ASSERT_MESSAGE(s_textures.find(name) == s_textures.end(), "Trying to add 2D texture that is already in the library!");
        s_textures[name] = { texture, filepath };
    }

    Ref<Texture2D> AssetLibrary::GetTexture2D(const String &name) {
        HYP_ASSERT_MESSAGE(s_textures.find(name) != s_textures.end(), "Trying to get 2D texture that is not in the library!");
        return s_textures[name].asset;
    }

    void AssetLibrary::Init(bool hot_loading, const String &shader_path, const String &texture_path) {
        s_hot_loading = hot_loading;

        InitShaders(shader_path);
        InitTextures2D(texture_path);
    }

    void AssetLibrary::Update() {
        if (s_hot_loading) {
            s_shader_watcher->Update();
            s_texture_watcher->Update();
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
                if (status == FileStatus::Created || status == FileStatus::Modified) {
                    if (extension == ".glsl") {
                        auto name = filename.substr(0, filename.length() - 5);
                        ReloadShader(name);
                    }
                }
            }, false);
        }
    }

    void AssetLibrary::InitTextures2D(const String &texture_path) {
        for (auto &entry : std::filesystem::directory_iterator(texture_path)) {
            auto &path = entry.path();
            if (path.extension() == ".png") { // FIXME: This is hardcoded to only load .png files
                auto &filename = path.filename().string();
                LoadTexture2D(filename.substr(0, filename.length() - 4), path.string());
            }
        }

        if (s_hot_loading) {
            s_texture_watcher = FileWatcher::Create(texture_path, [](FileStatus status, const String &path, const String &filename, const String &extension) {
                if (status == FileStatus::Created || status == FileStatus::Modified) {
                    if (extension == ".png") {
                        auto name = filename.substr(0, filename.length() - 4);
                        ReloadTexture2D(name);
                    }
                }
            }, false);
        }
    }

    void AssetLibrary::ReloadShader(const String &name) {
        if (s_shaders.find(name) == s_shaders.end()) {
            // Apparently we are trying to reload a shader that is not part of the 
            HYP_LOG_WARN("Assets", "Trying to reload a shader that is not in the library!");
            return;
        }

        AssetEntry entry = s_shaders[name];
        String &source = FileUtilities::ReadTextFile(entry.filepath);
        entry.asset->Recompile(source);
    }

    void AssetLibrary::ReloadTexture2D(const String &name) {
        if (s_textures.find(name) == s_textures.end()) {
            HYP_LOG_WARN("Assets", "Trying to reload a 2D texture that is not in the library!");
            return;
        }
        
        AssetEntry entry = s_textures[name];

        stbi_set_flip_vertically_on_load(true);

        s32 width;
        s32 height;
        s32 channels;
        u8 *pixels = stbi_load(entry.filepath.c_str(), &width, &height, &channels, 0);

        if (pixels) {
            entry.asset->Resize(width, height);
            entry.asset->SetPixels(pixels);
        }

        stbi_image_free(pixels);
    }

}