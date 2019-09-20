#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/math/math.hpp"

namespace Hyperion::Rendering {

    struct SVertexPNUV {
        Math::SVec3 position;
        Math::SVec3 normal;
        Math::SVec2 uv;
    };

    class CMesh : public CAsset {
    protected:
        TRef<CVertexArray> m_vertex_array;
    public:
        const TRef<CVertexArray> GetVertexArray() const { return m_vertex_array; }

        static TRef<CMesh> Create(const TRef<CVertexArray> &vertex_array);
        static TRef<CMesh> CreatePlane(float width, float height);
        static TRef<CMesh> CreatePlane(Math::SVec2 size);
    };

}