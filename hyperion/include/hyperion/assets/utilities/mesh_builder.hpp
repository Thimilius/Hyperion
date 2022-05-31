//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/mesh.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class MeshBuilder final {
  public:
    inline bool8 IsEmpty() const { return m_vertex_count == 0 || m_index_count == 0; }
    inline uint32 GetVertexCount() const { return m_vertex_count; }
    inline uint32 GetIndexCount() const { return m_index_count; }
    inline uint32 GetIndexOffset() const { return m_index_offset; }
    inline void AddIndexOffset(uint32 offset) { m_index_offset += offset; }

    void AddVertex(Vector3 position, Vector3 normal, Vector2 texture0);
    void AddVertex(Vector3 position, Color color, Vector2 texture0);
    void AddTriangle(uint32 a, uint32 b, uint32 c);

    void AddQuad(Vector3 corners[4], Vector3 normal);
    void AddQuad(Vector3 corners[4], Vector3 normal, Vector2 uvs[4]);
    void AddQuad(Vector3 corners[4], Color color);
    void AddQuad(Vector3 corners[4], Color color, Vector2 uvs[4]);

    void Transform(const Matrix4x4 &transformation);
    void TransformAndAlignPixels(const Matrix4x4 &transformation, Vector2Int alignment_size);

    Mesh *CreateMesh(AssetDataAccess data_access = AssetDataAccess::None);
    void SetToMesh(Mesh *mesh);

    void Clear();
  private:
    Rendering::MeshData m_mesh_data;
    uint32 m_vertex_count = 0;
    uint32 m_index_count = 0;
    uint32 m_index_offset = 0;
  };

}
