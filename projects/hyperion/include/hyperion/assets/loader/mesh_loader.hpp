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
        virtual Result<Mesh *, Error> Load(const String &path) = 0;
    };

    class MeshLoader final {
    public:
        inline static Result<Mesh *, Error> Load(const String &path) { return s_mesh_loader->Load(path); }
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