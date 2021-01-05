#include "hyppch.hpp"

#include "hyperion/assets/asset_manager.hpp"

#include "hyperion/assets/mesh_factory.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/core/io/image_loader.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Shader *AssetManager::LoadShader(const String &name, const String &filepath) {
        String source = FileSystem::ReadAllText(filepath);
        Shader *shader = Shader::Create(name, source);
        AddShader(name, filepath, shader);
        return shader;
    }

    void AssetManager::AddShader(const String &name, const String &filepath, Shader *shader) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) == s_shaders.end(), "Trying to add shader that is already in the library!");
        s_shaders[name] = { shader, filepath };
    }

    Shader *AssetManager::GetShader(const String &name) {
        HYP_ASSERT_MESSAGE(s_shaders.find(name) != s_shaders.end(), "Trying to get shader that is not in the library!");
        return s_shaders[name].asset;
    }

    Texture2D *AssetManager::LoadTexture2D(const String &name, const String &filepath, TextureParameters parameters) {
        Image *image = ImageLoader::Load(filepath);
        Texture2D *texture;
        if (!image->IsEmpty()) {
            texture = Texture2D::Create(image->GetWidth(), image->GetHeight(), GetTextureFormatFromImage(image), parameters, image->GetPixels());
        }

        AddTexture2D(name, filepath, texture);

        return texture;
    }

    void AssetManager::AddTexture2D(const String &name, const String &filepath, Texture2D *texture) {
        HYP_ASSERT_MESSAGE(s_textures.find(name) == s_textures.end(), "Trying to add 2D texture that is already in the library!");
        s_textures[name] = { texture, filepath };
    }

    Texture2D *AssetManager::GetTexture2D(const String &name) {
        HYP_ASSERT_MESSAGE(s_textures.find(name) != s_textures.end(), "Trying to get 2D texture that is not in the library!");
        return s_textures[name].asset;
    }

    TextureCubemap *AssetManager::LoadTextureCubemap(const String &name, const String &directory, const String &extension, TextureParameters parameters) {
        // Make sure directory has a seperator at the end
        String dir;
        if (directory.back() != '\\' || directory.back() != '/') {
            dir = directory + "/";
        }

        Map<TextureCubemapFace, Image *> images = {
            { TextureCubemapFace::PositiveX, ImageLoader::Load(dir + "right" + extension, false) },
            { TextureCubemapFace::NegativeX, ImageLoader::Load(dir + "left" + extension, false) },
            { TextureCubemapFace::PositiveY, ImageLoader::Load(dir + "top" + extension, false) },
            { TextureCubemapFace::NegativeY, ImageLoader::Load(dir + "bottom" + extension, false) },
            { TextureCubemapFace::PositiveZ, ImageLoader::Load(dir + "back" + extension, false) },
            { TextureCubemapFace::NegativeZ, ImageLoader::Load(dir + "front" + extension, false) }
        };

        Image *sample_image = images.begin()->second;
        u32 width = sample_image->GetWidth();
        u32 height = sample_image->GetHeight();
        TextureFormat format = GetTextureFormatFromImage(sample_image);

        Map<TextureCubemapFace, const u8 *> pixels;
        for (auto [face, image] : images) {
            pixels[face] = image->GetPixels();
        }

        TextureCubemap *texture_cubemap = TextureCubemap::Create(width, height, format, parameters, pixels);

        return texture_cubemap;
    }

    void AssetManager::AddTextureCubemap(const String &name, TextureCubemap *texture_cubemap) {
        HYP_ASSERT_MESSAGE(s_texture_cubemaps.find(name) == s_texture_cubemaps.end(), "Trying to add cube texture that is already in the library!");
        // NOTE: The corresponding cubemap files currently get ignored which means they can not be hotloaded
        s_texture_cubemaps[name] = { texture_cubemap, "" };
    }

    TextureCubemap *AssetManager::GetTextureCubemap(const String &name) {
        HYP_ASSERT_MESSAGE(s_textures.find(name) != s_textures.end(), "Trying to get cubemap texture that is not in the library!");
        return s_texture_cubemaps[name].asset;
    }

    Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
        switch (mesh_primitive) {
            case MeshPrimitive::Quad: return s_mesh_primitives.quad;
            case MeshPrimitive::Plane: return s_mesh_primitives.plane;
            case MeshPrimitive::Cube: return s_mesh_primitives.cube;
            case MeshPrimitive::Sphere: return s_mesh_primitives.sphere;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    void AssetManager::Init(const AssetSettings &settings) {
        s_settings = settings;

        if (!std::filesystem::exists(settings.shader_path)) {
            HYP_PANIC_MESSAGE("AssetLibrary", "Shader path: '{}' does not exist!", std::filesystem::absolute(settings.shader_path).u8string());
        }
        if (!std::filesystem::exists(settings.texture_path)) {
            HYP_PANIC_MESSAGE("AssetLibrary", "Texture path: '{}' does not exist!", std::filesystem::absolute(settings.texture_path).u8string());
        }

        ImageLoader::Init();

        InitShaders(settings.shader_path);
        InitTextures2D(settings.texture_path);
        InitMeshPrimitives();

        u8 *pixels = new u8[2 * 2 * 4];
        std::memset(pixels, 0xFF, 2 * 2 * 4);
        s_texture_primitive = Texture2D::Create(2, 2, TextureFormat::RGBA32, TextureParameters(), pixels);
        delete[] pixels;

        s_default_material = Material::Create(GetShader("standard"));
        s_default_material->SetTexture2D("u_texture", GetTexture2D("checkerboard"));
    }

    void AssetManager::Update() {
        if (s_settings.hot_loading) {
            s_shader_watcher->Update();
            s_texture_watcher->Update();
        }
    }

    void AssetManager::Shutdown() {
        ImageLoader::Shutdown();
    }

    void AssetManager::InitShaders(const String &shader_path) {
        for (auto &entry : std::filesystem::directory_iterator(shader_path)) {
            auto &path = entry.path();
            if (path.extension() == ".shader") {
                auto filename = path.filename().string();
                LoadShader(filename.substr(0, filename.find_last_of(".")), path.string());
            }
        }

        if (s_settings.hot_loading) {
            s_shader_watcher = FileWatcher::Create(shader_path, [](FileStatus status, const String &path, const String &filename, const String &extension) {
                if (status == FileStatus::Created || status == FileStatus::Modified) {
                    if (extension == ".shader") {
                        auto name = filename.substr(0, filename.find_last_of("."));
                        ReloadShader(name);
                    }
                }
            }, false);
        }
    }

    void AssetManager::InitTextures2D(const String &texture_path) {
        for (auto &entry : std::filesystem::directory_iterator(texture_path)) {
            auto &path = entry.path();
            if (ImageLoader::SupportsExtension(path.extension().u8string())) {
                auto filename = path.filename().string();
                LoadTexture2D(filename.substr(0, filename.find_last_of(".")), path.string());
            }
        }

        if (s_settings.hot_loading) {
            s_texture_watcher = FileWatcher::Create(texture_path, [](FileStatus status, const String &path, const String &filename, const String &extension) {
                if (status == FileStatus::Created || status == FileStatus::Modified) {
                    if (ImageLoader::SupportsExtension(extension)) {
                        auto name = filename.substr(0, filename.find_last_of("."));
                        ReloadTexture2D(name);
                    }
                }
            }, false);
        }
    }

    void AssetManager::ReloadShader(const String &name) {
        if (s_shaders.find(name) == s_shaders.end()) {
            HYP_LOG_WARN("AssetLibrary", "Trying to reload a shader that is not in the library!");
            return;
        }

        AssetEntry entry = s_shaders[name];
        String source = FileSystem::ReadAllText(entry.filepath);
        entry.asset->Recompile(source);
    }

    void AssetManager::ReloadTexture2D(const String &name) {
        if (s_textures.find(name) == s_textures.end()) {
            HYP_LOG_WARN("AssetLibrary", "Trying to reload a 2D texture that is not in the library!");
            return;
        }
        
        AssetEntry entry = s_textures[name];

        Image *image = ImageLoader::Load(entry.filepath);

        if (!image->IsEmpty()) {
            entry.asset->Resize(image->GetWidth(), image->GetHeight());
            entry.asset->SetPixels(image->GetPixels());
        }
    }

    void AssetManager::InitMeshPrimitives() {
        s_mesh_primitives.quad = MeshFactory::CreateQuad(1.0f, 1.0f);
        s_mesh_primitives.plane = MeshFactory::CreatePlane(10.0f, 10.0f);
        s_mesh_primitives.cube = MeshFactory::CreateCube(1.0f);
        s_mesh_primitives.sphere = MeshFactory::CreateSphere(0.5f);
    }

    TextureFormat AssetManager::GetTextureFormatFromImage(Image *image) {
        switch (image->GetChannels()) {
            case 3: return TextureFormat::RGB24;
            case 4: return TextureFormat::RGBA32;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return TextureFormat::RGBA32;
        }
    }

}
