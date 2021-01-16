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
        u32 vertex_count = static_cast<u32>(mesh_data.positions.size());
        descriptor.vertices.resize(vertex_count);
        descriptor.indices = mesh_data.indices;

        for (u32 i = 0; i < vertex_count; i++) {
            descriptor.vertices[i].position = mesh_data.positions[i];
            descriptor.vertices[i].normal = mesh_data.normals[i];
            descriptor.vertices[i].uv = mesh_data.uvs[i];
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