//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/asset_system.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/ecs/component/components.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void AssetLoadSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();

        for (Asset *asset : AssetManager::s_assets_to_load) {
            render_frame->AddAssetToLoad(asset);
            asset->ResetDirty();
        }
        AssetManager::s_assets_to_load.Clear();
    }

    //--------------------------------------------------------------
    void AssetUnloadSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();

        for (Asset *asset : AssetManager::s_assets_to_unload) {
            render_frame->AddAssetToUnload(asset->GetAssetInfo().id);
        }
    }

}