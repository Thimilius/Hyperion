#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ComponentId = uint32;

    class Component final {
    public:
        template<typename T>
        inline static ComponentId GetId() {
            static ComponentId component_id = s_component_counter;
            return component_id;
        }
    private:
        inline static ComponentId s_component_counter = 0;
    };

}