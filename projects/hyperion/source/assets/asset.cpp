#include "hyppch.hpp"

#include "hyperion/assets/asset.hpp"

#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/io/image_loader.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Asset::Asset() {
        m_resource_id = Resource::GetNextResourceId();
    }

    Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
        switch (mesh_primitive) {
            case MeshPrimitive::Quad: return s_mesh_primitive_quad;
            case MeshPrimitive::Plane: return s_mesh_primitive_plane;
            case MeshPrimitive::Cube: return s_mesh_primitive_cube;
            case MeshPrimitive::Sphere: return s_mesh_primitive_sphere;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    Texture *AssetManager::GetTexturePrimitive(TexturePrimitive texture_primitive) {
        switch (texture_primitive) {
            case TexturePrimitive::Grid: return s_texture_primitive_grid;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    void AssetManager::Initialize() {
        ImageLoader::Initialize();
        FontLoader::Initialize();
        MeshFactory::Initialize();

        s_mesh_primitive_quad = MeshFactory::CreateQuad(1.0f, 1.0f);
        s_mesh_primitive_plane = MeshFactory::CreatePlane(10.0f, 10.0f);
        s_mesh_primitive_cube = MeshFactory::CreateCube(1.0f);
        s_mesh_primitive_sphere = MeshFactory::CreateSphere(1.0f);

        Image *image = ImageLoader::Load("data/textures/checkerboard.png");
        s_texture_primitive_grid = Texture2D::Create(image->GetWidth(), image->GetHeight(), Rendering::TextureFormat::RGB24, Rendering::TextureParameters(), image->GetPixels());
        Object::Destroy(image);

        String source = R"(
            #type vertex
            #version 410 core
                
            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec3 a_normal;
            layout(location = 4) in vec2 a_texture0;

            out V2F {
	            vec2 texture0;
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
                o_v2f.texture0 = a_texture0;

	            gl_Position = obj_to_clip_space(a_position);
            }

            #type fragment
            #version 410 core

            layout(location = 0) out vec4 o_color;

            in V2F {
	            vec2 texture0;
            } i_v2f;

            uniform vec4 u_color;
            uniform sampler2D u_texture;

            void main() {
                vec4 texture_color = texture(u_texture, i_v2f.texture0);
	            o_color = u_color * texture_color;
            }
        )";
        Shader *shader = Shader::Create(source);
        s_default_material = Material::Create(shader);
        s_default_material->SetVec4("u_color", Color::White());
        s_default_material->SetTexture("u_texture", s_texture_primitive_grid);
    }

    void AssetManager::Shutdown() {
        MeshFactory::Shutdown();
        FontLoader::Shutdown();
        ImageLoader::Shutdown();
    }

}
