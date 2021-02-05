#include "hyperion/editor/editor_world_grid.hpp"

#include <hyperion/assets/shader.hpp>

#include "hyperion/editor/editor_world_view.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorWorldGrid::Initialize() {
        Map<ShaderStageFlags, String> sources = {
            { ShaderStageFlags::Vertex, R"(
                #version 410 core
                
                layout(location = 0) in vec3 a_position;
                layout(location = 3) in vec4 a_color;

                out V2F {
	                vec4 color;
                } o_v2f;

                uniform struct Transform {
                    mat4 model;
                    mat4 view;
                    mat4 projection;
                } u_transform;

                vec4 obj_to_clip_space(vec3 position) {
	                return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
                }

                void main() {
                    o_v2f.color = a_color;

	                gl_Position = obj_to_clip_space(a_position);
                }
            )" },
            { ShaderStageFlags::Fragment, R"(
                #version 410 core

                layout(location = 0) out vec4 o_color;

                in V2F {
	                vec4 color;
                } i_v2f;

                void main() {
	                o_color = i_v2f.color;
                }
            )" }
        };
        Shader *grid_shader = Shader::Create(sources);
        s_material = Material::Create(grid_shader);

        int32 half_grid_size = GRID_SIZE / 2;
        float32 to_point = static_cast<float32>(half_grid_size);

        // We have to remember that we are actually drawing always one more line that the actual size of the grid.
        uint32 grid_vertex_count = ((GRID_SIZE + 1) * 4);
        MeshData mesh_data;
        mesh_data.positions.resize(grid_vertex_count);
        mesh_data.colors.resize(grid_vertex_count);
        mesh_data.indices.resize(grid_vertex_count);

        uint32 index = 0;
        for (int32 x = -half_grid_size; x <= half_grid_size; x++) {
            float32 from_point = static_cast<float32>(x);
            Color color = (x % GRID_CHUNK_SIZE) == 0 ? GRID_SPECIAL_COLOR : GRID_COLOR;
            mesh_data.positions[index] = Vec3(from_point, 0, to_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
            mesh_data.positions[index] = Vec3(from_point, 0, -to_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
        }
        for (int32 z = -half_grid_size; z <= half_grid_size; z++) {
            float32 from_point = static_cast<float32>(z);
            Color color = (z % GRID_CHUNK_SIZE) == 0 ? GRID_SPECIAL_COLOR : GRID_COLOR;
            mesh_data.positions[index] = Vec3(to_point, 0, from_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
            mesh_data.positions[index] = Vec3(-to_point, 0, from_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
        }

        Vector<SubMesh> sub_meshes = { { MeshTopology::Lines, grid_vertex_count, 0, 0 } };
        s_mesh = Mesh::Create(mesh_data, sub_meshes);
    }

    void EditorWorldGrid::Render(IRenderDriver *render_driver, const CameraData &camera_data) {
        if (EditorWorldView::ShouldDrawGrid()) {
            // We want to draw the grid at the center of the camera corresponding to the grid chunk size.
            Mat4 translation = GetTranslation(camera_data);
            Mat4 rotation = GetRotation();
            render_driver->DrawMesh(s_mesh->GetResourceId(), translation * rotation, s_material->GetResourceId(), 0);
        }
    }

    Mat4 EditorWorldGrid::GetTranslation(const CameraData &camera_data) {
        Vec3 camera_position = camera_data.position;
        int32 x = static_cast<int32>(camera_position.x + (GRID_CHUNK_SIZE / 2.0f)) / GRID_CHUNK_SIZE;
        int32 y = static_cast<int32>(camera_position.y + (GRID_CHUNK_SIZE / 2.0f)) / GRID_CHUNK_SIZE;
        int32 z = static_cast<int32>(camera_position.z + (GRID_CHUNK_SIZE / 2.0f)) / GRID_CHUNK_SIZE;
        Vec3 grid_position;
        switch (s_grid_type) {
            case EditorWorldGridType::XZPlane: return Mat4::Translate(static_cast<float32>(x * GRID_CHUNK_SIZE), 0.0f, static_cast<float32>(z * GRID_CHUNK_SIZE));
            case EditorWorldGridType::XYPlane: return Mat4::Translate(static_cast<float32>(x * GRID_CHUNK_SIZE), static_cast<float32>(y * GRID_CHUNK_SIZE), 0.0f);
            case EditorWorldGridType::YZPlane: return Mat4::Translate(0.0f, static_cast<float32>(y * GRID_CHUNK_SIZE), static_cast<float32>(z * GRID_CHUNK_SIZE));
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Mat4::Identity();
        }
    }

    Mat4 EditorWorldGrid::GetRotation() {
        switch (s_grid_type) {
            case EditorWorldGridType::XZPlane: return Mat4::Identity();
            case EditorWorldGridType::XYPlane: return Mat4::Rotate(Vec3::Right(), 90.0f);
            case EditorWorldGridType::YZPlane: return Mat4::Rotate(Vec3::Forward(), 90.0f);
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Mat4::Identity();
        }
    }

}