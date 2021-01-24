#include "hyppch.hpp"

#include "hyperion/assets/asset.hpp"

namespace Hyperion {

    Asset::Asset() {
        m_resource_id = Resource::GetNextResourceId();
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    Registration<Asset>("Asset")
        .property_readonly("resource_id", &Asset::m_resource_id)(metadata(Metadata::Serialize, false));
}
HYP_REFLECT_REGISTER_END