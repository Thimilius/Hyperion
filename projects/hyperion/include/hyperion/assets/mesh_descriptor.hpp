#pragma once

#include "hyperion/core/types.hpp"

namespace Hyperion {

    enum class MeshTopology {
        Triangles,
        Lines,
        LineStrip,
        Points
    };

    enum class VertexAttribute {
        Position,
        Normal,
        Tangent,
        Color,
        Texture0,
        Texture1,
        Texture2,
        Texture3,
    };

    enum class VertexAttributeFormat {
        Float32,
    };

    enum class IndexFormat {
        UInt16,
        UInt32
    };

    struct VertexAttributeDescriptor {
        VertexAttribute attribute;
        VertexAttributeFormat format;
        uint32 dimension;
    };

    struct SubMesh {
        MeshTopology topology;

        uint32 index_count;
        uint32 index_offset;
        uint32 vertex_offset;
    };

}