//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  enum class MeshIndexFormat {
    UInt16,
    UInt32
  };

  enum class MeshUpdateType {
    Static,
    Dynamic
  };
  
  enum class MeshTopology {
    Points,
    Lines,
    LineStrip,
    Triangles
  };

  struct SubMesh {
    MeshTopology topology = MeshTopology::Triangles;

    uint32 vertex_count = 0;
    uint32 vertex_offset = 0;
    uint32 index_count = 0;
    uint32 index_offset = 0;
  };

  using SubMeshes = Array<SubMesh>;

  struct MeshData {
    Array<Vector3> positions;
    Array<Vector3> normals;
    Array<Vector4> colors;
    Array<Vector2> texture0;

    // NOTE: This is quite wasteful as every mesh as INT indices currently. We should have support for SHORT as well.
    Array<uint32> indices;
  };

  enum class VertexAttributeKind {
    Position,
    Normal,
    Tangent,
    Color,
    Texture0,
    Texture1,
    Texture2,
    Texture3,
  };

  enum class VertexAttributeType {
    Float32,
  };

  struct VertexAttribute {
    VertexAttributeKind kind;
    VertexAttributeType type;
    uint32 dimension;
  };

  using VertexAttributes = Array<VertexAttribute>;
  
  struct MeshVertexFormat {
    uint32 stride = 0;
    VertexAttributes attributes;

    inline static constexpr uint32 VERTEX_ATTRIBUTE_SIZE_POSITION = sizeof(float32) * 3;
    inline static constexpr uint32 VERTEX_ATTRIBUTE_SIZE_NORMAL = sizeof(float32) * 3;
    inline static constexpr uint32 VERTEX_ATTRIBUTE_SIZE_COLOR = sizeof(float32) * 4;
    inline static constexpr uint32 VERTEX_ATTRIBUTE_SIZE_TEXTURE0 = sizeof(float32) * 2;
  };

}
