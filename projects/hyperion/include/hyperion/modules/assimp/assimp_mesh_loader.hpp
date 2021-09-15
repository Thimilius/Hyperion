#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/mesh_loader.hpp"

//-------------------- Forward Declarations --------------------
struct aiMesh;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class AssimpMeshLoader : public IMeshLoader {
    public:
        Result<Mesh *, Error> Load(const String &path) override;
    private:
        void LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, Array<Rendering::SubMesh> &sub_meshes);
    private:
        static Graphics::GraphicsTopology GetTopologyForPrimitiveType(uint32 primitive_type);
    };

}