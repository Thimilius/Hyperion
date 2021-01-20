#pragma once

#include "hyperion/assets/mesh.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"

struct aiMesh;

namespace Hyperion {

    class AssimpMeshLoader : public IMeshLoader {
    public:
        Mesh *LoadMesh(const String &path) override;
    private:
        void LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, Vector<Rendering::SubMesh> &sub_meshes);

        static Rendering::MeshTopology GetMeshTopologyForPrimitiveType(uint32 primitive_type);
    };

}