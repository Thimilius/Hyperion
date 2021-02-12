#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Asset;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ResourceId = uint64;

    class Resource final {
    private:
        Resource() = delete;
        ~Resource() = delete;
    private:
        inline static ResourceId GetNextResourceId() { return ++s_resource_id_counter; }
    private:
        inline static ResourceId s_resource_id_counter = 0;
    private:
        friend class Hyperion::Asset;
    };

}