#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {

    class CMesh : public CAsset {
    protected:
        TRef<CVertexArray> m_vertex_array;
    public:
        const TRef<CVertexArray> GetVertexArray() const { return m_vertex_array; }

        static TRef<CMesh> Create(const TRef<CVertexArray> &vertex_array);
        static TRef<CMesh> CreatePlane();
    };

}