#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/math/math.hpp"

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
    
    struct SMeshData {
        TVector<Math::SVec3> positions;
        TVector<Math::SVec3> normals;
        TVector<Math::SVec2> uvs;

        TVector<u32> indicies;
    };

    struct SSubMesh {
        u32 index_count;
        u32 index_offset;
        u32 vertex_offset;

        SSubMesh(u32 index_count, u32 index_offset, u32 vertex_offset) : index_count(index_count), index_offset(index_offset), vertex_offset(vertex_offset) { }
    };

    class CMesh : public CAsset {
    private:
        SMeshData m_mesh_data;
        Math::SBounds m_bounds;

        TVector<SSubMesh> m_sub_meshes;

        TRef<CVertexArray> m_vertex_array;
    public:
        inline const SMeshData &GetMeshData() const { return m_mesh_data; }
        inline Math::SBounds GetBounds() const { return m_bounds; }
        inline const TRef<CVertexArray> GetVertexArray() const { return m_vertex_array; }
        inline const TVector<SSubMesh> GetSubMeshes() const { return m_sub_meshes; }

        static TRef<CMesh> Create(const SMeshData &mesh_data);
        static TRef<CMesh> Create(const SMeshData &mesh_data, const TVector<SSubMesh> sub_meshes);
        static TRef<CMesh> CreatePlane(float width, float height);
        static TRef<CMesh> CreateCube(float size);
        static TRef<CMesh> CreateFromFile(const TString &path);
    private:
        CMesh(const SMeshData &mesh_data, const TVector<SSubMesh> sub_meshes);

        void RecalculateBounds();

        static TRef<CMesh> LoadMesh(const TString &path);
        static void LoadSubMesh(const aiMesh *mesh, SMeshData &mesh_data, TVector<SSubMesh> &sub_meshes);
    };

}