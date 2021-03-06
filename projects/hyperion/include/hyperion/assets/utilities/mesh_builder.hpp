#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class MeshBuilder final {
    public:
        void Clear();

        void AddVertex(Vec3 position, Vec3 normal, Vec2 texture0);
        void AddVertex(Vec3 position, Color color, Vec2 texture0);
        void AddTriangle(uint32 a, uint32 b, uint32 c);

        Mesh *CreateMesh();
        void SetToMesh(Mesh *mesh);
    private:
        MeshData m_mesh_data;
        uint32 m_index_count = 0;
    };

}