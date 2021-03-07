//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/mesh.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    constexpr uint32 VERTEX_ATTRIBUTE_SIZE_POSITION = sizeof(float32) * 3;
    constexpr uint32 VERTEX_ATTRIBUTE_SIZE_NORMAL = sizeof(float32) * 3;
    constexpr uint32 VERTEX_ATTRIBUTE_SIZE_COLOR = sizeof(float32) * 4;
    constexpr uint32 VERTEX_ATTRIBUTE_SIZE_TEXTURE0 = sizeof(float32) * 2;

    //--------------------------------------------------------------
    Mesh::Mesh(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes, bool read_and_write_enabled) {
        HYP_ASSERT_MESSAGE(sub_meshes.size() > 0, "Must provide at least one submesh when creating a mesh!");

        RegisterAsset();

        m_read_and_write_enabled = read_and_write_enabled;
        m_bounds = CalculateBounds(mesh_data.positions);

        if (read_and_write_enabled) {
            m_mesh_data = mesh_data;
            m_sub_meshes = sub_meshes;
        }

        MeshDescriptor descriptor = { };
        descriptor.sub_meshes = sub_meshes;

        // The minimum a mesh must provide is positional data.
        HYP_ASSERT(mesh_data.positions.size() > 0);
        bool has_normals = mesh_data.normals.size() > 0;
        bool has_colors = mesh_data.colors.size() > 0;
        bool has_texture0 = mesh_data.texture0.size() > 0;

        m_vertex_format.vertex_attributes.push_back({ VertexAttributeKind::Position, VertexAttributeType::Float32, 3 });
        uint32 stride = VERTEX_ATTRIBUTE_SIZE_POSITION;

        if (has_normals) {
            HYP_ASSERT(mesh_data.normals.size() == mesh_data.positions.size());
            m_vertex_format.vertex_attributes.push_back({ VertexAttributeKind::Normal, VertexAttributeType::Float32, 3 });
            stride += VERTEX_ATTRIBUTE_SIZE_NORMAL;
        }
        if (has_colors) {
            HYP_ASSERT(mesh_data.colors.size() == mesh_data.positions.size());
            m_vertex_format.vertex_attributes.push_back({ VertexAttributeKind::Color, VertexAttributeType::Float32, 4 });
            stride += VERTEX_ATTRIBUTE_SIZE_COLOR;
        }
        if (has_texture0) {
            HYP_ASSERT(mesh_data.texture0.size() == mesh_data.positions.size());
            m_vertex_format.vertex_attributes.push_back({ VertexAttributeKind::Texture0, VertexAttributeType::Float32, 2 });
            stride += VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
        }
        m_vertex_format.stride = stride;

        descriptor.vertex_format.attributes = m_vertex_format.vertex_attributes;
        descriptor.vertex_format.stride = m_vertex_format.stride;

        uint32 vertex_count = static_cast<uint32>(mesh_data.positions.size());
        Vector<uint8> vertices(vertex_count * stride);
        for (uint32 i = 0; i < vertex_count; i++) {
            uint32 index = i * stride;
            
            Vec3 &position = reinterpret_cast<Vec3 &>(vertices[index]);
            position = mesh_data.positions[i];
            index += VERTEX_ATTRIBUTE_SIZE_POSITION;

            if (has_normals) {
                Vec3 &normal = reinterpret_cast<Vec3 &>(vertices[index]);
                normal = mesh_data.normals[i];
                index += VERTEX_ATTRIBUTE_SIZE_NORMAL;
            }
            if (has_colors) {
                Vec4 &normal = reinterpret_cast<Vec4 &>(vertices[index]);
                normal = mesh_data.colors[i];
                index += VERTEX_ATTRIBUTE_SIZE_COLOR;
            }
            if (has_texture0) {
                Vec2 &normal = reinterpret_cast<Vec2 &>(vertices[index]);
                normal = mesh_data.texture0[i];
                index += VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
            }
        }
        descriptor.vertices = vertices;

        // FIXME: This is hardcoded for 32-Bit indices!
        descriptor.index_format = IndexFormat::UInt32;
        uint32 index_count = static_cast<uint32>(mesh_data.indices.size());
        Vector<uint8> indices(index_count * sizeof(uint32));
        uint32 *index_data = reinterpret_cast<uint32 *>(indices.data());
        for (uint32 i = 0; i < index_count; i++) {
            index_data[i] = mesh_data.indices[i];
        }
        descriptor.indices = indices;

        Rendering::RenderEngine::GetRenderDriver()->CreateMesh(m_resource_id, descriptor);
    }

    //--------------------------------------------------------------
    bool Mesh::HasVertexAttribute(VertexAttributeKind vertex_attribute_kind) {
        return std::find_if(m_vertex_format.vertex_attributes.begin(), m_vertex_format.vertex_attributes.end(), [vertex_attribute_kind](const VertexAttribute &vertex_attribute) {
            return vertex_attribute.kind == vertex_attribute_kind;
        }) != m_vertex_format.vertex_attributes.end();
    }

    //--------------------------------------------------------------
    const MeshData &Mesh::GetMeshData() const {
        if (!m_read_and_write_enabled) {
            HYP_LOG_WARN("Mesh", "The data of a mesh that is not read and write enabled will not exist when queried!");
        }
        return m_mesh_data;
    }

    //--------------------------------------------------------------
    const Vector<Rendering::SubMesh> &Mesh::GetSubMeshes() const {
        if (!m_read_and_write_enabled) {
            HYP_LOG_WARN("Mesh", "The data of a mesh that is not read and write enabled will not exist when queried!");
        }
        return m_sub_meshes;
    }

    //--------------------------------------------------------------
    void Mesh::SetData(const MeshData &mesh_data, const Vector<Rendering::SubMesh> &sub_meshes) {
        // We first want to make sure that the data we are trying to set matches the format initally supplied.
        bool has_normals = mesh_data.normals.size() > 0;
        HYP_ASSERT(!has_normals || (has_normals && HasVertexAttribute(VertexAttributeKind::Normal)));
        bool has_colors = mesh_data.colors.size() > 0;
        HYP_ASSERT(!has_colors || (has_colors && HasVertexAttribute(VertexAttributeKind::Color)));
        bool has_texture0 = mesh_data.texture0.size() > 0;
        HYP_ASSERT(!has_texture0 || (has_texture0 && HasVertexAttribute(VertexAttributeKind::Texture0)));

        MeshDataDescriptor descriptor = { };
        
        uint32 vertex_count = static_cast<uint32>(mesh_data.positions.size());
        Vector<uint8> vertices(vertex_count * m_vertex_format.stride);
        for (uint32 i = 0; i < vertex_count; i++) {
            uint32 index = i * m_vertex_format.stride;

            Vec3 &position = reinterpret_cast<Vec3 &>(vertices[index]);
            position = mesh_data.positions[i];
            index += VERTEX_ATTRIBUTE_SIZE_POSITION;

            if (has_normals) {
                Vec3 &normal = reinterpret_cast<Vec3 &>(vertices[index]);
                normal = mesh_data.normals[i];
                index += VERTEX_ATTRIBUTE_SIZE_NORMAL;
            }
            if (has_colors) {
                Vec4 &normal = reinterpret_cast<Vec4 &>(vertices[index]);
                normal = mesh_data.colors[i];
                index += VERTEX_ATTRIBUTE_SIZE_COLOR;
            }
            if (has_texture0) {
                Vec2 &normal = reinterpret_cast<Vec2 &>(vertices[index]);
                normal = mesh_data.texture0[i];
                index += VERTEX_ATTRIBUTE_SIZE_TEXTURE0;
            }
        }
        descriptor.vertices = vertices;

        uint32 index_count = static_cast<uint32>(mesh_data.indices.size());
        Vector<uint8> indices(index_count * sizeof(uint32));
        uint32 *index_data = reinterpret_cast<uint32 *>(indices.data());
        for (uint32 i = 0; i < index_count; i++) {
            index_data[i] = mesh_data.indices[i];
        }
        descriptor.indices = indices;

        Rendering::RenderEngine::GetRenderDriver()->SetMeshData(m_resource_id, descriptor);
    }

    //--------------------------------------------------------------
    Mesh *Mesh::Create() {
        return new Mesh();
    }

    //--------------------------------------------------------------
    Mesh *Mesh::Create(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes, bool read_and_write_enabled) {
        return new Mesh(mesh_data, sub_meshes, read_and_write_enabled);
    }

    //--------------------------------------------------------------
    void Mesh::OnDestroy() {
        Rendering::RenderEngine::GetRenderDriver()->DestroyMesh(m_resource_id);

        Asset::OnDestroy();
    }

    //--------------------------------------------------------------
    BoundingBox Mesh::CalculateBounds(const Vector<Vec3> &positions) {
        Vec3 min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        Vec3 max = Vec3(FLT_MIN, FLT_MIN, FLT_MIN);

        for (uint32 i = 0; i < positions.size(); i++) {
            Vec3 position = positions[i];
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

        return BoundingBox(min, max);
    }

}