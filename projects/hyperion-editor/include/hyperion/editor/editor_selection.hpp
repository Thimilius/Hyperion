//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/ecs/entity/entity_types.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorSelection final {
    public:
        static bool8 HasSelection() { return s_selection != Entity::EMPTY; }
        static EntityId GetSelection() { return s_selection; }
        static void Select(EntityId selection) { s_selection = selection; }
    private:
        EditorSelection() = delete;
        ~EditorSelection() = delete;
    private:
        inline static EntityId s_selection = Entity::EMPTY;
    };

}