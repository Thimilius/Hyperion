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
#include "hyperion/render/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void AssetLoadSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("AssetLoadSystem.Run")

    RenderAssetContext &asset_context = RenderEngine::GetMainRenderFrame()->GetAssetContext();
    for (Asset *asset : AssetManager::s_assets_to_load) {
      const AssetMetadata &metadata = asset->GetMetadata();
      AssetHandle asset_handle = metadata.handle;
      bool8 can_move = metadata.data_access != AssetDataAccess::ReadAndWrite;

      switch (asset->GetAssetType()) {
        case AssetType::Texture: {
          HYP_PROFILE_SCOPE("AssetLoadSystem.LoadTexture")

          Texture *texture = static_cast<Texture *>(asset);
          switch (texture->GetDimension()) {
            case TextureDimension::Texture2D: {
              Texture2D *texture_2d = static_cast<Texture2D *>(texture);
              RenderAssetTexture2D &render_asset_texture_2d = asset_context.AddTexture2DAssetToLoad();
              render_asset_texture_2d.handle = asset_handle;
              render_asset_texture_2d.parameters.format = texture_2d->GetFormat();
              render_asset_texture_2d.parameters.attributes = texture_2d->GetAttributes();
              render_asset_texture_2d.parameters.width = texture_2d->GetWidth();
              render_asset_texture_2d.parameters.height = texture_2d->GetHeight();
              render_asset_texture_2d.mipmap_count = texture_2d->GetMipmapCount();
              if (can_move) {
                render_asset_texture_2d.pixels = std::move(texture_2d->m_pixels);

                // Get the pixels into a valid state after the move.
                texture_2d->m_pixels.Clear();
              } else {
                render_asset_texture_2d.pixels = texture_2d->GetPixels();
              }
              break;
            }
            case TextureDimension::RenderTexture: {
              RenderTexture *render_texture = static_cast<RenderTexture *>(texture);
              RenderAssetRenderTexture &render_asset_render_texture = asset_context.AddRenderTextureAssetToLoad();
              render_asset_render_texture.handle = asset_handle;
              render_asset_render_texture.parameters.attachments = render_texture->GetAttachments();
              render_asset_render_texture.parameters.width = render_texture->GetWidth();
              render_asset_render_texture.parameters.height = render_texture->GetHeight();
              render_asset_render_texture.mipmap_count = render_texture->GetMipmapCount();
              break;
            }
            case TextureDimension::TextureCubemap:
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
          }
          break;
        }
        case AssetType::Material: {
          HYP_PROFILE_SCOPE("AssetLoadSystem.LoadMaterial")

          Material *material = static_cast<Material *>(asset);
          RenderAssetMaterial &render_asset_material = asset_context.AddMaterialAssetToLoad();
          render_asset_material.handle = asset_handle;
          render_asset_material.shader_handle = material->GetShader()->GetMetadata().handle;
          render_asset_material.properties = material->GetProperties();
          break;
        }
        case AssetType::Mesh: {
          HYP_PROFILE_SCOPE("AssetLoadSystem.LoadMesh")

          Mesh *mesh = static_cast<Mesh *>(asset);
          RenderAssetMesh &render_asset_mesh = asset_context.AddMeshAssetToLoad();
          render_asset_mesh.handle = asset_handle;
          render_asset_mesh.vertex_format = mesh->GetVertexFormat();
          render_asset_mesh.index_format = mesh->GetIndexFormat();
          render_asset_mesh.update_type = mesh->GetUpdateType();
          render_asset_mesh.sub_meshes = mesh->GetSubMeshes();
          if (can_move) {
            render_asset_mesh.data = std::move(mesh->m_data);

            // Get the data into a valid state after the move.
            mesh->m_data.positions.Clear();
            mesh->m_data.normals.Clear();
            mesh->m_data.colors.Clear();
            mesh->m_data.texture0.Clear();
            mesh->m_data.indices.Clear();
          } else {
            render_asset_mesh.data = mesh->GetData();
          }
          break;
        }
        case AssetType::Shader: {
          HYP_PROFILE_SCOPE("AssetLoadSystem.LoadShader")

          Shader *shader = static_cast<Shader *>(asset);
          RenderAssetShader &render_asset_shader = asset_context.AddShaderAssetToLoad();
          render_asset_shader.handle = asset_handle;
          render_asset_shader.is_valid = shader->IsValid();
          render_asset_shader.data = shader->GetData();
          break;
        }
        case AssetType::None:
        case AssetType::Font:
        case AssetType::TextureAtlas: break;
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }

      asset->ResetDirty();
    }
    AssetManager::s_assets_to_load.Clear();
  }

  //--------------------------------------------------------------
  void AssetUnloadSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("AssetUnloadSystem.Run")

    RenderAssetContext &asset_context = RenderEngine::GetMainRenderFrame()->GetAssetContext();
    for (Asset *asset : AssetManager::s_assets_to_unload) {
      AssetHandle asset_handle = asset->GetMetadata().handle;

      switch (asset->GetAssetType()) {
        case AssetType::Texture: {
          Texture *texture = static_cast<Texture *>(asset);
          switch (texture->GetDimension()) {
            case TextureDimension::Texture2D: {
              asset_context.AddTexture2DToUnload(asset_handle);
              break;
            }
            case TextureDimension::RenderTexture: {
              asset_context.AddRenderTextureToUnload(asset_handle);
              break;
            }
            case TextureDimension::TextureCubemap:
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
          }
          break;
        }
        case AssetType::Material: {
          asset_context.AddMaterialToUnload(asset_handle);
          break;
        }
        case AssetType::Mesh: {
          asset_context.AddMeshToUnload(asset_handle);
          break;
        }
        case AssetType::Shader: {
          asset_context.AddShaderToUnload(asset_handle);
          break;
        }
        case AssetType::None:
        case AssetType::Font:
        case AssetType::TextureAtlas: break;
        default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
      }
    }
  }

}
