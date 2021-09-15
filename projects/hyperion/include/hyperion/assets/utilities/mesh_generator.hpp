#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class MeshGenerator final {
    public:
        static Mesh *GenerateQuad(float32 width, float32 height);
        static Mesh *GeneratePlane(float32 width, float32 height);
        static Mesh *GenerateCube(float32 size);
        static Mesh *GenerateSphere(float32 radius, uint32 sector_count = 36, uint32 stack_count = 18);
    private:
        MeshGenerator() = delete;
        ~MeshGenerator() = delete;
    };

}