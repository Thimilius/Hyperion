//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/assets/asset_manager.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/loader/image_loader.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {
    
    //--------------------------------------------------------------
    void AssetManager::Initialize() {
        ImageLoader::Initialize();
    }

    //--------------------------------------------------------------
    void AssetManager::Shutdown() {
        ImageLoader::Shutdown();
    }

}