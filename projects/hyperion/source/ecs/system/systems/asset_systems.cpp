//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/asset_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/render/frame/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void AssetLoadSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("AssetLoadSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
        for (Asset *asset : AssetManager::s_assets_to_load) {
            AssetInfo info = asset->GetAssetInfo();
            AssetId asset_id = info.id;
            bool can_move = info.data_access == AssetDataAccess::None;

            switch (asset->GetAssetType()) {
                case AssetType::Texture: {
                    HYP_PROFILE_SCOPE("AssetLoadSystem.LoadTexture");

                    Texture *texture = static_cast<Texture *>(asset);
                    switch (texture->GetDimension()) {
                        case TextureDimension::Texture2D: {
                            Texture2D *texture_2d = static_cast<Texture2D *>(texture);
                            RenderFrameContextAssetTexture2D &render_frame_context_asset_texture_2d = render_frame_context.AddTexture2DAssetToLoad();
                            render_frame_context_asset_texture_2d.id = asset_id;
                            render_frame_context_asset_texture_2d.parameters.format = texture_2d->GetFormat();
                            render_frame_context_asset_texture_2d.parameters.attributes = texture_2d->GetAttributes();
                            render_frame_context_asset_texture_2d.parameters.width = texture_2d->GetWidth();
                            render_frame_context_asset_texture_2d.parameters.height = texture_2d->GetHeight();
                            if (can_move) {
                                render_frame_context_asset_texture_2d.pixels = std::move(texture_2d->m_pixels);

                                // Get the pixels into a valid state after the move.
                                texture_2d->m_pixels.Clear();
                            } else {
                                render_frame_context_asset_texture_2d.pixels = texture_2d->GetPixels();
                            }
                            break;
                        }
                        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
                    }
                    break;
                }
                case AssetType::Material: {
                    HYP_PROFILE_SCOPE("AssetLoadSystem.LoadMaterial");

                    Material *material = static_cast<Material *>(asset);
                    RenderFrameContextAssetMaterial &render_frame_context_asset_material = render_frame_context.AddMaterialAssetToLoad();
                    render_frame_context_asset_material.id = asset_id;
                    render_frame_context_asset_material.shader_id = material->GetShader()->GetAssetInfo().id;
                    render_frame_context_asset_material.properties = material->GetProperties();
                    break;
                }
                case AssetType::Mesh: {
                    HYP_PROFILE_SCOPE("AssetLoadSystem.LoadMesh");

                    Mesh *mesh = static_cast<Mesh *>(asset);
                    RenderFrameContextAssetMesh &render_frame_context_asset_mesh = render_frame_context.AddMeshAssetToLoad();
                    render_frame_context_asset_mesh.id = asset_id;
                    render_frame_context_asset_mesh.vertex_format = mesh->GetVertexFormat();
                    render_frame_context_asset_mesh.sub_meshes = mesh->GetSubMeshes();
                    if (can_move) {
                        render_frame_context_asset_mesh.data = std::move(mesh->m_data);
                        
                        // Get the data into a valid state after the move.
                        mesh->m_data.positions.Clear();
                        mesh->m_data.normals.Clear();
                        mesh->m_data.colors.Clear();
                        mesh->m_data.texture0.Clear();
                        mesh->m_data.indices.Clear();
                    } else {
                        render_frame_context_asset_mesh.data = mesh->GetData();
                    }
                    break;
                }
                case AssetType::Shader: {
                    HYP_PROFILE_SCOPE("AssetLoadSystem.LoadShader");

                    Shader *shader = static_cast<Shader *>(asset);
                    RenderFrameContextAssetShader &render_frame_context_asset_shader = render_frame_context.AddShaderAssetToLoad();
                    render_frame_context_asset_shader.id = asset_id;
                    render_frame_context_asset_shader.data = shader->GetData();
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
            }

            asset->ResetDirty();
        }
        AssetManager::s_assets_to_load.Clear();
    }

    //--------------------------------------------------------------
    void AssetUnloadSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("AssetUnloadSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
        for (Asset *asset : AssetManager::s_assets_to_unload) {
            render_frame_context.AddAssetToUnload(asset->GetAssetInfo().id);
        }
    }

}