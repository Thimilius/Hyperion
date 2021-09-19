#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class ComponentRegistry final {
    public:
        inline static const Array<ComponentInfo> &GetComponents() { return s_components; }

        template<typename T>
        inline static ComponentId GetId() {
            static ComponentId component_id = s_component_counter++;
            return component_id;
        }

        template<typename T>
        inline static void Register(const String &name) {
            ComponentInfo component_info;
            component_info.name = name;
            component_info.id = GetId<T>();
            component_info.element_size = sizeof(T);
            component_info.destructor = [](const void *component) { static_cast<const T *>(component)->~T(); };

            s_components.Add(component_info);
        }
    private:
        inline static Array<ComponentInfo> s_components;

        inline static ComponentId s_component_counter = 0;
    };

}