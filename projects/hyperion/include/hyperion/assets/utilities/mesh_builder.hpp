//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class MeshBuilder final {
    public:
        inline bool8 IsEmpty() const { return m_vertex_count == 0 || m_index_count == 0; }
        inline uint32 GetVertextCount() const { return m_vertex_count; }
        inline uint32 GetIndexCount() const { return m_index_count; }

        void AddVertex(Vector3 position, Vector3 normal, Vector2 texture0);
        void AddVertex(Vector3 position, Color color, Vector2 texture0);
        inline void AddIndexOffset(uint32 offset) { m_index_offset += offset; }
        void AddTriangle(uint32 a, uint32 b, uint32 c);

        void Transform(const Matrix4x4 &transformation);

        Mesh *CreateMesh();
        void SetToMesh(Mesh *mesh);

        void Clear();
    private:
        Rendering::MeshData m_mesh_data;
        uint32 m_vertex_count = 0;
        uint32 m_index_count = 0;
        uint32 m_index_offset = 0;
    };

}