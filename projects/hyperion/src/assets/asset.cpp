#include "hyppch.hpp"

#include "hyperion/assets/asset.hpp"

namespace Hyperion {

    Asset::Asset() {
        m_resource_id = Resource::GetNextResourceId();
    }

}