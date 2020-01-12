#pragma once

#include "hyperion/assets/mesh_loader.hpp"

struct aiMesh;

namespace Hyperion {

    class AssimpMeshLoader : public MeshLoader {
    public:
        Ref<Rendering::Mesh> LoadMesh(const String &path) override;
    private:
        void LoadSubMesh(const aiMesh *mesh, Rendering::MeshData &mesh_data, Vector<Rendering::SubMesh> &sub_meshes);
    };

}