#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
    class Mesh;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class AssetManager final {
    private:
        AssetManager() = delete;
        ~AssetManager() = delete;
    public:
        static Mesh *CreateMesh();
    private:
        static void Initialize();
        static void Shutdown();

        static AssetInfo GetNextAssetInfo();
    private:
        inline static Array<Mesh *> s_meshes;

        inline static AssetId s_id_counter;
    private:
        friend class Hyperion::Engine;
    };

}