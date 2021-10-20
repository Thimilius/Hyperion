#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/math/rect.hpp"
#include "hyperion/core/math/vector2.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/ui/ui_event.hpp"
#include "hyperion/ui/ui_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::UI {
    class UIElement;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    struct UIElementRenderer {
        Mesh *mesh = nullptr;

        Color color = Color::White();
        Texture2D *texture = nullptr;
    };

    class UIElementStyle {
    public:
        inline UIVisibility GetVisibility() const { return m_visibility; }
        void SetVisibility(UIVisibility visibility) { m_visibility = visibility; }

        inline Color GetColor() const { return m_color; }
        void SetColor(Color color);

        inline float32 GetOpacity() const { return m_opacity; }
        void SetOpacity(float32 opacity);
    private:
        UIElement *m_element = nullptr;

        UIVisibility m_visibility = UIVisibility::Visible;
        Color m_color = Color::White();
        float32 m_opacity = 1.0f;
    private:
        friend class Hyperion::UI::UIElement;
    };

    class UIElementHierarchy {
    public:
        inline UIElement *GetParent() const { return m_parent; }
        inline const Array<UIElement *> &GetChildren() const { return m_children; }

        void AddChild(UIElement *child);
    private:
        UIElement *m_element = nullptr;

        UIElement *m_parent = nullptr;
        Array<UIElement *> m_children;
    private:
        friend class Hyperion::UI::UIElement;
    };

    class UIElement {
    public:
        UIElement();
    public:
        inline bool8 IsDirty() const { return m_is_dirty; }
        inline void MarkDirty() { m_is_dirty = true; }
        void MarkHierarchyDirty();

        inline UIElementRenderer &GetRenderer() { return m_renderer; }
        inline const UIElementRenderer &GetRenderer() const { return m_renderer; }

        inline UIElementStyle &GetStyle() { return m_style; }
        inline const UIElementStyle &GeStyle() const { return m_style; }

        inline UIElementHierarchy &GetHierarchy() { return m_hierarchy; }
        inline const UIElementHierarchy &GetHierarchy() const { return m_hierarchy; }

        void GetLocalCorners(Vector3 corners[4]) const;
        void GetWorldCorners(Vector3 corners[4]) const;

        void SetAnchorPreset(UIAnchorPreset preset);

        virtual void OnEvent(UIEvent &event);
        virtual bool8 ContainsScreenPoint(Vector2 screen_point);

        void Rebuild(float32 ui_scale, MeshBuilder &mesh_builder);
    protected:
        virtual void OnRebuildLayout();
        virtual void OnRebuildGeometry(MeshBuilder &mesh_builder);
    private:
        void RecalculateTransform(float32 ui_scale);
    private:
        bool8 m_is_dirty = true;

        Vector2 m_local_position = Vector2(0.0f, 0.0f);
        float32 m_local_rotation = 0.0f;
        Vector2 m_local_scale = Vector2(1.0f, 1.0f);

        Rect m_rect = Rect(0.0f, 0.0f, 100.0f, 100.0f);
        Vector2 m_size = Vector2(100.0f, 100.0f);
        Vector2 m_pivot = Vector2(0.5f, 0.5f);

        Vector2 m_anchor_min = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_max = Vector2(0.5f, 0.5f);
        Vector3 m_anchored_position = Vector3(0.0f, 0.0f, 0.0f);

        Vector3 m_derived_position;
        Quaternion m_derived_rotation;
        Vector3 m_derived_scale;
        // TODO: We should be able to just use a 3x3 matrix.
        Matrix4x4 m_transform = Matrix4x4::Identity();

        UIElementRenderer m_renderer;
        UIElementStyle m_style;
        UIElementHierarchy m_hierarchy;
    private:
        friend class Hyperion::UI::UIElementHierarchy;
    };

}