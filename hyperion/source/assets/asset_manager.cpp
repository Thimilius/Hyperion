//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/assets/loader/font_loader.hpp"
#include "hyperion/assets/loader/image_loader.hpp"
#include "hyperion/assets/loader/mesh_loader.hpp"
#include "hyperion/assets/utilities/mesh_generator.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/ecs/system/systems/asset_systems.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Texture2D *AssetManager::GetTexture2DPrimitive(Texture2DPrimitive texture_2d_primitive) {
    switch (texture_2d_primitive) {
      case Hyperion::Texture2DPrimitive::White: return s_primitives.texture_2d_white;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return nullptr;
    }
  }

  //--------------------------------------------------------------
  Texture2D *AssetManager::GetTexture2D(AssetHandle handle) {
    auto it = s_textures.Find(handle);
    if (it == s_textures.end()) {
      HYP_LOG_ERROR("Asset", "The texture with handle {} does not exist!", handle);
      return nullptr;
    } else {
      Texture *texture = it->second;
      if (texture->GetDimension() == Rendering::TextureDimension::Texture2D) {
        return static_cast<Texture2D *>(texture);
      } else {
        HYP_LOG_ERROR("Asset", "The texture with handle {} is not a 2D texture!", handle);
        return nullptr;
      }
    }
  }

  //--------------------------------------------------------------
  Texture2D *AssetManager::CreateTexture2D(const Rendering::Texture2DParameters &parameters) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Texture, AssetDataAccess::ReadAndWrite);
    Texture2D *texture = new Texture2D(metadata, parameters);
    s_textures.Insert(metadata.handle, texture);
    s_assets.Insert(metadata.handle, texture);
    return texture;
  }

  //--------------------------------------------------------------
  Texture2D *AssetManager::CreateTexture2D(
    const Rendering::Texture2DParameters &parameters,
    const Rendering::TexturePixelData &pixels,
    AssetDataAccess data_access) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Texture, data_access);
    Texture2D *texture = new Texture2D(metadata, parameters, pixels);
    s_textures.Insert(metadata.handle, texture);
    s_assets.Insert(metadata.handle, texture);
    return texture;
  }

  //--------------------------------------------------------------
  RenderTexture *AssetManager::GetRenderTexture(AssetHandle handle) {
    auto it = s_textures.Find(handle);
    if (it == s_textures.end()) {
      HYP_LOG_ERROR("Asset", "The texture with handle {} does not exist!", handle);
      return nullptr;
    } else {
      Texture *texture = it->second;
      if (texture->GetDimension() == Rendering::TextureDimension::RenderTexture) {
        return static_cast<RenderTexture *>(texture);
      } else {
        HYP_LOG_ERROR("Asset", "The texture with handle {} is not a render texture!", handle);
        return nullptr;
      }
    }
  }

  //--------------------------------------------------------------
  RenderTexture *AssetManager::CreateRenderTexture(const Rendering::RenderTextureParameters &parameters) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Texture, AssetDataAccess::None);
    RenderTexture *render_texture = new RenderTexture(metadata, parameters);
    s_textures.Insert(metadata.handle, render_texture);
    s_assets.Insert(metadata.handle, render_texture);
    return render_texture;
  }

  //--------------------------------------------------------------
  Font *AssetManager::CreateFont(
    uint32 size,
    float32 baseline_offset,
    FontCharacterSet character_set,
    FontAtlas *font_atlas,
    SpecialFontGlyphs special_glyphs) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Font, AssetDataAccess::ReadAndWrite);
    Font *font = new Font(metadata, size, baseline_offset, character_set, font_atlas, special_glyphs);
    s_fonts.Insert(metadata.handle, font);
    s_assets.Insert(metadata.handle, font);
    return font;
  }

  //--------------------------------------------------------------
  Shader *AssetManager::GetShaderPrimitive(ShaderPrimitive shader_primitive) {
    switch (shader_primitive) {
      case ShaderPrimitive::Standard: return s_primitives.shader_standard;
      case ShaderPrimitive::Unlit: return s_primitives.shader_unlit;
      case ShaderPrimitive::Gizmo: return s_primitives.shader_gizmo;
      case ShaderPrimitive::UI: return s_primitives.shader_ui;
      case ShaderPrimitive::Font: return s_primitives.shader_font;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return nullptr;
    }
  }

  //--------------------------------------------------------------
  Shader *AssetManager::GetShader(AssetHandle handle) {
    auto it = s_shaders.Find(handle);
    if (it == s_shaders.end()) {
      HYP_LOG_ERROR("Asset", "The shader with handle {} does not exist!", handle);
      return nullptr;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  Shader *AssetManager::CreateShader(const String &path) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Shader, AssetDataAccess::ReadAndWrite, path);
    String source = FileSystem::ReadAllText(path);
    Shader *shader = new Shader(metadata, source);
    s_shaders.Insert(metadata.handle, shader);
    s_assets.Insert(metadata.handle, shader);
    return shader;
  }

  //--------------------------------------------------------------
  Material *AssetManager::GetMaterialPrimitive(MaterialPrimitive material_primitive) {
    switch (material_primitive) {
      case MaterialPrimitive::Default: return s_primitives.material_default;
      case MaterialPrimitive::Unlit: return s_primitives.material_unlit;
      case MaterialPrimitive::UI: return s_primitives.material_ui;
      case MaterialPrimitive::Font: return s_primitives.material_font;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return nullptr;
    }
  }

  //--------------------------------------------------------------
  Material *AssetManager::GetMaterial(AssetHandle handle) {
    auto it = s_materials.Find(handle);
    if (it == s_materials.end()) {
      HYP_LOG_ERROR("Asset", "The material with handle {} does not exist!", handle);
      return nullptr;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  Material *AssetManager::CreateMaterial(Shader *shader) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Material, AssetDataAccess::ReadAndWrite);
    Material *material = new Material(metadata, shader);
    s_materials.Insert(metadata.handle, material);
    s_assets.Insert(metadata.handle, material);
    return material;
  }

  //--------------------------------------------------------------
  Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
    switch (mesh_primitive) {
      case MeshPrimitive::Quad: return s_primitives.mesh_quad;
      case MeshPrimitive::Plane: return s_primitives.mesh_plane;
      case MeshPrimitive::Cube: return s_primitives.mesh_cube;
      case MeshPrimitive::Sphere: return s_primitives.mesh_sphere;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        return nullptr;
    }
  }

  //--------------------------------------------------------------
  Mesh *AssetManager::GetMesh(AssetHandle handle) {
    auto it = s_meshes.Find(handle);
    if (it == s_meshes.end()) {
      HYP_LOG_ERROR("Asset", "The mesh with handle {} does not exist!", handle);
      return nullptr;
    } else {
      return it->second;
    }
  }

  //--------------------------------------------------------------
  Mesh *AssetManager::CreateMesh() {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Mesh, AssetDataAccess::ReadAndWrite);
    Mesh *mesh = new Mesh(metadata);
    s_meshes.Insert(metadata.handle, mesh);
    s_assets.Insert(metadata.handle, mesh);
    return mesh;
  }

  Mesh *AssetManager::CreateMesh(const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes, AssetDataAccess data_access) {
    AssetMetadata metadata = CreateAssetMetadata(AssetType::Mesh, data_access);
    Mesh *mesh = new Mesh(metadata, data, sub_meshes);
    s_meshes.Insert(metadata.handle, mesh);
    s_assets.Insert(metadata.handle, mesh);
    return mesh;
  }

  //--------------------------------------------------------------
  void AssetManager::Unload(Asset *asset) {
    if (asset == nullptr) {
      return;
    }

    AssetHandle asset_handle = asset->GetMetadata().handle;
    
    s_assets.Remove(asset_handle);
    
    switch (asset->GetAssetType()) {
      case AssetType::Material: {
        HYP_ASSERT(s_materials.Contains(asset_handle));
        s_materials.Remove(asset_handle);
        break;
      }
      case AssetType::Mesh: {
        HYP_ASSERT(s_meshes.Contains(asset_handle));
        s_meshes.Remove(asset_handle);
        break;
      }
      case AssetType::Shader: {
        HYP_ASSERT(s_shaders.Contains(asset_handle));
        s_shaders.Remove(asset_handle);
        break;
      }
      case AssetType::Texture: {
        HYP_ASSERT(s_textures.Contains(asset_handle));
        s_textures.Remove(asset_handle);
        break;
      }
      case AssetType::None:
      case AssetType::Font:
      case AssetType::TextureAtlas: break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    // FIXME: Check for duplicates.
    s_assets_to_unload.Add(asset);
  }

  //--------------------------------------------------------------
  void AssetManager::Initialize() {
    ImageLoader::Initialize();
    MeshLoader::Initialize();
    FontLoader::Initialize();

    InitializePrimitives();

    // TODO: Take into account asset settings.
    s_shader_watcher = new FileWatcher("assets/shaders/", [](FileWatcherFileStatus status, const String &path, const String &filename, const String &extension) {
      if (status == FileWatcherFileStatus::Modified) {
        for (auto [handle, shader] : s_shaders) {
          const AssetMetadata &metadata = shader->GetMetadata();
          if (metadata.file_path == path) {
            shader->Recompile(FileSystem::ReadAllText(metadata.file_path));
          }
        }
      }
    }, false);
  }

  //--------------------------------------------------------------
  void AssetManager::PreUpdate() {
    HYP_PROFILE_SCOPE("AssetManager.PreUpdate")

    s_shader_watcher->Update();
  }

  //--------------------------------------------------------------
  void AssetManager::LateUpdate() {
    HYP_PROFILE_SCOPE("AssetManager.LateUpdate")

    {
      AssetLoadSystem asset_load_system;
      asset_load_system.Run(nullptr);

      AssetUnloadSystem asset_unload_system;
      asset_unload_system.Run(nullptr);
    }

    for (Asset *asset : s_assets_to_unload) {
      delete asset;
    }
    s_assets_to_unload.Clear();
  }

  //--------------------------------------------------------------
  void AssetManager::Shutdown() {
    UnloadAllAssets();
    
    FontLoader::Shutdown();
    MeshLoader::Shutdown();
    ImageLoader::Shutdown();
  }

  //--------------------------------------------------------------
  void AssetManager::UnloadAllAssets() {
    for (auto [asset_handle, asset] : s_assets) {
      s_assets_to_unload.Add(asset);
    }
    s_assets.Clear();
    
    AssetUnloadSystem asset_unload_system;
    asset_unload_system.Run(nullptr);

    for (Asset *asset : s_assets_to_unload) {
      delete asset;
    }
    s_assets_to_unload.Clear();
  }

  //--------------------------------------------------------------
  void AssetManager::InitializePrimitives() {
    Rendering::Texture2DParameters texture_parameters;
    texture_parameters.format = Rendering::TextureFormat::RGBA32;
    texture_parameters.width = 2;
    texture_parameters.height = 2;
    Array<byte> texture_pixels;
    texture_pixels.Resize(static_cast<uint64>(4 * 4), 0xFF);
    s_primitives.texture_2d_white = CreateTexture2D(texture_parameters, texture_pixels);
    SetNewHandle(s_primitives.texture_2d_white, "{DAD9FD91-8932-4A1E-B086-56F64DC20EF7}");

    s_primitives.shader_standard = CreateShader("assets/shaders/standard.shader");
    SetNewHandle(s_primitives.shader_standard, "{6AFEA19E-547B-41F5-A008-4473AE771E06}");
    s_primitives.shader_unlit = CreateShader("assets/shaders/unlit.shader");
    SetNewHandle(s_primitives.shader_unlit, "{23AA53FE-6A47-4571-BC47-00EAAFA2F54B}");
    s_primitives.shader_gizmo = CreateShader("assets/shaders/gizmo.shader");
    SetNewHandle(s_primitives.shader_gizmo, "{F05F02F1-A7E1-42B7-9618-F13AB38BCA87}");
    s_primitives.shader_ui = CreateShader("assets/shaders/ui.shader");
    SetNewHandle(s_primitives.shader_ui, "{D1D71E77-6EA7-4B5D-A7D3-C5C07D1F5386}");
    s_primitives.shader_font = CreateShader("assets/shaders/font.shader");
    SetNewHandle(s_primitives.shader_font, "{97566962-2BEF-4D77-9813-27FC6F73375F}");

    s_primitives.material_default = CreateMaterial(s_primitives.shader_standard);
    SetNewHandle(s_primitives.material_default, "{B2463C27-7FD8-44A2-BC53-2AD74FAA7979}");
    s_primitives.material_unlit = CreateMaterial(s_primitives.shader_unlit);
    SetNewHandle(s_primitives.material_unlit, "{C718D97E-A0D9-4567-AFE6-F264B8C29730}");
    s_primitives.material_ui = CreateMaterial(s_primitives.shader_ui);
    SetNewHandle(s_primitives.material_ui, "{8367D740-AF57-4C1C-AFAC-BE0C6847D8C3}");
    s_primitives.material_font = CreateMaterial(s_primitives.shader_font);
    SetNewHandle(s_primitives.material_font, "{6FCEDCC2-0156-4E78-AA2F-03AF96C3580A}");

    s_primitives.mesh_quad = MeshGenerator::GenerateQuad(1.0f, 1.0f);
    SetNewHandle(s_primitives.mesh_quad, "{D54B554E-2BED-4F36-AF12-9C20C83F4EFB}");
    s_primitives.mesh_plane = MeshGenerator::GeneratePlane(10.0f, 10.0f);
    SetNewHandle(s_primitives.mesh_plane, "{F5464C26-BA78-418D-8DFF-CC67A189DE47}");
    s_primitives.mesh_cube = MeshGenerator::GenerateCube(1.0f);
    SetNewHandle(s_primitives.mesh_cube, "{36E92468-41BB-4B06-918B-958ED7F5DD43}");
    s_primitives.mesh_sphere = MeshGenerator::GenerateSphere(0.5f);
    SetNewHandle(s_primitives.mesh_sphere, "{93DFBF96-D7DB-40B7-91C3-89C6FB1B1E49}");
  }

  //--------------------------------------------------------------
  void AssetManager::SetNewHandle(Asset *asset, const String &handle) {
    AssetHandle old_handle = asset->GetMetadata().handle;
    AssetHandle new_handle = AssetHandle(AssetHandleType::Generate(handle));

    HYP_ASSERT(s_assets.Contains(old_handle));
    s_assets.Remove(old_handle);
    s_assets.Insert(new_handle, asset);

    switch (asset->GetAssetType()) {
      case AssetType::Material: {
        Material *material = static_cast<Material *>(asset);
        HYP_ASSERT(s_materials.Contains(old_handle));
        s_materials.Remove(old_handle);
        s_materials.Insert(new_handle, material);
        break;
      }
      case AssetType::Mesh: {
        Mesh *mesh = static_cast<Mesh *>(asset);
        HYP_ASSERT(s_meshes.Contains(old_handle));
        s_meshes.Remove(old_handle);
        s_meshes.Insert(new_handle, mesh);
        break;
      }
      case AssetType::Shader: {
        Shader *shader = static_cast<Shader *>(asset);
        HYP_ASSERT(s_shaders.Contains(old_handle));
        s_shaders.Remove(old_handle);
        s_shaders.Insert(new_handle, shader);
        break;
      }
      case AssetType::Texture: {
        Texture *texture = static_cast<Texture *>(asset);
        HYP_ASSERT(s_textures.Contains(old_handle));
        s_textures.Remove(old_handle);
        s_textures.Insert(new_handle, texture);
        break;
      }
      case AssetType::None:
      case AssetType::Font:
      case AssetType::TextureAtlas: break;
      default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
    }

    asset->m_metadata.handle = new_handle;
  }

  //--------------------------------------------------------------
  void AssetManager::AddDirtyAsset(Asset *asset) {
    s_assets_to_load.Add(asset);
  }

  //--------------------------------------------------------------
  AssetMetadata AssetManager::CreateAssetMetadata(AssetType type, AssetDataAccess data_access, const String &file_path) {
    AssetMetadata metadata = { };
    metadata.handle = AssetHandleType::Generate();
    metadata.type = type;
    metadata.data_access = data_access;
    metadata.file_path = file_path;
    metadata.is_loaded = true;
    return metadata;
  }

}
