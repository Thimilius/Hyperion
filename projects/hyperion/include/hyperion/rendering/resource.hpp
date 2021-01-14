#pragma once

#include "hyperion/core/resource_id.hpp"

namespace Hyperion {
    class Asset;
}

namespace Hyperion::Rendering {

    class Resource final {
    private:
        inline static ResourceId GetNextResourceId() { return s_resource_id_counter++; }
    private:
        inline static ResourceId s_resource_id_counter = 0;
    private:
        friend class Hyperion::Asset;
    };

}