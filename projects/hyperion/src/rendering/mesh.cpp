#include "hyppch.hpp"

#include "hyperion/rendering/mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hyperion::Rendering {

    Ref<Mesh> Mesh::Create(const MeshData &mesh_data) {
        return Ref<Mesh>(new Mesh(mesh_data, { }));
    }

    Ref<Mesh> Mesh::Create(const MeshData &mesh_data, const Vector<SubMesh> sub_meshes) {
        return Ref<Mesh>(new Mesh(mesh_data, sub_meshes));
    }

    Ref<Mesh> Mesh::CreatePlane(float width, float height) {
        float half_width = width / 2.0f;
        float half_height = height / 2.0f;

        // The plane should face up
        Vec3 normal = Vec3::Up();

        // Remember that we are right-handed and therefore -z is into the screen!

        MeshData mesh_data;
        mesh_data.positions.resize(4);
        mesh_data.normals.resize(4);
        mesh_data.uvs.resize(4);
        mesh_data.indicies.resize(6);

        mesh_data.positions[0] = Vec3(-half_width, 0, half_height);
        mesh_data.normals[0]   = normal;
        mesh_data.uvs[0]       = Vec2(0.0f, 0.0f);

        mesh_data.positions[1] = Vec3(-half_width, 0, -half_height);
        mesh_data.normals[1]   = normal;
        mesh_data.uvs[1]       = Vec2(0.0f, 1.0f);

        mesh_data.positions[2] = Vec3(half_width, 0, -half_height);
        mesh_data.normals[2]   = normal;
        mesh_data.uvs[2]       = Vec2(1.0f, 1.0f);

        mesh_data.positions[3] = Vec3(half_width, 0, half_height);
        mesh_data.normals[3]   = normal;
        mesh_data.uvs[3]       = Vec2(1.0f, 0.0f);

        mesh_data.indicies = {
            0, 1, 2,
            0, 2, 3
        };

        return Create(mesh_data);
    }

    Ref<Mesh> Mesh::CreateCube(float size) {
        MeshData mesh_data;
        mesh_data.positions.resize(24);
        mesh_data.normals.resize(24);
        mesh_data.uvs.resize(24);
        mesh_data.indicies.resize(36);

        float half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube (aka right-handed)!
        {
            // Forward (-z)
            mesh_data.positions[0]  = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[0]    = Vec3::Forward();
            mesh_data.uvs[0]        = Vec2(0, 0);

            mesh_data.positions[1]  = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[1]    = Vec3::Forward();
            mesh_data.uvs[1]        = Vec2(0, 1);
            
            mesh_data.positions[2]  = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[2]    = Vec3::Forward();
            mesh_data.uvs[2]        = Vec2(1, 1);
            
            mesh_data.positions[3]  = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[3]    = Vec3::Forward();
            mesh_data.uvs[3]        = Vec2(1, 0);

            // Right
            mesh_data.positions[4]  = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[4]    = Vec3::Right();
            mesh_data.uvs[4]        = Vec2(0, 0);

            mesh_data.positions[5]  = Vec3(half_size, half_size, half_size);
            mesh_data.normals[5]    = Vec3::Right();
            mesh_data.uvs[5]        = Vec2(0, 1);

            mesh_data.positions[6]  = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[6]    = Vec3::Right();
            mesh_data.uvs[6]        = Vec2(1, 1);

            mesh_data.positions[7]  = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[7]    = Vec3::Right();
            mesh_data.uvs[7]        = Vec2(1, 0);

            // Back (+z)
            mesh_data.positions[8]  = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[8]    = Vec3::Back();
            mesh_data.uvs[8]        = Vec2(0, 0);

            mesh_data.positions[9]  = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[9]    = Vec3::Back();
            mesh_data.uvs[9]        = Vec2(0, 1);

            mesh_data.positions[10] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[10]   = Vec3::Back();
            mesh_data.uvs[10]       = Vec2(1, 1);

            mesh_data.positions[11] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[11]   = Vec3::Back();
            mesh_data.uvs[11]       = Vec2(1, 0);

            // Left
            mesh_data.positions[12] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[12]   = Vec3::Left();
            mesh_data.uvs[12]       = Vec2(0, 0);

            mesh_data.positions[13] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[13]   = Vec3::Left();
            mesh_data.uvs[13]       = Vec2(0, 1);

            mesh_data.positions[14] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[14]   = Vec3::Left();
            mesh_data.uvs[14]       = Vec2(1, 1);

            mesh_data.positions[15] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[15]   = Vec3::Left();
            mesh_data.uvs[15]       = Vec2(1, 0);

            // Up
            mesh_data.positions[16] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[16]   = Vec3::Up();
            mesh_data.uvs[16]       = Vec2(0, 0);

            mesh_data.positions[17] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[17]   = Vec3::Up();
            mesh_data.uvs[17]       = Vec2(0, 1);

            mesh_data.positions[18] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[18]   = Vec3::Up();
            mesh_data.uvs[18]       = Vec2(1, 1);

            mesh_data.positions[19] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[19]   = Vec3::Up();
            mesh_data.uvs[19]       = Vec2(1, 0);

            // Down
            mesh_data.positions[20] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[20]   = Vec3::Down();
            mesh_data.uvs[20]       = Vec2(0, 0);

            mesh_data.positions[21] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[21]   = Vec3::Down();
            mesh_data.uvs[21]       = Vec2(0, 1);

            mesh_data.positions[22] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[22]   = Vec3::Down();
            mesh_data.uvs[22]       = Vec2(1, 1);

            mesh_data.positions[23] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[23]   = Vec3::Down();
            mesh_data.uvs[23]       = Vec2(1, 0);
        }

        mesh_data.indicies = {
            // Back
            0, 1, 2,
            0, 2, 3,

            // Right
            4, 5, 6,
            4, 6, 7,

            // Forward
            8, 9, 10,
            8, 10, 11,

            // Left
            12, 13, 14,
            12, 14, 15,

            // Up
            16, 17, 18,
            16, 18, 19,

            // Down
            20, 21, 22,
            20, 22, 23
        };

        return Create(mesh_data);
    }

    Ref<Mesh> Mesh::CreateFromFile(const String &path) {
        return LoadMesh(path);
    }
    
    Mesh::Mesh(const MeshData &mesh_data, const Vector<SubMesh> sub_meshes) {
        m_mesh_data = mesh_data;

        u32 vertex_count = (u32)mesh_data.positions.size();
        Vector<SVertexPNU> verticies(vertex_count);
        for (u32 i = 0; i < vertex_count; i++) {
            verticies[i].position = mesh_data.positions[i];
            verticies[i].normal = mesh_data.normals[i];
            verticies[i].uv = mesh_data.uvs[i];
        }
        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create((const u8*)verticies.data(), sizeof(SVertexPNU) * vertex_count);
        vertex_buffer->SetLayout(SVertexPNU::GetBufferLayout());

        u32 index_count = (u32)mesh_data.indicies.size();
        Ref<IndexBuffer> index_buffer = IndexBuffer::Create(mesh_data.indicies.data(), index_count);

        m_vertex_array = VertexArray::Create();
        m_vertex_array->AddVertexBuffer(vertex_buffer);
        m_vertex_array->SetIndexBuffer(index_buffer);

        if (sub_meshes.size() == 0) {
            m_sub_meshes = { SubMesh(index_count, 0, 0) };
        } else {
            m_sub_meshes = sub_meshes;
        }

        RecalculateBounds();
    }

    void Mesh::RecalculateBounds() {
        Vec3 min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        Vec3 max = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);

        for (u32 i = 0; i < m_mesh_data.positions.size(); i++) {
            Vec3 position = m_mesh_data.positions[i];
            if (position.x < min.x) {
                min.x = position.x;
            } 
            if (position.y < min.y) {
                min.y = position.y;
            }
            if (position.z < min.z) {
                min.z = position.z;
            }
            if (position.x > max.x) {
                max.x = position.x;
            }
            if (position.y > max.y) {
                max.y = position.y;
            }
            if (position.z > max.z) {
                max.z = position.z;
            }
        }

        return m_bounds.SetMinMax(min, max);
    }

    Ref<Mesh> Mesh::LoadMesh(const String &path) {
        Assimp::Importer importer;
        u32 import_flags = aiProcess_CalcTangentSpace | // Create binormals/tangents just in case
            aiProcess_Triangulate |                     // Make sure we're triangles
            aiProcess_JoinIdenticalVertices |           // Join verticies to optimize for indexed rendering
            aiProcess_SortByPType |                     // Split meshes by primitive type
            aiProcess_GenNormals |                      // Make sure we have legit normals
            aiProcess_GenUVCoords |                     // Convert UVs if required 
            aiProcess_OptimizeMeshes |                  // Batch draws where possible
            aiProcess_FlipWindingOrder |                // Flip winding order to be clockwise                                                        
            aiProcess_ValidateDataStructure;            // Validation
        const aiScene *scene = importer.ReadFile(path, import_flags);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            HYP_LOG_ERROR("Engine", "Failed to load mesh: '{}'", path);
            return nullptr;
        };
        if (scene->mNumMeshes == 0) {
            HYP_LOG_ERROR("Engine", "The loaded mesh '{}' does not contain any meshes!", path);
            return nullptr;
        }

        MeshData mesh_data;
        Vector<SubMesh> sub_meshes;
        for (u32 i = 0; i < scene->mNumMeshes; i++) {
            LoadSubMesh(scene->mMeshes[i], mesh_data, sub_meshes);
        }

        return Create(mesh_data, sub_meshes);
    }

    void Mesh::LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, Vector<SubMesh> &sub_meshes) {
        // Make sure the mesh has all necessary components
        if (!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0)) {
            HYP_LOG_ERROR("Engine", "Mesh does not contain relevant data!");
            return;
        }

        // Handle verticies
        u32 vertex_offset = (u32)mesh_data.positions.size();
        u32 vertex_size = mesh->mNumVertices;
        mesh_data.positions.reserve(vertex_offset + vertex_size);
        mesh_data.normals.reserve(vertex_offset + vertex_size);
        mesh_data.uvs.reserve(vertex_offset + vertex_size);

        for (u32 i = 0; i < vertex_size; i++) {
            aiVector3D position = mesh->mVertices[i];
            mesh_data.positions.emplace_back(Vec3(position.x, position.y, position.z));
            aiVector3D normal = mesh->mNormals[i];
            mesh_data.normals.emplace_back(Vec3(normal.x, normal.y, normal.z));
            aiVector3D uv = mesh->mTextureCoords[0][i];
            mesh_data.uvs.emplace_back(Vec2(uv.x, uv.y));
        }

        // Handle indicies
        u32 index_offset = (u32)mesh_data.indicies.size();
        u32 index_count = mesh->mNumFaces * 3;
        mesh_data.indicies.reserve(index_offset + index_count);
        for (u32 i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices != 3) {
                HYP_LOG_ERROR("Engine", "Mesh contains faces which do not have 3 indicies!");
                return;
            }

            mesh_data.indicies.emplace_back(face.mIndices[0]);
            mesh_data.indicies.emplace_back(face.mIndices[1]);
            mesh_data.indicies.emplace_back(face.mIndices[2]);
        }

        sub_meshes.push_back(SubMesh(index_count, index_offset, vertex_offset));
    }

}