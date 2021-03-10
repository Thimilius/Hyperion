//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/font_loader.hpp"
#include "hyperion/assets/loader/mesh_loader.hpp"
#include "hyperion/assets/utilities/mesh_generator.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/core/io/image_loader.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Texture *AssetManager::GetTexturePrimitive(TexturePrimitive texture_primitive) {
        switch (texture_primitive) {
            case TexturePrimitive::Grid: return s_primitives.texture_grid;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Material *AssetManager::GetMaterialPrimitive(MaterialPrimitive material_primitive) {
        switch (material_primitive) {
            case MaterialPrimitive::Default:  return s_primitives.material_default;
            case MaterialPrimitive::Unlit: return s_primitives.material_unlit;
            case MaterialPrimitive::Ui: return s_primitives.material_ui;
            case MaterialPrimitive::Font: return s_primitives.material_font;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
        switch (mesh_primitive) {
            case MeshPrimitive::Quad: return s_primitives.mesh_quad;
            case MeshPrimitive::Plane: return s_primitives.mesh_plane;
            case MeshPrimitive::Cube: return s_primitives.mesh_cube;
            case MeshPrimitive::Sphere: return s_primitives.mesh_sphere;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    void AssetManager::Initialize(AssetSettings settings) {
        s_settings = settings;

        if (settings.hot_loading) {
            InitializeFileWatchers();
        }

        ImageLoader::Initialize();
        MeshLoader::Initialize();
        FontLoader::Initialize();

        InitializePrimitives();
    }

    //--------------------------------------------------------------
    void AssetManager::Update() {
        if (s_settings.hot_loading) {
            s_file_watcher_shaders->Update();
        }
    }

    //--------------------------------------------------------------
    template<typename T>
    void DestroyAssets(const Map<ResourceId, T *> &assets) {
        // We are going to make copies of the vectors, so that we can properly iterate through them and change their contens.
        Map<ResourceId, T *> assets_copy = assets;
        for (auto [resource_id, asset] : assets_copy) {
            Object::Destroy(asset);
        }
    }

    //--------------------------------------------------------------
    void AssetManager::Shutdown() {
        // We are explicitly destroying all assets in a certain order.
        // For example materials get always destroyed before the shaders.
        DestroyAssets(s_fonts);
        DestroyAssets(s_materials);
        DestroyAssets(s_meshes);
        DestroyAssets(s_textures);
        DestroyAssets(s_shaders);

        FontLoader::Shutdown();
        MeshLoader::Shutdown();
        ImageLoader::Shutdown();
    }

    //--------------------------------------------------------------
    void AssetManager::InitializeFileWatchers() {
        s_file_watcher_shaders = FileWatcher::Create(s_settings.shader_path, [](FileStatus status, const String &path, const String &name, const String &extension) {
            if (status == FileStatus::Modified) {
                if (extension == ".shader") {
                    // TODO: We need a way to store assets by their name or path, so we can look them up and reload them when necessary.

                    // FIXME: This is very hardcoded.
                    if (name == "font.shader") {
                        s_primitives.material_font->GetShader()->Recompile(FileSystem::ReadAllText(path));
                    } else if (name == "standard.shader") {
                        s_primitives.material_default->GetShader()->Recompile(FileSystem::ReadAllText(path));
                    } else if (name == "ui.shader") {
                        s_primitives.material_ui->GetShader()->Recompile(FileSystem::ReadAllText(path));
                    }
                }
            }
        }, false);
    }

    //--------------------------------------------------------------
    void AssetManager::InitializePrimitives() {
        s_primitives.mesh_quad = MeshGenerator::GenerateQuad(1.0f, 1.0f);
        s_primitives.mesh_plane = MeshGenerator::GeneratePlane(10.0f, 10.0f);
        s_primitives.mesh_cube = MeshGenerator::GenerateCube(1.0f);
        s_primitives.mesh_sphere = MeshGenerator::GenerateSphere(0.5f);

        Image *image = ImageLoader::Load("data/textures/checkerboard.png");
        s_primitives.texture_grid = Texture2D::Create(image->GetWidth(), image->GetHeight(), TextureFormat::RGB24, TextureParameters(), image->GetPixels());
        Object::Destroy(image);

        Shader *default_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/standard.shader"));
        s_primitives.material_default = Material::Create(default_shader);
        s_primitives.material_default->SetVec4("u_color", Color::White());
        s_primitives.material_default->SetTexture("u_texture", s_primitives.texture_grid);

        Shader *unlit_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/unlit.shader"));
        s_primitives.material_unlit = Material::Create(unlit_shader);
        s_primitives.material_unlit->SetVec4("u_color", Color::White());

        Shader *ui_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/ui.shader"));
        s_primitives.material_ui = Material::Create(ui_shader);

        Shader *font_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/font.shader"));
        s_primitives.material_font = Material::Create(font_shader);
    }

    //--------------------------------------------------------------
    template<typename T>
    void RegisterAssetType(Map<ResourceId, T *> &assets, Asset *asset) {
        T *real_asset = static_cast<T *>(asset);
        ResourceId resource_id = real_asset->GetResourceId();
        HYP_ASSERT(assets.find(resource_id) == assets.end());
        assets[resource_id] = real_asset;
    }

    //--------------------------------------------------------------
    void AssetManager::RegisterAsset(Asset *asset) {
        HYP_ASSERT(asset);
        
        switch (asset->GetAssetType()) {
            case AssetType::Font: RegisterAssetType<Font>(s_fonts, asset); break;
            case AssetType::Material: RegisterAssetType<Material>(s_materials, asset); break;
            case AssetType::Mesh: RegisterAssetType<Mesh>(s_meshes, asset); break;
            case AssetType::Shader: RegisterAssetType<Shader>(s_shaders, asset); break;
            case AssetType::RenderTexture: case AssetType::Texture: RegisterAssetType<Texture>(s_textures, asset); break;
        }
        
        // Any other asset type does not interest us, as we do not cache them.
    }

    //--------------------------------------------------------------
    template<typename T>
    void UnregisterAssetType(Map<ResourceId, T *> &assets, Asset *asset) {
        T *real_asset = static_cast<T *>(asset);
        ResourceId resource_id = real_asset->GetResourceId();
        HYP_ASSERT(assets.find(resource_id) != assets.end());
        assets.erase(resource_id);
    }

    //--------------------------------------------------------------
    void AssetManager::UnregisterAsset(Asset *asset) {
        HYP_ASSERT(asset);

        switch (asset->GetAssetType()) {
            case AssetType::Font: UnregisterAssetType<Font>(s_fonts, asset); break;
            case AssetType::Material: UnregisterAssetType<Material>(s_materials, asset); break;
            case AssetType::Mesh: UnregisterAssetType<Mesh>(s_meshes, asset); break;
            case AssetType::Shader: UnregisterAssetType<Shader>(s_shaders, asset); break;
            case AssetType::RenderTexture: case AssetType::Texture: UnregisterAssetType<Texture>(s_textures, asset); break;
        }

        // Any other asset type does not interest us, as we do not cache them.
    }

}