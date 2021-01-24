#include "hyppch.hpp"

#include "hyperion/modules/assimp/assimp_mesh_loader.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace Hyperion::Rendering;

namespace Hyperion {

    static uint32 g_assimp_import_flags =
        aiProcess_GenNormals |            // Make sure we have normals
        aiProcess_GenUVCoords |           // Make sure we have uvs
        aiProcess_CalcTangentSpace |      // Create binormals/tangents
        aiProcess_Triangulate |           // Make sure we have triangles
        aiProcess_JoinIdenticalVertices | // Join verticies to optimize for indexed rendering
        aiProcess_SortByPType |           // Split meshes by primitive type
        aiProcess_OptimizeMeshes |        // Batch draws where possible
        aiProcess_FlipWindingOrder |      // Flip winding order to be clockwise                                                        
        aiProcess_ValidateDataStructure;  // Validation

    Mesh *AssimpMeshLoader::LoadMesh(const String &path) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path, g_assimp_import_flags);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            HYP_LOG_ERROR("MeshImporter", "Failed to load mesh: '{}'", path);
            return nullptr;
        };
        if (scene->mNumMeshes == 0) {
            HYP_LOG_ERROR("MeshImporter", "The loaded mesh '{}' does not contain any meshes!", path);
            return nullptr;
        }

        MeshData mesh_data;
        Vector<SubMesh> sub_meshes;
        for (uint32 i = 0; i < scene->mNumMeshes; i++) {
            LoadSubMesh(scene->mMeshes[i], mesh_data, sub_meshes);
        }

        return Mesh::Create(mesh_data, sub_meshes);
    }

    void AssimpMeshLoader::LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, Vector<SubMesh> &sub_meshes) {
        // Make sure the mesh has all necessary components
        if (!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0)) {
            HYP_LOG_ERROR("MeshImporter", "Mesh does not contain basic relevant data!");
            return;
        }

        // Handle verticies
        uint64 vertex_offset = mesh_data.positions.size();
        uint32 vertex_size = mesh->mNumVertices;
        mesh_data.positions.reserve(vertex_offset + vertex_size);
        mesh_data.normals.reserve(vertex_offset + vertex_size);
        mesh_data.texture0.reserve(vertex_offset + vertex_size);

        for (uint32 i = 0; i < vertex_size; i++) {
            aiVector3D position = mesh->mVertices[i];
            mesh_data.positions.emplace_back(position.x, position.y, position.z);
            aiVector3D normal = mesh->mNormals[i];
            mesh_data.normals.emplace_back(normal.x, normal.y, normal.z);
            aiVector3D uv = mesh->mTextureCoords[0][i];
            mesh_data.texture0.emplace_back(uv.x, uv.y);
        }

        // Handle indices
        uint32 index_offset = static_cast<uint32>(mesh_data.indices.size());
        uint32 index_count = mesh->mNumFaces * 3;
        mesh_data.indices.reserve(index_offset + index_count);
        for (uint32 i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];

            mesh_data.indices.emplace_back(face.mIndices[0]);
            mesh_data.indices.emplace_back(face.mIndices[1]);
            mesh_data.indices.emplace_back(face.mIndices[2]);
        }

        sub_meshes.push_back({ GetMeshTopologyForPrimitiveType(mesh->mPrimitiveTypes), index_count, index_offset, static_cast<uint32>(vertex_offset) });
    }

    MeshTopology AssimpMeshLoader::GetMeshTopologyForPrimitiveType(uint32 primitive_type) {
        switch (primitive_type) {
            case aiPrimitiveType_TRIANGLE: return MeshTopology::Triangles;
            case aiPrimitiveType_LINE: return MeshTopology::Lines;
            case aiPrimitiveType_POINT: return MeshTopology::Points;
            default: HYP_LOG_ERROR("MeshImporter", "Mesh contains an unsupported primitive type!"); return MeshTopology::Triangles;
        }
    }

}