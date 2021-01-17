#pragma once

#include "hyperion/core/types.hpp"

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

    struct VertexAttributeDescriptor {
        VertexAttribute attribute;
        VertexAttributeFormat format;
        uint32 dimension;
    };

    struct VertexFormat {
        Vector<VertexAttributeDescriptor> vertex_attributes;
        uint32 stride;
    };

    struct SubMesh {
        MeshTopology topology;

        uint32 index_count;
        uint32 index_offset;
        uint32 vertex_offset;
    };

    struct MeshDescriptor {
        Vector<SubMesh> sub_meshes;

        VertexFormat vertex_format;
        IndexFormat index_format;

        Vector<uint8> vertex_data;
        Vector<uint8> index_data;
    };

}