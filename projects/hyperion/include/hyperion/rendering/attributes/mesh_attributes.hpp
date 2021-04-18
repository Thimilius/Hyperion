#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

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

    enum class IndexFormat {
        UInt16,
        UInt32
    };

    enum class MeshTopology {
        Points,
        Lines,
        LineStrip,
        Triangles
    };

    struct SubMesh {
        MeshTopology topology;

        uint32 index_count;
        uint32 index_offset;
        uint32 vertex_offset;
    };

}