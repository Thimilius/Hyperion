#pragma once

#include "hyperion/assets/mesh_loader.hpp"

struct aiMesh;

namespace Hyperion {

    class AssimpMeshLoader : public IMeshLoader {
    public:
        Ref<Rendering::Mesh> LoadMesh(const String &path) override;
    private:
        void LoadSubMesh(const aiMesh *mesh, Rendering::MeshData &mesh_data, Vector<Rendering::SubMesh> &sub_meshes);

        static Rendering::MeshTopology GetMeshTopologyForPrimitiveType(u32 primitive_type);
    };

}