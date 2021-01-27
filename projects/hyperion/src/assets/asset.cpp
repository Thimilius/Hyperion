#include "hyppch.hpp"

#include "hyperion/assets/asset.hpp"

#include "hyperion/assets/font.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/io/image_loader.hpp"

namespace Hyperion {

    Asset::Asset() {
        m_resource_id = Resource::GetNextResourceId();
    }

    void AssetManager::Init() {
        ImageLoader::Init();
        FontLoader::Init();
        MeshFactory::Init();
    }

    void AssetManager::Shutdown() {
        MeshFactory::Shutdown();
        FontLoader::Shutdown();
        ImageLoader::Shutdown();
    }

}
