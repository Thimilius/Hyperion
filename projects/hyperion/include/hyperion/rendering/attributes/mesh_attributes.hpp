#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Rendering {

    enum class MeshTopology {
        Points,
        Lines,
        LineStrip,
        Triangles
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

    struct SubMesh {
        MeshTopology topology;

        uint32 index_count;
        uint32 index_offset;
        uint32 vertex_offset;
    };

}