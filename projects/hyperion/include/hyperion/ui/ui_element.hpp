#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
    class UIElement;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    struct UIElementRenderer {
        Mesh *mesh;
    };

    struct UIElementStyle {
        Color color = Color::White();
        float32 opcacity = 1.0f;
    };

    class UIElementHierarchy {
    public:
        inline UIElement *GetParent() const { return m_parent; }
        inline const Array<UIElement *> &GetChildren() const { return m_children; }
    private:
        UIElement *m_parent = nullptr;
        Array<UIElement *> m_children;
    };

    class UIElement {
    public:
        UIElement() = default;
    public:
        inline bool8 IsEnabled() const { return m_enabled; }
        inline void SetEnabled(bool8 enabled) { m_enabled = enabled; }

        UIElementRenderer GetRenderer() const { return m_renderer; }

        inline UIElementStyle &GetStyle() { return m_style; }
        inline const UIElementStyle &GeStyle() const { return m_style; }

        inline UIElementHierarchy &GetHierarchy() { return m_hierarchy; }
        inline const UIElementHierarchy &GetHierarchy() const { return m_hierarchy; }
    private:
        bool8 m_enabled = true;

        UIElementRenderer m_renderer;

        Vector2 m_position = Vector2(0.0f, 0.0f);
        float32 m_rotation = 0.0f;

        Rect m_rect = Rect(0.0f, 0.0f, 100.0f, 100.0f);
        Vector2 m_size = Vector2(100.0f, 100.0f);
        Vector2 m_pivot = Vector2(0.5f, 0.5f);

        Vector2 m_anchor_min = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_max = Vector2(0.5f, 0.5f);
        Vector3 m_anchored_position = Vector3(0.0f, 0.0f, 0.0f);

        UIElementStyle m_style;
        UIElementHierarchy m_hierarchy;
    };

}