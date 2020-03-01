#include "hyppch.hpp"

#include "hyperion/rendering/mesh.hpp"

namespace Hyperion::Rendering {

    Ref<Mesh> Mesh::Create(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes) {
        return Ref<Mesh>(new Mesh(mesh_data, sub_meshes));
    }

    Mesh::Mesh(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes) {
        HYP_ASSERT_MESSAGE(sub_meshes.size() > 0, "Must provide at least one submesh when creating a mesh!");

        m_mesh_data = mesh_data;
        m_sub_meshes = sub_meshes;

        u32 vertex_count = (u32)mesh_data.positions.size();
        Vector<VertexPNU> verticies(vertex_count);
        for (u32 i = 0; i < vertex_count; i++) {
            verticies[i].position = mesh_data.positions[i];
            verticies[i].normal = mesh_data.normals[i];
            verticies[i].uv = mesh_data.uvs[i];
        }
        Ref<VertexBuffer> vertex_buffer = VertexBuffer::Create((u8*)verticies.data(), vertex_count * sizeof(VertexPNU));
        vertex_buffer->SetLayout(VertexPNU::GetBufferLayout());

        u32 index_count = (u32)mesh_data.indicies.size();
        Ref<IndexBuffer> index_buffer = IndexBuffer::Create(mesh_data.indicies.data(), index_count);

        m_vertex_array = VertexArray::Create();
        m_vertex_array->AddVertexBuffer(vertex_buffer);
        m_vertex_array->SetIndexBuffer(index_buffer);

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

        m_bounds = BoundingBox(min, max);
    }

}