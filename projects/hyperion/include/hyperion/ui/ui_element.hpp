#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UIElementHierarchy {
    private:
        UIElement *m_parent = nullptr;
        Array<UIElement *> m_children;
    };

    class UIElement {
    public:
        UIElement() = default;
    private:
        Vector2 m_position = Vector2(0.0f, 0.0f);
        float32 m_rotation = 0.0f;

        Rect m_rect = Rect(0.0f, 0.0f, 100.0f, 100.0f);
        Vector2 m_size = Vector2(100.0f, 100.0f);
        Vector2 m_pivot = Vector2(0.5f, 0.5f);

        Vector2 m_anchor_min = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_max = Vector2(0.5f, 0.5f);
        Vector3 m_anchored_position = Vector3(0.0f, 0.0f, 0.0f);

        bool m_enabled = true;

        UIElementHierarchy m_hierarchy;
    };

}