//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/asset_system.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void AssetLoadSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("AssetLoadSystem");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
        for (Asset *asset : AssetManager::s_assets_to_load) {
            render_frame_context.AddAssetToLoad(asset);
            asset->ResetDirty();
        }
        AssetManager::s_assets_to_load.Clear();
    }

    //--------------------------------------------------------------
    void AssetUnloadSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("AssetUnloadSystem");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
        for (Asset *asset : AssetManager::s_assets_to_unload) {
            render_frame_context.AddAssetToUnload(asset->GetAssetInfo().id);
        }
    }

}