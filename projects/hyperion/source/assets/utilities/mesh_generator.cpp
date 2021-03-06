//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/utilities/mesh_generator.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/mesh_builder.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Mesh *MeshGenerator::GenerateQuad(float32 width, float32 height) {
        float32 half_width = width / 2.0f;
        float32 half_height = height / 2.0f;

        // The quad should face the camera (right-handed).
        Vec3 normal = Vec3::Back();

        MeshBuilder mesh_builder;

        mesh_builder.AddVertex(Vec3(half_width, half_height, 0.0f), normal, Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_width, -half_height, 0.0f), normal, Vec2(1.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, -half_height, 0.0f), normal, Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, half_height, 0.0f), normal, Vec2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshGenerator::GeneratePlane(float32 width, float32 height) {
        float32 half_width = width / 2.0f;
        float32 half_height = height / 2.0f;

        // The plane should face up.
        Vec3 normal = Vec3::Up();

        MeshBuilder mesh_builder;

        // Remember that we are right-handed and therefore -z is into the screen!
        mesh_builder.AddVertex(Vec3(half_width, 0.0f, -half_height), normal, Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_width, 0.0f, half_height), normal, Vec2(1.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, 0.0f, half_height), normal, Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_width, 0.0f, -half_height), normal, Vec2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshGenerator::GenerateCube(float32 size) {
        MeshBuilder mesh_builder;

        float32 half_size = size / 2.0f;

        // Forward and back as seen from the center of the cube (aka right-handed)!

        // Forward (-z)
        mesh_builder.AddVertex(Vec3(half_size, -half_size, -half_size), Vec3::Forward(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(half_size, half_size, -half_size), Vec3::Forward(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size, half_size, -half_size), Vec3::Forward(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, -half_size), Vec3::Forward(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);
        // Right (+x)
        mesh_builder.AddVertex(Vec3(half_size, -half_size, half_size), Vec3::Right(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(half_size, half_size, half_size), Vec3::Right(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, half_size, -half_size), Vec3::Right(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, -half_size, -half_size), Vec3::Right(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(4, 5, 6);
        mesh_builder.AddTriangle(4, 6, 7);
        // Back (+z)
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, half_size), Vec3::Back(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size, half_size, half_size), Vec3::Back(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, half_size, half_size), Vec3::Back(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, -half_size, half_size), Vec3::Back(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(8, 9, 10);
        mesh_builder.AddTriangle(8, 10, 11);
        // Left (-x)
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, -half_size), Vec3::Left(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size, half_size, -half_size), Vec3::Left(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size, half_size, half_size), Vec3::Left(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, half_size), Vec3::Left(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(12, 13, 14);
        mesh_builder.AddTriangle(12, 14, 15);
        // Up (+y)
        mesh_builder.AddVertex(Vec3(-half_size, half_size, half_size), Vec3::Up(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size, half_size, -half_size), Vec3::Up(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, half_size, -half_size), Vec3::Up(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, half_size, half_size), Vec3::Up(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(16, 17, 18);
        mesh_builder.AddTriangle(16, 18, 19);
        // Down (-y)
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, -half_size), Vec3::Down(), Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(Vec3(-half_size, -half_size, half_size), Vec3::Down(), Vec2(0.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, -half_size, half_size), Vec3::Down(), Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(Vec3(half_size, -half_size, -half_size), Vec3::Down(), Vec2(1.0f, 0.0f));
        mesh_builder.AddTriangle(20, 21, 22);
        mesh_builder.AddTriangle(20, 22, 23);

        return mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    Mesh *MeshGenerator::GenerateSphere(float32 radius, uint32 sector_count, uint32 stack_count) {
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
    
}