//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ComponentRegistry final {
    public:
        inline static const Array<ComponentInfo> &GetComponentInfos() { return s_component_infos; }

        template<typename T>
        inline static ComponentId GetId() { return GetId(MetaRegistry::Resolve<T>()); }

        inline static ComponentId GetId(MetaType type) {
            // NOTE: This method/lookup is probably not as fast as it could be.
            // And it really should because it is pretty much THE hottest path in the engine.
            return s_component_ids.Get(type);
        }
    private:
        static void Initialize();
    private:
        inline static Map<MetaType, ComponentId> s_component_ids;
        inline static Array<ComponentInfo> s_component_infos;

        inline static ComponentId s_component_counter = 0;
    private:
        friend class Hyperion::WorldManager;
    };

}