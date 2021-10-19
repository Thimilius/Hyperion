#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
    class UIElement;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    struct UIElementStyle {
        Color color = Color::White();
        float32 opcacity = 1.0f;
    };
    
    class UIElementRenderer {
    public:
        inline Mesh *GetMesh() const { return m_mesh; }

        void RebuildMesh();
    private:
        UIElement *m_element = nullptr;

        Mesh *m_mesh = nullptr;
    private:
        friend class Hyperion::UI::UIElement;
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
        UIElement();
    public:
        inline bool8 IsEnabled() const { return m_enabled; }
        inline void SetEnabled(bool8 enabled) { m_enabled = enabled; }

        inline UIElementRenderer &GetRenderer() { return m_renderer; }
        inline const UIElementRenderer &GetRenderer() const { return m_renderer; }

        inline UIElementStyle &GetStyle() { return m_style; }
        inline const UIElementStyle &GeStyle() const { return m_style; }

        inline UIElementHierarchy &GetHierarchy() { return m_hierarchy; }
        inline const UIElementHierarchy &GetHierarchy() const { return m_hierarchy; }

        void GetLocalCorners(Vector3 corners[4]) const;
        void GetWorldCorners(Vector3 corners[4]) const;

        void SetAnchorPreset(UIAnchorPreset preset);

        bool8 ContainsScreenPoint(Vector2 screen_point);
    private:
        void RecalculateTransform();
    private:
        bool8 m_enabled = true;

        UIElementRenderer m_renderer;

        Vector2 m_local_position = Vector2(0.0f, 0.0f);
        float32 m_local_rotation = 0.0f;
        Vector2 m_local_scale = Vector2(1.0f, 1.0f);

        Rect m_rect = Rect(0.0f, 0.0f, 100.0f, 100.0f);
        Vector2 m_size = Vector2(100.0f, 100.0f);
        Vector2 m_pivot = Vector2(0.5f, 0.5f);

        Vector2 m_anchor_min = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_max = Vector2(0.5f, 0.5f);
        Vector3 m_anchored_position = Vector3(0.0f, 0.0f, 0.0f);

        // TODO: We should be able to just use a 3x3 matrix.
        Matrix4x4 m_transform = Matrix4x4::Identity();

        UIElementStyle m_style;
        UIElementHierarchy m_hierarchy;
    private:
        friend class Hyperion::UI::UIElementRenderer;
    };

}