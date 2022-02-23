//------------------------ Header Guard ------------------------
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
    void LoadSubMesh(const aiMesh *mesh, Rendering::MeshData &mesh_data, Rendering::SubMeshes &sub_meshes);
  private:
    static Rendering::MeshTopology GetTopologyForPrimitiveType(uint32 primitive_type);
  };

}