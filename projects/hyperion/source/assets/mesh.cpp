//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/mesh.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/assimp/assimp_mesh_loader.hpp"
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

    //--------------------------------------------------------------
    void MeshBuilder::Clear() {
        m_mesh_data.positions.clear();
        m_mesh_data.normals.clear();
        m_mesh_data.colors.clear();
        m_mesh_data.texture0.clear();
        m_mesh_data.indices.clear();

        m_index_count = 0;
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddVertex(Vec3 position, Vec3 normal, Vec2 texture0) {
        m_mesh_data.positions.push_back(position);
        m_mesh_data.normals.push_back(normal);
        m_mesh_data.texture0.push_back(texture0);
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddVertex(Vec3 position, Color color, Vec2 texture0) {
        m_mesh_data.positions.push_back(position);
        m_mesh_data.colors.push_back(color);
        m_mesh_data.texture0.push_back(texture0);
    }

    //--------------------------------------------------------------
    void MeshBuilder::AddTriangle(uint32 a, uint32 b, uint32 c) {
        m_mesh_data.indices.push_back(a);
        m_mesh_data.indices.push_back(b);
        m_mesh_data.indices.push_back(c);
        m_index_count += 3;
    }

    //--------------------------------------------------------------
    Mesh *MeshBuilder::CreateMesh() {
        Vector<SubMesh> sub_meshes = { { MeshTopology::Triangles, m_index_count, 0, 0 } };
        return Mesh::Create(m_mesh_data, sub_meshes);
    }

    //--------------------------------------------------------------
    void MeshBuilder::SetToMesh(Mesh *mesh) {
        Vector<SubMesh> sub_meshes = { { MeshTopology::Triangles, m_index_count, 0, 0 } };
        mesh->SetData(m_mesh_data, sub_meshes);
    }

    // TODO: Use mesh build in mesh factory methods.

    //--------------------------------------------------------------
    Mesh *MeshFactory::CreateQuad(float32 width, float32 height) {
        float32 half_width = width / 2.0f;
        float32 half_height = height / 2.0f;

        // The quad should face the camera (right-handed).
        Vec3 normal = Vec3::Back();

        MeshBuilder mesh_builder;

        mesh_builder.AddVertex(Vec3( half_width,  half_height, 0.0f), normal, Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_width, -half_height, 0.0f), normal, Vec2(1.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, -half_height, 0.0f), normal, Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width,  half_height, 0.0f), normal, Vec2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshFactory::CreatePlane(float32 width, float32 height) {
        float32 half_width = width / 2.0f;
        float32 half_height = height / 2.0f;

        // The plane should face up.
        Vec3 normal = Vec3::Up();

        MeshBuilder mesh_builder;

        // Remember that we are right-handed and therefore -z is into the screen!
        mesh_builder.AddVertex(Vec3( half_width, 0.0f, -half_height), normal, Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_width, 0.0f,  half_height), normal, Vec2(1.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, 0.0f,  half_height), normal, Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, 0.0f, -half_height), normal, Vec2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshFactory::CreateCube(float32 size) {
        MeshBuilder mesh_builder;
        
        float32 half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube (aka right-handed)!

        // Forward (-z)
        mesh_builder.AddVertex(Vec3( half_size, -half_size, -half_size), Vec3::Forward(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3( half_size,  half_size, -half_size), Vec3::Forward(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size,  half_size, -half_size), Vec3::Forward(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, -half_size), Vec3::Forward(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);
        // Right (+x)
        mesh_builder.AddVertex(Vec3(half_size, -half_size,  half_size), Vec3::Right(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(half_size,  half_size,  half_size), Vec3::Right(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size,  half_size, -half_size), Vec3::Right(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, -half_size, -half_size), Vec3::Right(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(4, 5, 6);
        mesh_builder.AddTriangle(4, 6, 7);
        // Back (+z)
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, half_size), Vec3::Back(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size,  half_size, half_size), Vec3::Back(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_size,  half_size, half_size), Vec3::Back(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_size, -half_size, half_size), Vec3::Back(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(8, 9, 10);
        mesh_builder.AddTriangle(8, 10, 11);
        // Left (-x)
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, -half_size), Vec3::Left(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size,  half_size, -half_size), Vec3::Left(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size,  half_size,  half_size), Vec3::Left(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size, -half_size,  half_size), Vec3::Left(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(12, 13, 14);
        mesh_builder.AddTriangle(12, 14, 15);
        // Up (+y)
        mesh_builder.AddVertex(Vec3(-half_size, half_size,  half_size), Vec3::Up(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size, half_size, -half_size), Vec3::Up(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_size, half_size, -half_size), Vec3::Up(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_size, half_size,  half_size), Vec3::Up(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(16, 17, 18);
        mesh_builder.AddTriangle(16, 18, 19);
        // Down (-y)
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, -half_size), Vec3::Down(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size, -half_size,  half_size), Vec3::Down(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_size, -half_size,  half_size), Vec3::Down(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3( half_size, -half_size, -half_size), Vec3::Down(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(20, 21, 22);
        mesh_builder.AddTriangle(20, 22, 23);

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshFactory::CreateSphere(float32 radius, uint32 sector_count, uint32 stack_count) {
        MeshBuilder mesh_builder;

        float32 sector_step = 2 * Math::PI / sector_count;
        float32 stack_step = Math::PI / stack_count;
        float32 length_inverse = 1.0f / radius;

        for (uint32 i = 0; i <= stack_count; ++i) {
            float32 stack_angle = Math::PI / 2 - i * stack_step;
            float32 xy = radius * Math::Cos(stack_angle);
            float32 y = radius * Math::Sin(stack_angle);

            for (uint32 j = 0; j <= sector_count; ++j) {
                float32 sector_angle = j * sector_step;

                float32 x = xy * Math::Cos(sector_angle);
                float32 z = xy * Math::Sin(sector_angle);
                float32 nx = x * length_inverse;
                float32 ny = y * length_inverse;
                float32 nz = z * length_inverse;
                float32 u = 1.0f - static_cast<float32>(j) / sector_count;
                float32 v = 1.0f - static_cast<float32>(i) / stack_count;

                mesh_builder.AddVertex(Vec3(x, y, z), Vec3(nx, ny, nz), Vec2(u, v));
            }
        }

        for (uint32 i = 0; i < stack_count; ++i) {
            uint32 k1 = i * (sector_count + 1);
            uint32 k2 = k1 + sector_count + 1;

            for (uint32 j = 0; j < sector_count; ++j, ++k1, ++k2) {
                if (i != 0) {
                    mesh_builder.AddTriangle(k1, k2, k1 + 1);
                }

                if (i != (stack_count - 1)) {
                    mesh_builder.AddTriangle(k1 + 1, k2, k2 + 1);
                }
            }
        }

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshFactory::CreateFromFile(const String &path) {
        return s_mesh_loader->LoadMesh(path);
    }

    //--------------------------------------------------------------
    void MeshFactory::Initialize() {
        s_mesh_loader = new AssimpMeshLoader();
    }

    //--------------------------------------------------------------
    void MeshFactory::Shutdown() {
        delete s_mesh_loader;
    }

}
