#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class AssetManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class IMeshLoader {
    public:
        virtual ~IMeshLoader() = default;
    public:
        virtual Mesh *LoadMesh(const String &path) = 0;
    };

    class MeshLoader final {
    public:
        inline static Mesh *LoadMesh(const String &path) { s_mesh_loader->LoadMesh(path); }
    private:
        MeshLoader() = delete;
        ~MeshLoader() = delete;
    private:
        static void Initialize();
        static void Shutdown();
    private:
        inline static IMeshLoader *s_mesh_loader;
    private:
        friend class Hyperion::AssetManager;
    };

}