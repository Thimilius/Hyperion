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

  enum class MeshTopology {
    Points,
    Lines,
    LineStrip,
    Triangles
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

  struct SubMesh {
    MeshTopology topology;

    uint32 vertex_count;
    uint32 vertex_offset;
    uint32 index_count;
    uint32 index_offset;
  };

  using SubMeshes = Array<SubMesh>;

  struct MeshData {
    Array<Vector3> positions;
    Array<Vector3> normals;
    Array<Vector4> colors;
    Array<Vector2> texture0;

    Array<uint32> indices;
  };

  struct MeshVertexFormat {
    uint32 stride;
    Array<VertexAttribute> attributes;

    inline static uint32 VERTEX_ATTRIBUTE_SIZE_POSITION = sizeof(float32) * 3;
    inline static uint32 VERTEX_ATTRIBUTE_SIZE_NORMAL = sizeof(float32) * 3;
    inline static uint32 VERTEX_ATTRIBUTE_SIZE_COLOR = sizeof(float32) * 4;
    inline static uint32 VERTEX_ATTRIBUTE_SIZE_TEXTURE0 = sizeof(float32) * 2;
  };

}