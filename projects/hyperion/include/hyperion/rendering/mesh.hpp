#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/asset.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/rendering/vertex_format.hpp"

struct aiMesh;

namespace Hyperion::Rendering {

    struct MeshData {
        Vector<Vec3> positions;
        Vector<Vec3> normals;
        Vector<Vec2> uvs;

        Vector<u32> indicies;
    };

    struct SubMesh {
        u32 index_count;
        u32 index_offset;
        u32 vertex_offset;

        SubMesh(u32 index_count, u32 index_offset, u32 vertex_offset) : index_count(index_count), index_offset(index_offset), vertex_offset(vertex_offset) { }
    };

    class Mesh : public Asset {
    private:
        MeshData m_mesh_data;
        Bounds m_bounds;

        Vector<SubMesh> m_sub_meshes;

        Ref<VertexArray> m_vertex_array;
    public:
        inline const MeshData &GetMeshData() const { return m_mesh_data; }
        inline Bounds GetBounds() const { return m_bounds; }
        inline const Ref<VertexArray> GetVertexArray() const { return m_vertex_array; }
        inline const Vector<SubMesh> GetSubMeshes() const { return m_sub_meshes; }

        static Ref<Mesh> Create(const MeshData &mesh_data);
        static Ref<Mesh> Create(const MeshData &mesh_data, const Vector<SubMesh> sub_meshes);
        static Ref<Mesh> CreatePlane(f32 width, f32 height);
        static Ref<Mesh> CreateCube(f32 size);
        static Ref<Mesh> CreateFromFile(const String &path);
    private:
        Mesh(const MeshData &mesh_data, const Vector<SubMesh> sub_meshes);

        void RecalculateBounds();

        static Ref<Mesh> LoadMesh(const String &path);
        static void LoadSubMesh(const aiMesh *mesh, MeshData &mesh_data, Vector<SubMesh> &sub_meshes);
    };

}