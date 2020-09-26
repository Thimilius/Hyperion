#pragma once

#include "hyperion/assets/mesh_loader.hpp"

namespace Hyperion {

    class MeshFactory final {
    public:
        static Rendering::Mesh *CreateQuad(f32 width, f32 height);
        static Rendering::Mesh *CreatePlane(f32 width, f32 height);
        static Rendering::Mesh *CreateCube(f32 size);
        static Rendering::Mesh *CreateFromFile(const String &path);
    private:
        MeshFactory() = delete;
        ~MeshFactory() = delete;
    private:
        static IMeshLoader *s_mesh_loader;
    };

}