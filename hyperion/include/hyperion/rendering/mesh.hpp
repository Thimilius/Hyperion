#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/math/math.hpp"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace Hyperion::Rendering {

    struct SVertexPNU {
        Math::SVec3 position;
        Math::SVec3 normal;
        Math::SVec2 uv;
    };

    class CMesh : public CAsset {
    private:
        
    struct SSubMesh {
        u32 index_offset;
        u32 index_count;
    };

    protected:
        TRef<CVertexArray> m_vertex_array;
        TVector<SSubMesh> m_sub_meshes;
    public:
        const TRef<CVertexArray> GetVertexArray() const { return m_vertex_array; }
        const TVector<SSubMesh> GetSubMeshes() const { return m_sub_meshes; }

        static TRef<CMesh> Create(const TRef<CVertexArray> &vertex_array);
        static TRef<CMesh> Create(const TRef<CVertexArray> &vertex_array, const TVector<SSubMesh> sub_meshes);
        static TRef<CMesh> CreatePlane(float width, float height);
        static TRef<CMesh> CreatePlane(Math::SVec2 size);
        static TRef<CMesh> CreateCube(float size);
        static TRef<CMesh> CreateFromFile(const TString &path);
    private:
        CMesh(const TRef<CVertexArray> &vertex_array, const TVector<SSubMesh> sub_meshes);

        static TRef<CMesh> LoadMesh(const TString &path);
        static void LoadSubMesh(const aiMesh *mesh, TVector<SVertexPNU> &verticies, TVector<u32> &indicies, TVector<SSubMesh> &sub_meshes);
    };

}