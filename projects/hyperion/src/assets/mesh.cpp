#include "hyppch.hpp"

#include "hyperion/assets/mesh.hpp"

#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion {

    Mesh::Mesh(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes) {
        HYP_ASSERT_MESSAGE(sub_meshes.size() > 0, "Must provide at least one submesh when creating a mesh!");

        m_mesh_data = mesh_data;
        m_sub_meshes = sub_meshes;

        RecalculateBounds();

        Rendering::MeshDescriptor descriptor;

        bool has_positions = mesh_data.positions.size() > 0;
        bool has_normals = mesh_data.normals.size() > 0;
        bool has_texture0 = mesh_data.texture0.size() > 0;

        uint32 vertex_count = static_cast<uint32>(mesh_data.positions.size());
        descriptor.vertex_data.resize(vertex_count * sizeof(VertexMesh));
        VertexMesh *vertex_data = reinterpret_cast<VertexMesh *>(descriptor.vertex_data.data());
        for (uint32 i = 0; i < vertex_count; i++) {
            vertex_data[i].position = mesh_data.positions[i];
            vertex_data[i].normal = mesh_data.normals[i];
            vertex_data[i].texture0 = mesh_data.texture0[i];
        }
        descriptor.vertex_attributes = {
            { VertexAttribute::Position, VertexAttributeFormat::Float32, 3 },
            { VertexAttribute::Normal, VertexAttributeFormat::Float32, 3 },
            { VertexAttribute::Texture0, VertexAttributeFormat::Float32, 2 },
        };

        // FIXME: This is hardcoded for 32-Bit indices!
        descriptor.index_format = IndexFormat::UInt32;
        uint32 index_count = static_cast<uint32>(mesh_data.indices.size());
        descriptor.index_count = index_count;
        descriptor.index_data.resize(index_count * sizeof(uint32));
        uint32 *index_data = reinterpret_cast<uint32 *>(descriptor.index_data.data());
        for (uint32 i = 0; i < index_count; i++) {
            index_data[i] = mesh_data.indices[i];
        }

        Rendering::RenderEngine::GetRenderDriver()->CreateMesh(m_resource_id, descriptor);
    }

    Mesh::~Mesh() {
        Rendering::RenderEngine::GetRenderDriver()->FreeMesh(m_resource_id);
    }

    Mesh *Mesh::Create(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes) {
        return new Mesh(mesh_data, sub_meshes);
    }

    void Mesh::RecalculateBounds() {
        Vec3 min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        Vec3 max = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);

        for (uint32 i = 0; i < m_mesh_data.positions.size(); i++) {
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