#include "hyppch.hpp"

#include "hyperion/assets/mesh.hpp"

#include "hyperion/modules/assimp/assimp_mesh_loader.hpp"
#include "hyperion/rendering/render_driver.hpp"
#include "hyperion/rendering/render_engine.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Mesh::Mesh(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes, bool read_and_write_enabled) {
        HYP_ASSERT_MESSAGE(sub_meshes.size() > 0, "Must provide at least one submesh when creating a mesh!");

        m_read_and_write_enabled = read_and_write_enabled;
        m_bounds = CalculateBounds(mesh_data.positions);

        if (read_and_write_enabled) {
            m_mesh_data = mesh_data;
            m_sub_meshes = sub_meshes;
        }

        // NOTE: Currently we don't really do a lot of data validation.

        MeshDescriptor descriptor = { };
        descriptor.sub_meshes = sub_meshes;

        bool has_positions = mesh_data.positions.size() > 0;
        bool has_normals = mesh_data.normals.size() > 0;
        bool has_texture0 = mesh_data.texture0.size() > 0;

        uint32 vertex_count = static_cast<uint32>(mesh_data.positions.size());
        Vector<uint8> vertices(vertex_count * sizeof(VertexMesh));
        VertexMesh *vertex_data = reinterpret_cast<VertexMesh *>(vertices.data());
        for (uint32 i = 0; i < vertex_count; i++) {
            vertex_data[i].position = mesh_data.positions[i];
            vertex_data[i].normal = mesh_data.normals[i];
            vertex_data[i].texture0 = mesh_data.texture0[i];
        }
        descriptor.vertices = vertices;
        Vector<VertexAttribute> vertex_attributes = {
            { VertexAttributeKind::Position, VertexAttributeType::Float32, 3 },
            { VertexAttributeKind::Normal, VertexAttributeType::Float32, 3 },
            { VertexAttributeKind::Texture0, VertexAttributeType::Float32, 2 },
        };
        descriptor.vertex_format.attributes = vertex_attributes;
        descriptor.vertex_format.stride = 32;

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

    const MeshData &Mesh::GetMeshData() const {
        if (!m_read_and_write_enabled) {
            HYP_LOG_WARN("Mesh", "The data of a mesh that is not read and write enabled will not exist when queried!");
        }
        return m_mesh_data;
    }

    const Vector<Rendering::SubMesh> &Mesh::GetSubMeshes() const {
        if (!m_read_and_write_enabled) {
            HYP_LOG_WARN("Mesh", "The data of a mesh that is not read and write enabled will not exist when queried!");
        }
        return m_sub_meshes;
    }

    Mesh *Mesh::Create() {
        return new Mesh();
    }

    Mesh *Mesh::Create(const MeshData &mesh_data, const Vector<SubMesh> &sub_meshes, bool read_and_write_enabled) {
        return new Mesh(mesh_data, sub_meshes, read_and_write_enabled);
    }

    void Mesh::OnDestroy() {
        Rendering::RenderEngine::GetRenderDriver()->DestroyMesh(m_resource_id);
    }

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

    Mesh *MeshFactory::CreateQuad(float32 width, float32 height) {
        float32 half_width = width / 2.0f;
        float32 half_height = height / 2.0f;

        // The quad should face the camera (right-haneded)
        Vec3 normal = Vec3::Back();

        MeshData mesh_data;
        mesh_data.positions.resize(4);
        mesh_data.normals.resize(4);
        mesh_data.texture0.resize(4);
        mesh_data.indices.resize(6);

        mesh_data.positions[0] = Vec3(half_width, half_height, 0);
        mesh_data.normals[0] = normal;
        mesh_data.texture0[0] = Vec2(1.0f, 1.0f);

        mesh_data.positions[1] = Vec3(half_width, -half_height, 0);
        mesh_data.normals[1] = normal;
        mesh_data.texture0[1] = Vec2(1.0f, 0.0f);

        mesh_data.positions[2] = Vec3(-half_width, -half_height, 0);
        mesh_data.normals[2] = normal;
        mesh_data.texture0[2] = Vec2(0.0f, 0.0f);

        mesh_data.positions[3] = Vec3(-half_width, half_height, 0);
        mesh_data.normals[3] = normal;
        mesh_data.texture0[3] = Vec2(0.0f, 1.0f);

        mesh_data.indices = {
            0, 1, 2,
            0, 2, 3
        };

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, 6, 0, 0 } });
    }

    Mesh *MeshFactory::CreatePlane(float32 width, float32 height) {
        float32 half_width = width / 2.0f;
        float32 half_height = height / 2.0f;

        // The plane should face up
        Vec3 normal = Vec3::Up();

        MeshData mesh_data;
        mesh_data.positions.resize(4);
        mesh_data.normals.resize(4);
        mesh_data.texture0.resize(4);
        mesh_data.indices.resize(6);

        // Remember that we are right-handed and therefore -z is into the screen!

        mesh_data.positions[0] = Vec3(-half_width, 0, half_height);
        mesh_data.normals[0] = normal;
        mesh_data.texture0[0] = Vec2(0.0f, 0.0f);

        mesh_data.positions[1] = Vec3(-half_width, 0, -half_height);
        mesh_data.normals[1] = normal;
        mesh_data.texture0[1] = Vec2(0.0f, 1.0f);

        mesh_data.positions[2] = Vec3(half_width, 0, -half_height);
        mesh_data.normals[2] = normal;
        mesh_data.texture0[2] = Vec2(1.0f, 1.0f);

        mesh_data.positions[3] = Vec3(half_width, 0, half_height);
        mesh_data.normals[3] = normal;
        mesh_data.texture0[3] = Vec2(1.0f, 0.0f);

        mesh_data.indices = {
            0, 1, 2,
            0, 2, 3
        };

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, 6, 0, 0 } });
    }

    Mesh *MeshFactory::CreateCube(float32 size) {
        MeshData mesh_data;
        mesh_data.positions.resize(24);
        mesh_data.normals.resize(24);
        mesh_data.texture0.resize(24);
        mesh_data.indices.resize(36);

        float32 half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube (aka right-handed)!
        {
            // Forward (-z)
            mesh_data.positions[0] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[0] = Vec3::Forward();
            mesh_data.texture0[0] = Vec2(0, 0);

            mesh_data.positions[1] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[1] = Vec3::Forward();
            mesh_data.texture0[1] = Vec2(0, 1);

            mesh_data.positions[2] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[2] = Vec3::Forward();
            mesh_data.texture0[2] = Vec2(1, 1);

            mesh_data.positions[3] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[3] = Vec3::Forward();
            mesh_data.texture0[3] = Vec2(1, 0);

            // Right
            mesh_data.positions[4] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[4] = Vec3::Right();
            mesh_data.texture0[4] = Vec2(0, 0);

            mesh_data.positions[5] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[5] = Vec3::Right();
            mesh_data.texture0[5] = Vec2(0, 1);

            mesh_data.positions[6] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[6] = Vec3::Right();
            mesh_data.texture0[6] = Vec2(1, 1);

            mesh_data.positions[7] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[7] = Vec3::Right();
            mesh_data.texture0[7] = Vec2(1, 0);

            // Back (+z)
            mesh_data.positions[8] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[8] = Vec3::Back();
            mesh_data.texture0[8] = Vec2(0, 0);

            mesh_data.positions[9] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[9] = Vec3::Back();
            mesh_data.texture0[9] = Vec2(0, 1);

            mesh_data.positions[10] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[10] = Vec3::Back();
            mesh_data.texture0[10] = Vec2(1, 1);

            mesh_data.positions[11] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[11] = Vec3::Back();
            mesh_data.texture0[11] = Vec2(1, 0);

            // Left
            mesh_data.positions[12] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[12] = Vec3::Left();
            mesh_data.texture0[12] = Vec2(0, 0);

            mesh_data.positions[13] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[13] = Vec3::Left();
            mesh_data.texture0[13] = Vec2(0, 1);

            mesh_data.positions[14] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[14] = Vec3::Left();
            mesh_data.texture0[14] = Vec2(1, 1);

            mesh_data.positions[15] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[15] = Vec3::Left();
            mesh_data.texture0[15] = Vec2(1, 0);

            // Up
            mesh_data.positions[16] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[16] = Vec3::Up();
            mesh_data.texture0[16] = Vec2(0, 0);

            mesh_data.positions[17] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[17] = Vec3::Up();
            mesh_data.texture0[17] = Vec2(0, 1);

            mesh_data.positions[18] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[18] = Vec3::Up();
            mesh_data.texture0[18] = Vec2(1, 1);

            mesh_data.positions[19] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[19] = Vec3::Up();
            mesh_data.texture0[19] = Vec2(1, 0);

            // Down
            mesh_data.positions[20] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[20] = Vec3::Down();
            mesh_data.texture0[20] = Vec2(0, 0);

            mesh_data.positions[21] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[21] = Vec3::Down();
            mesh_data.texture0[21] = Vec2(0, 1);

            mesh_data.positions[22] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[22] = Vec3::Down();
            mesh_data.texture0[22] = Vec2(1, 1);

            mesh_data.positions[23] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[23] = Vec3::Down();
            mesh_data.texture0[23] = Vec2(1, 0);
        }

        mesh_data.indices = {
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

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, 36, 0, 0 } });
    }

    Mesh *MeshFactory::CreateSphere(float32 radius) {
        MeshData mesh_data;

        const uint32 SECTOR_COUNT = 36;
        const uint32 STACK_COUNT = 18;
        const float32 SECTOR_STEP = 2 * Math::PI / SECTOR_COUNT;
        const float32 STACK_STEP = Math::PI / STACK_COUNT;

        float32 length_inverse = 1.0f / radius;

        for (uint32 i = 0; i <= STACK_COUNT; ++i) {
            float32 stack_angle = Math::PI / 2 - i * STACK_STEP;
            float32 xy = radius * Math::Cos(stack_angle);
            float32 y = radius * Math::Sin(stack_angle);

            for (int j = 0; j <= SECTOR_COUNT; ++j) {
                float32 sector_angle = j * SECTOR_STEP;

                float32 x = xy * Math::Cos(sector_angle);
                float32 z = xy * Math::Sin(sector_angle);
                mesh_data.positions.push_back(Vec3(x, y, z));

                float32 nx = x * length_inverse;
                float32 ny = y * length_inverse;
                float32 nz = z * length_inverse;
                mesh_data.normals.push_back(Vec3(nx, ny, nz));

                float32 u = static_cast<float32>(j) / SECTOR_COUNT;
                float32 v = static_cast<float32>(i) / STACK_COUNT;
                mesh_data.texture0.push_back(Vec2(u, v));
            }
        }

        for (uint32 i = 0; i < STACK_COUNT; ++i) {
            uint32 k1 = i * (SECTOR_COUNT + 1);
            uint32 k2 = k1 + SECTOR_COUNT + 1;

            for (uint32 j = 0; j < SECTOR_COUNT; ++j, ++k1, ++k2) {
                if (i != 0) {
                    mesh_data.indices.push_back(k1);
                    mesh_data.indices.push_back(k2);
                    mesh_data.indices.push_back(k1 + 1);
                }

                if (i != (STACK_COUNT - 1)) {
                    mesh_data.indices.push_back(k1 + 1);
                    mesh_data.indices.push_back(k2);
                    mesh_data.indices.push_back(k2 + 1);
                }
            }
        }

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, static_cast<uint32>(mesh_data.indices.size()), 0, 0 } });
    }

    Mesh *MeshFactory::CreateFromFile(const String &path) {
        return s_mesh_loader->LoadMesh(path);
    }

    void MeshFactory::Init() {
        s_mesh_loader = new AssimpMeshLoader();
    }

    void MeshFactory::Shutdown() {
        delete s_mesh_loader;
    }

}
