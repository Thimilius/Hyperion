#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class MeshBuilder final {
    public:
        void Clear();

        void AddVertex(Vector3 position, Vector3 normal, Vector2 texture0);
        void AddVertex(Vector3 position, Color color, Vector2 texture0);
        void AddTriangle(uint32 a, uint32 b, uint32 c);

        Mesh *CreateMesh();
        void SetToMesh(Mesh *mesh);
    private:
        Rendering::MeshData m_mesh_data;
        uint32 m_vertex_count = 0;
        uint32 m_index_count = 0;
    };

}