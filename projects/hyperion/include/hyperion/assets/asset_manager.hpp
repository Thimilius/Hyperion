#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Mesh;
    class Texture;
    class Texture2D;

    namespace Rendering {
        class RenderAssetUnloadSystem;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class AssetManager final {
    private:
        AssetManager() = delete;
        ~AssetManager() = delete;
    public:
        static Mesh *CreateMesh();
        static Texture2D *CreateTexture2D();

        static void Unload(Asset *asset);
    private:
        static void Initialize();
        static void LateUpdate();
        static void Shutdown();

        static void Unregister(Asset *asset);
        static AssetInfo GetNextAssetInfo();
    private:
        inline static Array<Mesh *> s_meshes;
        inline static Array<Texture *> s_textures;

        inline static Array<Asset *> s_assets_to_unload;

        inline static AssetId s_id_counter;
    private:
        friend class Hyperion::Asset;
        friend class Hyperion::Engine;
        friend class Hyperion::Rendering::RenderAssetUnloadSystem;
    };

}