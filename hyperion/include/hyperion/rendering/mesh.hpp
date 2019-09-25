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

        static CBufferLayout GetBufferLayout() {
            return CBufferLayout({
                SBufferElement("a_position", EShaderDataType::Float3),
                SBufferElement("a_normal", EShaderDataType::Float3),
                SBufferElement("a_uv", EShaderDataType::Float2)
            });
        }
    };

    struct SSubMesh {
        u32 index_count;
        u32 index_offset;
        u32 vertex_offset;

        SSubMesh(u32 index_count, u32 index_offset, u32 vertex_offset) : index_count(index_count), index_offset(index_offset), vertex_offset(vertex_offset) { }
    };

    class CMesh : public CAsset {
    private:
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