#include "hyppch.hpp"

#include "hyperion/rendering/mesh.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Hyperion::Math;

namespace Hyperion::Rendering {

    TRef<CMesh> CMesh::Create(const TRef<CVertexArray> &vertex_array) {
        return Create(vertex_array, { SSubMesh(vertex_array->GetIndexBuffer()->GetCount(), 0, 0) });
    }

    TRef<CMesh> CMesh::Create(const TRef<CVertexArray> &vertex_array, const TVector<SSubMesh> sub_meshes) {
        return TRef<CMesh>(new CMesh(vertex_array, sub_meshes));
    }

    TRef<CMesh> CMesh::CreatePlane(float width, float height) {
        float half_width = width / 2.0f;
        float half_height = height / 2.0f;

        // The plane should face up
        SVec3 normal = SVec3::Up();

        SVertexPNU verticies[4];

        verticies[0].position = SVec3(half_width, 0, half_height);
        verticies[0].normal = normal;
        verticies[0].uv = SVec2(1.0f, 1.0f);

        verticies[1].position = SVec3(half_width, 0, -half_height);
        verticies[1].normal = normal;
        verticies[1].uv = SVec2(1.0f, 0.0f);

        verticies[2].position = SVec3(-half_width, 0, -half_height);
        verticies[2].normal = normal;
        verticies[2].uv = SVec2(0.0f, 0.0f);

        verticies[3].position = SVec3(-half_width, 0, half_height);
        verticies[3].normal = normal;
        verticies[3].uv = SVec2(0.0f, 1.0f);

        TRef<CVertexBuffer> vertex_buffer = CVertexBuffer::Create((u8*)verticies, sizeof(verticies));
        CBufferLayout buffer_layout = CBufferLayout({
           SBufferElement("a_position", EShaderDataType::Float3),
           SBufferElement("a_normal", EShaderDataType::Float3),
           SBufferElement("a_uv", EShaderDataType::Float2),
        });
        vertex_buffer->SetLayout(buffer_layout);

        u16 indicies[6] = {
            0, 2, 1,
            0, 3, 2
        };
        TRef<CIndexBuffer> index_buffer = CIndexBuffer::Create(indicies, sizeof(indicies));

        TRef<CVertexArray> vertex_array = CVertexArray::Create();
        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        return Create(vertex_array);
    }

    TRef<CMesh> CMesh::CreatePlane(Math::SVec2 size) {
        return CreatePlane(size.x, size.y);
    }

    TRef<CMesh> CMesh::CreateCube(float size) {
        SVertexPNU verticies[24];

        float half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube!
        {
            // Forward
            verticies[0].position = SVec3(half_size, -half_size, -half_size);
            verticies[0].normal = SVec3::Forward();
            verticies[0].uv = SVec2(0, 0);

            verticies[1].position = SVec3(half_size, half_size, -half_size);
            verticies[1].normal = SVec3::Forward();
            verticies[1].uv = SVec2(0, 1);

            verticies[2].position = SVec3(-half_size, half_size, -half_size);
            verticies[2].normal = SVec3::Forward();
            verticies[2].uv = SVec2(1, 1);

            verticies[3].position = SVec3(-half_size, -half_size, -half_size);
            verticies[3].normal = SVec3::Forward();
            verticies[3].uv = SVec2(1, 0);

            // Right
            verticies[4].position = SVec3(half_size, -half_size, half_size);
            verticies[4].normal = SVec3::Right();
            verticies[4].uv = SVec2(0, 0);

            verticies[5].position = SVec3(half_size, half_size, half_size);
            verticies[5].normal = SVec3::Right();
            verticies[5].uv = SVec2(0, 1);

            verticies[6].position = SVec3(half_size, half_size, -half_size);
            verticies[6].normal = SVec3::Right();
            verticies[6].uv = SVec2(1, 1);

            verticies[7].position = SVec3(half_size, -half_size, -half_size);
            verticies[7].normal = SVec3::Right();
            verticies[7].uv = SVec2(1, 0);

            // Back
            verticies[8].position = SVec3(-half_size, -half_size, half_size);
            verticies[8].normal = SVec3::Back();
            verticies[8].uv = SVec2(0, 0);

            verticies[9].position = SVec3(-half_size, half_size, half_size);
            verticies[9].normal = SVec3::Back();
            verticies[9].uv = SVec2(0, 1);

            verticies[10].position = SVec3(half_size, half_size, half_size);
            verticies[10].normal = SVec3::Back();
            verticies[10].uv = SVec2(1, 1);

            verticies[11].position = SVec3(half_size, -half_size, half_size);
            verticies[11].normal = SVec3::Back();
            verticies[11].uv = SVec2(1, 0);

            // Left
            verticies[12].position = SVec3(-half_size, -half_size, -half_size);
            verticies[12].normal = SVec3::Left();
            verticies[12].uv = SVec2(0, 0);

            verticies[13].position = SVec3(-half_size, half_size, -half_size);
            verticies[13].normal = SVec3::Left();
            verticies[13].uv = SVec2(0, 1);

            verticies[14].position = SVec3(-half_size, half_size, half_size);
            verticies[14].normal = SVec3::Left();
            verticies[14].uv = SVec2(1, 1);

            verticies[15].position = SVec3(-half_size, -half_size, half_size);
            verticies[15].normal = SVec3::Left();
            verticies[15].uv = SVec2(1, 0);

            // Up
            verticies[16].position = SVec3(-half_size, half_size, half_size);
            verticies[16].normal = SVec3::Up();
            verticies[16].uv = SVec2(0, 0);

            verticies[17].position = SVec3(-half_size, half_size, -half_size);
            verticies[17].normal = SVec3::Up();
            verticies[17].uv = SVec2(0, 1);

            verticies[18].position = SVec3(half_size, half_size, -half_size);
            verticies[18].normal = SVec3::Up();
            verticies[18].uv = SVec2(1, 1);

            verticies[19].position = SVec3(half_size, half_size, half_size);
            verticies[19].normal = SVec3::Up();
            verticies[19].uv = SVec2(1, 0);

            // Down
            verticies[20].position = SVec3(-half_size, -half_size, -half_size);
            verticies[20].normal = SVec3::Down();
            verticies[20].uv = SVec2(0, 0);

            verticies[21].position = SVec3(-half_size, -half_size, half_size);
            verticies[21].normal = SVec3::Down();
            verticies[21].uv = SVec2(0, 1);

            verticies[22].position = SVec3(half_size, -half_size, half_size);
            verticies[22].normal = SVec3::Down();
            verticies[22].uv = SVec2(1, 1);

            verticies[23].position = SVec3(half_size, -half_size, -half_size);
            verticies[23].normal = SVec3::Down();
            verticies[23].uv = SVec2(1, 0);
        }

        TRef<CVertexBuffer> vertex_buffer = CVertexBuffer::Create((u8 *)verticies, sizeof(verticies));
        CBufferLayout buffer_layout = CBufferLayout({
           SBufferElement("a_position", EShaderDataType::Float3),
           SBufferElement("a_normal", EShaderDataType::Float3),
           SBufferElement("a_uv", EShaderDataType::Float2),
        });
        vertex_buffer->SetLayout(buffer_layout);

        u16 indicies[] = {
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
        TRef<CIndexBuffer> index_buffer = CIndexBuffer::Create(indicies, sizeof(indicies));

        TRef<CVertexArray> vertex_array = CVertexArray::Create();
        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        return Create(vertex_array);
    }

    TRef<CMesh> CMesh::CreateFromFile(const TString &path) {
        return LoadMesh(path);
    }

    CMesh::CMesh(const TRef<CVertexArray> &vertex_array, const TVector<SSubMesh> sub_meshes) {
        m_vertex_array = vertex_array;
        m_sub_meshes = sub_meshes;
    }

    TRef<CMesh> CMesh::LoadMesh(const TString &path) {
        Assimp::Importer importer;
        u32 import_flags = aiProcess_CalcTangentSpace | // Create binormals/tangents just in case
            aiProcess_Triangulate |                     // Make sure we're triangles
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

        TVector<SVertexPNU> verticies;
        TVector<u32> indicies;
        TVector<SSubMesh> sub_meshes;
        for (u32 i = 0; i < scene->mNumMeshes; i++) {
            LoadSubMesh(scene->mMeshes[i], verticies, indicies, sub_meshes);
        }

        TRef<CVertexBuffer> vertex_buffer = CVertexBuffer::Create((u8*)verticies.data(), (u32)(verticies.size() * sizeof(SVertexPNU)));
        vertex_buffer->SetLayout(CBufferLayout({
            SBufferElement("a_position", EShaderDataType::Float3),
            SBufferElement("a_normal", EShaderDataType::Float3),
            SBufferElement("a_uv", EShaderDataType::Float2)
        }));

        TRef<CIndexBuffer> index_buffer = CIndexBuffer::Create(indicies.data(), (u32)indicies.size());

        TRef<CVertexArray> vertex_array = CVertexArray::Create();
        vertex_array->AddVertexBuffer(vertex_buffer);
        vertex_array->SetIndexBuffer(index_buffer);

        return Create(vertex_array, sub_meshes);
    }

    void CMesh::LoadSubMesh(const aiMesh *mesh, TVector<SVertexPNU> &verticies, TVector<u32> &indicies, TVector<SSubMesh> &sub_meshes) {
        // Make sure the mesh has all necessary components
        if (!mesh->HasPositions() || !mesh->HasNormals() || !mesh->HasTextureCoords(0)) {
            HYP_LOG_ERROR("Engine", "Mesh does not contain relevant data!");
            return;
        }

        // Handle verticies
        u32 verticies_offset = (u32)verticies.size();
        verticies.reserve(verticies_offset + mesh->mNumVertices);
        for (u32 i = 0; i < mesh->mNumVertices; i++) {
            SVertexPNU vertex;

            aiVector3D position = mesh->mVertices[i];
            vertex.position = SVec3(position.x, position.y, position.z);
            aiVector3D normal = mesh->mNormals[i];
            vertex.normal = SVec3(normal.x, normal.y, normal.z);
            aiVector3D uv = mesh->mTextureCoords[0][i];
            vertex.uv = SVec2(uv.x, uv.y);

            verticies.emplace_back(vertex);
        }

        // Handle indicies
        u32 indicies_offset = (u32)indicies.size();
        u32 indicies_count = mesh->mNumFaces * 3;
        indicies.reserve(indicies_offset + indicies_count);
        for (u32 i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices != 3) {
                HYP_LOG_ERROR("Engine", "Mesh contains faces which do not have 3 indicies!");
                return;
            }

            indicies.push_back(face.mIndices[0]);
            indicies.push_back(face.mIndices[1]);
            indicies.push_back(face.mIndices[2]);
        }

        sub_meshes.push_back(SSubMesh(indicies_count, indicies_offset, verticies_offset));
    }

}