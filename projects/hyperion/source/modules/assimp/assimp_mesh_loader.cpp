//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/assimp/assimp_mesh_loader.hpp"

//---------------------- Library Includes ----------------------
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  static uint32 g_assimp_import_flags =
    aiProcess_GenNormals | // Make sure we have normals.
    aiProcess_GenUVCoords | // Make sure we have uvs.
    aiProcess_CalcTangentSpace | // Create binormals/tangents.
    aiProcess_Triangulate | // Make sure we have triangles.
    aiProcess_JoinIdenticalVertices | // Join verticies to optimize for indexed rendering.
    aiProcess_SortByPType | // Split meshes by primitive type.
    aiProcess_OptimizeMeshes | // Batch draws where possible.
    aiProcess_FlipWindingOrder | // Flip winding order to be clockwise.                                           
    aiProcess_ValidateDataStructure; // Validation.

  //--------------------------------------------------------------
  Result<Mesh *, Error> AssimpMeshLoader::Load(const String &path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, g_assimp_import_flags);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
      HYP_LOG_ERROR("MeshImporter", "Failed to load mesh: '{}'", path);
      return { Error::InvalidMesh };
    };
    if (scene->mNumMeshes == 0) {
      HYP_LOG_ERROR("MeshImporter", "The loaded mesh '{}' does not contain any meshes!", path);
      return { Error::InvalidMesh };
    }

    MeshData mesh_data;
    SubMeshes sub_meshes;
    for (uint32 i = 0; i < scene->mNumMeshes; i++) {
      LoadSubMesh(scene->mMeshes[i], mesh_data, sub_meshes);
    }

    return { AssetManager::CreateMesh(mesh_data, sub_meshes) };
  }

  //--------------------------------------------------------------
  void AssimpMeshLoader::LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, SubMeshes &sub_meshes) {
    // Make sure the mesh has all necessary components.
    if (!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0)) {
      HYP_LOG_ERROR("MeshImporter", "Mesh does not contain basic relevant data!");
      return;
    }

    uint64 vertex_offset = mesh_data.positions.GetLength();
    uint32 vertex_count = mesh->mNumVertices;
    mesh_data.positions.Reserve(vertex_offset + vertex_count);
    mesh_data.normals.Reserve(vertex_offset + vertex_count);
    mesh_data.texture0.Reserve(vertex_offset + vertex_count);
    for (uint32 i = 0; i < vertex_count; i++) {
      aiVector3D position = mesh->mVertices[i];
      mesh_data.positions.Add(Vector3(position.x, position.y, position.z));
      aiVector3D normal = mesh->mNormals[i];
      mesh_data.normals.Add(Vector3(normal.x, normal.y, normal.z));
      aiVector3D uv = mesh->mTextureCoords[0][i];
      mesh_data.texture0.Add(Vector2(uv.x, uv.y));
    }

    uint32 index_offset = static_cast<uint32>(mesh_data.indices.GetLength());
    uint32 index_count = mesh->mNumFaces * 3;
    mesh_data.indices.Reserve(index_offset + index_count);
    for (uint32 i = 0; i < mesh->mNumFaces; i++) {
      aiFace face = mesh->mFaces[i];

      mesh_data.indices.Add(face.mIndices[0]);
      mesh_data.indices.Add(face.mIndices[1]);
      mesh_data.indices.Add(face.mIndices[2]);
    }

    sub_meshes.Add({ GetTopologyForPrimitiveType(mesh->mPrimitiveTypes), vertex_count, static_cast<uint32>(vertex_offset), index_count, index_offset });
  }

  //--------------------------------------------------------------
  MeshTopology AssimpMeshLoader::GetTopologyForPrimitiveType(uint32 primitive_type) {
    switch (primitive_type) {
      case aiPrimitiveType_TRIANGLE: return MeshTopology::Triangles;
      case aiPrimitiveType_LINE: return MeshTopology::Lines;
      case aiPrimitiveType_POINT: return MeshTopology::Points;
      default: HYP_LOG_ERROR("MeshImporter", "Mesh contains an unsupported primitive type!");
        return MeshTopology::Triangles;
    }
  }

}
