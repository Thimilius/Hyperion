//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/font_types.hpp"
#include "hyperion/assets/texture_atlas_types.hpp"
#include "hyperion/core/io/file_watcher.hpp"
#include "hyperion/render/types/render_types_mesh.hpp"
#include "hyperion/render/types/render_types_texture.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class AssetLoadSystem;
  class AssetUnloadSystem;
  class Engine;
  class Font;
  class Material;
  class Mesh;
  class Shader;
  class Texture;
  class Texture2D;
  template<typename K, typename V>
  class TextureAtlas;
  class TextureAtlasBase;
  class RenderTexture;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  enum class Texture2DPrimitive {
    White,
  };

  enum class ShaderPrimitive {
    Standard,
    Unlit,
    Gizmo,
    UI,
    Font,
  };

  enum class MaterialPrimitive {
    Default,
    Unlit,
    UI,
    Font,
  };

  enum class MeshPrimitive {
    Quad,
    Plane,
    Cube,
    Sphere,
  };

  class AssetManager final {
  public:
    template<typename T>
    static T *GetAsset(AssetHandle handle) {
      auto it = s_assets.Find(handle);
      if (it == s_assets.end()) {
        HYP_LOG_ERROR("Asset", "The asset with handle {} does not exist!", handle);
        return nullptr;
      } else {
        return it->second;
      }
    }
    
    static Texture2D *GetTexture2DPrimitive(Texture2DPrimitive texture_2d_primitive);
    static Texture2D *GetTexture2D(AssetHandle handle);
    static Texture2D *CreateTexture2D(const Rendering::Texture2DParameters &parameters);
    static Texture2D *CreateTexture2D(
      const Rendering::Texture2DParameters &parameters,
      const Rendering::TexturePixelData &pixels,
      AssetDataAccess data_access = AssetDataAccess::None
    );

    static RenderTexture *GetRenderTexture(AssetHandle handle);
    static RenderTexture *CreateRenderTexture(const Rendering::RenderTextureParameters &parameters);

    template<typename K, typename V>
    static TextureAtlas<K, V> *CreateTextureAtlas(Texture2D *texture, const Map<K, TextureAtlasElement<V>> &elements) {
      AssetMetadata metadata = CreateAssetMetadata(AssetType::TextureAtlas, AssetDataAccess::ReadAndWrite);
      TextureAtlas<K, V> *texture_atlas = new TextureAtlas<K, V>(metadata, texture, elements);
      s_texture_atlases.Insert(metadata.handle, texture_atlas);
      s_assets.Insert(metadata.handle, texture_atlas);
      return texture_atlas;
    }

    static Font *CreateFont(uint32 size, float32 baseline_offset, FontCharacterSet character_set, FontAtlas *font_atlas, SpecialFontGlyphs special_glyphs);

    static Shader *GetShaderPrimitive(ShaderPrimitive shader_primitive);
    static Shader *GetShader(AssetHandle handle);
    static Shader *CreateShader(const String &path);

    static Material *GetMaterialPrimitive(MaterialPrimitive material_primitive);
    static Material *GetMaterial(AssetHandle handle);
    static Material *CreateMaterial(Shader *shader);

    static Mesh *GetMeshPrimitive(MeshPrimitive mesh_primitive);
    static Mesh *GetMesh(AssetHandle handle);
    static Mesh *CreateMesh();
    static Mesh *CreateMesh(const Rendering::MeshData &data, const Rendering::SubMeshes &sub_meshes, AssetDataAccess data_access = AssetDataAccess::None);

    static void Unload(Asset *asset);
  private:
    AssetManager() = delete;
    ~AssetManager() = delete;
  private:
    static void Initialize();
    static void PreUpdate();
    static void LateUpdate();
    static void Shutdown();

    static void UnloadAllAssets();
    
    static void InitializePrimitives();
    static void SetNewHandle(Asset *asset, const String &handle);

    static void AddDirtyAsset(Asset *asset);

    static AssetMetadata CreateAssetMetadata(AssetType type, AssetDataAccess data_access, const String &file_path = "");
  private:
    inline static Map<AssetHandle, Asset *> s_assets;
    
    inline static Map<AssetHandle, Texture *> s_textures;
    inline static Map<AssetHandle, TextureAtlasBase *> s_texture_atlases;
    inline static Map<AssetHandle, Font *> s_fonts;
    inline static Map<AssetHandle, Shader *> s_shaders;
    inline static Map<AssetHandle, Material *> s_materials;
    inline static Map<AssetHandle, Mesh *> s_meshes;
    
    inline static FileWatcher *s_shader_watcher;

    inline static struct Primitives {
      Texture2D *texture_2d_white;

      Shader *shader_standard;
      Shader *shader_unlit;
      Shader *shader_gizmo;
      Shader *shader_ui;
      Shader *shader_font;

      Material *material_default;
      Material *material_unlit;
      Material *material_ui;
      Material *material_font;

      Mesh *mesh_quad;
      Mesh *mesh_plane;
      Mesh *mesh_cube;
      Mesh *mesh_sphere;
    } s_primitives;

    inline static Array<Asset *> s_assets_to_load;
    inline static Array<Asset *> s_assets_to_unload;
  private:
    friend class Hyperion::Asset;
    friend class Hyperion::AssetLoadSystem;
    friend class Hyperion::AssetUnloadSystem;
    friend class Hyperion::Engine;
  };

}
