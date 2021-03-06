//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/font_loader.hpp"
#include "hyperion/assets/loader/mesh_loader.hpp"
#include "hyperion/assets/utilities/mesh_generator.hpp"
#include "hyperion/core/io/file_system.hpp"
#include "hyperion/core/io/image_loader.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Texture *AssetManager::GetTexturePrimitive(TexturePrimitive texture_primitive) {
        switch (texture_primitive) {
            case TexturePrimitive::Grid: return s_texture_primitive_grid;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Material *AssetManager::GetMaterialPrimitive(MaterialPrimitive material_primitive) {
        switch (material_primitive) {
            case MaterialPrimitive::Default:  return s_material_primitive_default;
            case MaterialPrimitive::Ui: return s_material_primitive_ui;
            case MaterialPrimitive::Font: return s_material_primitive_font;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    Mesh *AssetManager::GetMeshPrimitive(MeshPrimitive mesh_primitive) {
        switch (mesh_primitive) {
            case MeshPrimitive::Quad: return s_mesh_primitive_quad;
            case MeshPrimitive::Plane: return s_mesh_primitive_plane;
            case MeshPrimitive::Cube: return s_mesh_primitive_cube;
            case MeshPrimitive::Sphere: return s_mesh_primitive_sphere;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return nullptr;
        }
    }

    //--------------------------------------------------------------
    void AssetManager::Initialize() {
        ImageLoader::Initialize();
        MeshLoader::Initialize();
        FontLoader::Initialize();

        s_mesh_primitive_quad = MeshGenerator::GenerateQuad(1.0f, 1.0f);
        s_mesh_primitive_plane = MeshGenerator::GeneratePlane(10.0f, 10.0f);
        s_mesh_primitive_cube = MeshGenerator::GenerateCube(1.0f);
        s_mesh_primitive_sphere = MeshGenerator::GenerateSphere(0.5f);

        Image *image = ImageLoader::Load("data/textures/checkerboard.png");
        s_texture_primitive_grid = Texture2D::Create(image->GetWidth(), image->GetHeight(), TextureFormat::RGB24, TextureParameters(), image->GetPixels());
        Object::Destroy(image);

        Shader *default_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/standard.shader"));
        s_material_primitive_default = Material::Create(default_shader);
        s_material_primitive_default->SetVec4("u_color", Color::White());
        s_material_primitive_default->SetTexture("u_texture", s_texture_primitive_grid);

        Shader *ui_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/ui.shader"));
        s_material_primitive_ui = Material::Create(ui_shader);

        Shader *font_shader = Shader::Create(FileSystem::ReadAllText("data/shaders/font.shader"));
        s_material_primitive_font = Material::Create(font_shader);
    }

    //--------------------------------------------------------------
    void AssetManager::Update() {

    }

    //--------------------------------------------------------------
    void AssetManager::Shutdown() {
        FontLoader::Shutdown();
        MeshLoader::Shutdown();
        ImageLoader::Shutdown();
    }

}