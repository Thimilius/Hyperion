#pragma once

#include "hyperion/rendering/mesh.hpp"

struct aiMesh;

namespace Hyperion {

    class MeshFactory {
    public:
        static Ref<Rendering::Mesh> CreatePlane(f32 width, f32 height);
        static Ref<Rendering::Mesh> CreateCube(f32 size);
        static Ref<Rendering::Mesh> CreateFromFile(const String &path);
    private:
        MeshFactory() = delete;
        ~MeshFactory() = delete;

        static Ref<Rendering::Mesh> LoadMesh(const String &path);
        static void LoadSubMesh(const aiMesh *mesh, Rendering::MeshData &mesh_data, Vector<Rendering::SubMesh> &sub_meshes);
    };

}