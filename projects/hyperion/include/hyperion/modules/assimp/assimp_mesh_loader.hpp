#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"
#include "hyperion/rendering/attributes/mesh_attributes.hpp"

//-------------------- Forward Declarations --------------------
struct aiMesh;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class AssimpMeshLoader : public IMeshLoader {
    public:
        Mesh *LoadMesh(const String &path) override;
    private:
        void LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, Vector<Rendering::SubMesh> &sub_meshes);
    private:
        static Rendering::MeshTopology GetMeshTopologyForPrimitiveType(uint32 primitive_type);
    };

}