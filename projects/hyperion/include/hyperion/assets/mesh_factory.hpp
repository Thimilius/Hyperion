#pragma once

#include "hyperion/assets/mesh_loader.hpp"

namespace Hyperion {

    class MeshFactory {
    public:
        static Ref<Rendering::Mesh> CreateQuad(f32 width, f32 height);
        static Ref<Rendering::Mesh> CreatePlane(f32 width, f32 height);
        static Ref<Rendering::Mesh> CreateCube(f32 size);
        static Ref<Rendering::Mesh> CreateFromFile(const String &path);
    private:
        MeshFactory() = delete;
        ~MeshFactory() = delete;
    private:
        static Scope<MeshLoader> s_mesh_loader;
    };

}