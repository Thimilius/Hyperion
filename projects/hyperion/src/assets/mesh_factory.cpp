#include "hyppch.hpp"

#include "hyperion/assets/mesh_factory.hpp"

#include "hyperion/modules/assimp/assimp_mesh_loader.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    IMeshLoader *MeshFactory::s_mesh_loader = new AssimpMeshLoader();

    Rendering::Mesh *MeshFactory::CreateQuad(f32 width, f32 height) {
        f32 half_width = width / 2.0f;
        f32 half_height = height / 2.0f;

        // The quad should face to viewer
        Vec3 normal = Vec3::Back();

        MeshData mesh_data;
        mesh_data.positions.resize(4);
        mesh_data.normals.resize(4);
        mesh_data.uvs.resize(4);
        mesh_data.indices.resize(6);

        mesh_data.positions[0] = Vec3(half_width, half_height, 0);
        mesh_data.normals[0] = normal;
        mesh_data.uvs[0] = Vec2(1.0f, 1.0f);

        mesh_data.positions[1] = Vec3(half_width, -half_height, 0);
        mesh_data.normals[1] = normal;
        mesh_data.uvs[1] = Vec2(1.0f, 0.0f);

        mesh_data.positions[2] = Vec3(-half_width, -half_height, 0);
        mesh_data.normals[2] = normal;
        mesh_data.uvs[2] = Vec2(0.0f, 0.0f);

        mesh_data.normals[3] = normal;
        mesh_data.uvs[3] = Vec2(0.0f, 1.0f);
        mesh_data.positions[3] = Vec3(-half_width, half_height, 0);

        mesh_data.indices = {
            0, 1, 2,
            0, 2, 3
        };

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, 6, 0, 0 } });
    }

    Mesh *MeshFactory::CreatePlane(f32 width, f32 height) {
        f32 half_width = width / 2.0f;
        f32 half_height = height / 2.0f;

        // The plane should face up
        Vec3 normal = Vec3::Up();

        MeshData mesh_data;
        mesh_data.positions.resize(4);
        mesh_data.normals.resize(4);
        mesh_data.uvs.resize(4);
        mesh_data.indices.resize(6);

        // Remember that we are right-handed and therefore -z is into the screen!

        mesh_data.positions[0] = Vec3(-half_width, 0, half_height);
        mesh_data.normals[0] = normal;
        mesh_data.uvs[0] = Vec2(0.0f, 0.0f);

        mesh_data.positions[1] = Vec3(-half_width, 0, -half_height);
        mesh_data.normals[1] = normal;
        mesh_data.uvs[1] = Vec2(0.0f, 1.0f);

        mesh_data.positions[2] = Vec3(half_width, 0, -half_height);
        mesh_data.normals[2] = normal;
        mesh_data.uvs[2] = Vec2(1.0f, 1.0f);

        mesh_data.positions[3] = Vec3(half_width, 0, half_height);
        mesh_data.normals[3] = normal;
        mesh_data.uvs[3] = Vec2(1.0f, 0.0f);

        mesh_data.indices = {
            0, 1, 2,
            0, 2, 3
        };

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, 6, 0, 0 } });
    }

    Mesh *MeshFactory::CreateCube(f32 size) {
        MeshData mesh_data;
        mesh_data.positions.resize(24);
        mesh_data.normals.resize(24);
        mesh_data.uvs.resize(24);
        mesh_data.indices.resize(36);

        f32 half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube (aka right-handed)!
        {
            // Forward (-z)
            mesh_data.positions[0] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[0] = Vec3::Forward();
            mesh_data.uvs[0] = Vec2(0, 0);

            mesh_data.positions[1] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[1] = Vec3::Forward();
            mesh_data.uvs[1] = Vec2(0, 1);

            mesh_data.positions[2] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[2] = Vec3::Forward();
            mesh_data.uvs[2] = Vec2(1, 1);

            mesh_data.positions[3] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[3] = Vec3::Forward();
            mesh_data.uvs[3] = Vec2(1, 0);

            // Right
            mesh_data.positions[4] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[4] = Vec3::Right();
            mesh_data.uvs[4] = Vec2(0, 0);

            mesh_data.positions[5] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[5] = Vec3::Right();
            mesh_data.uvs[5] = Vec2(0, 1);

            mesh_data.positions[6] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[6] = Vec3::Right();
            mesh_data.uvs[6] = Vec2(1, 1);

            mesh_data.positions[7] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[7] = Vec3::Right();
            mesh_data.uvs[7] = Vec2(1, 0);

            // Back (+z)
            mesh_data.positions[8] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[8] = Vec3::Back();
            mesh_data.uvs[8] = Vec2(0, 0);

            mesh_data.positions[9] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[9] = Vec3::Back();
            mesh_data.uvs[9] = Vec2(0, 1);

            mesh_data.positions[10] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[10] = Vec3::Back();
            mesh_data.uvs[10] = Vec2(1, 1);

            mesh_data.positions[11] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[11] = Vec3::Back();
            mesh_data.uvs[11] = Vec2(1, 0);

            // Left
            mesh_data.positions[12] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[12] = Vec3::Left();
            mesh_data.uvs[12] = Vec2(0, 0);

            mesh_data.positions[13] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[13] = Vec3::Left();
            mesh_data.uvs[13] = Vec2(0, 1);

            mesh_data.positions[14] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[14] = Vec3::Left();
            mesh_data.uvs[14] = Vec2(1, 1);

            mesh_data.positions[15] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[15] = Vec3::Left();
            mesh_data.uvs[15] = Vec2(1, 0);

            // Up
            mesh_data.positions[16] = Vec3(-half_size, half_size, half_size);
            mesh_data.normals[16] = Vec3::Up();
            mesh_data.uvs[16] = Vec2(0, 0);

            mesh_data.positions[17] = Vec3(-half_size, half_size, -half_size);
            mesh_data.normals[17] = Vec3::Up();
            mesh_data.uvs[17] = Vec2(0, 1);

            mesh_data.positions[18] = Vec3(half_size, half_size, -half_size);
            mesh_data.normals[18] = Vec3::Up();
            mesh_data.uvs[18] = Vec2(1, 1);

            mesh_data.positions[19] = Vec3(half_size, half_size, half_size);
            mesh_data.normals[19] = Vec3::Up();
            mesh_data.uvs[19] = Vec2(1, 0);

            // Down
            mesh_data.positions[20] = Vec3(-half_size, -half_size, -half_size);
            mesh_data.normals[20] = Vec3::Down();
            mesh_data.uvs[20] = Vec2(0, 0);

            mesh_data.positions[21] = Vec3(-half_size, -half_size, half_size);
            mesh_data.normals[21] = Vec3::Down();
            mesh_data.uvs[21] = Vec2(0, 1);

            mesh_data.positions[22] = Vec3(half_size, -half_size, half_size);
            mesh_data.normals[22] = Vec3::Down();
            mesh_data.uvs[22] = Vec2(1, 1);

            mesh_data.positions[23] = Vec3(half_size, -half_size, -half_size);
            mesh_data.normals[23] = Vec3::Down();
            mesh_data.uvs[23] = Vec2(1, 0);
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

    Rendering::Mesh *MeshFactory::CreateSphere(f32 radius) {
        MeshData mesh_data;

        const u32 SECTOR_COUNT = 36;
        const u32 STACK_COUNT = 18;
        const f32 SECTOR_STEP = 2 * Math::PI / SECTOR_COUNT;
        const f32 STACK_STEP = Math::PI / STACK_COUNT;

        f32 length_inverse = 1.0f / radius;

        for (u32 i = 0; i <= STACK_COUNT; ++i) {
            f32 stack_angle = Math::PI / 2 - i * STACK_STEP;
            f32 xy = radius * Math::Cos(stack_angle);
            f32 y = radius * Math::Sin(stack_angle);

            for (int j = 0; j <= SECTOR_COUNT; ++j) {
                f32 sector_angle = j * SECTOR_STEP;

                f32 x = xy * Math::Cos(sector_angle);
                f32 z = xy * Math::Sin(sector_angle);
                mesh_data.positions.push_back(Vec3(x, y, z));

                f32 nx = x * length_inverse;
                f32 ny = y * length_inverse;
                f32 nz = z * length_inverse;
                mesh_data.normals.push_back(Vec3(nx, ny, nz));

                f32 u = static_cast<f32>(j) / SECTOR_COUNT;
                f32 v = static_cast<f32>(i) / STACK_COUNT;
                mesh_data.uvs.push_back(Vec2(u, v));
            }
        }

        for (u32 i = 0; i < STACK_COUNT; ++i) {
            u32 k1 = i * (SECTOR_COUNT + 1);
            u32 k2 = k1 + SECTOR_COUNT + 1;

            for (u32 j = 0; j < SECTOR_COUNT; ++j, ++k1, ++k2) {
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

        return Mesh::Create(mesh_data, { { MeshTopology::Triangles, static_cast<u32>(mesh_data.indices.size()), 0, 0 } });
    }

    Mesh *MeshFactory::CreateFromFile(const String &path) {
        return s_mesh_loader->LoadMesh(path);
    }

}