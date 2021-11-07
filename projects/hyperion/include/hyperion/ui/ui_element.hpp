//------------------------ Header Guard ------------------------
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

    class UIQuery final {
    public:
        template<typename T>
        static T *Query(UIElement *element, const String &name = "");
    private:
        UIQuery() = delete;
        ~UIQuery() = delete;
    };

    struct UIElementRenderer {
        Mesh *mesh = nullptr;
        
        Material *material = nullptr;

        Color color = Color::White();
        Texture *texture = nullptr;
        uint32 render_texture_attachment_index = 0;

        bool8 enable_blending = true;
    };

    struct UIElementLayout {
        bool8 ignore_layout = false;
        LayoutType layout_type = LayoutType::None;
        ChildAlignment child_alignment = ChildAlignment::TopLeft;
        Vector2 spacing = Vector2(0.0f, 0.0f);
    };

    class UIElementStyle {
    public:
        inline Visibility GetVisibility() const { return m_visibility; }
        void SetVisibility(Visibility visibility) { m_visibility = visibility; }

        inline Color GetColor() const { return m_color; }
        void SetColor(Color color);

        inline float32 GetOpacity() const { return m_opacity; }
        void SetOpacity(float32 opacity);

        inline ColorBlock &GetColorBlock() { return m_color_block; }
        inline const ColorBlock &GetColorBlock() const { return m_color_block; }
        void SetColorBlock(const ColorBlock &color_block);

        inline Shadow &GetShadow() { return m_shadow; }
        inline const Shadow &GetShadow() const { return m_shadow; }
        void SetShadow(const Shadow &shadow);
    private:
        UIElement *m_element = nullptr;

        Visibility m_visibility = Visibility::Visible;
        Color m_color = Color::White();
        float32 m_opacity = 1.0f;
        ColorBlock m_color_block;
        Shadow m_shadow;
    private:
        friend class Hyperion::UI::UIElement;
    };

    class UIElementHierarchy {
    public:
        inline UIElement *GetParent() const { return m_parent; }
        void SetParent(UIElement *parent);

        inline const Array<UIElement *> &GetChildren() const { return m_children; }
        void AddChild(UIElement *child);
        void RemoveChildren();
    private:
        UIElement *m_element = nullptr;

        UIElement *m_parent = nullptr;
        Array<UIElement *> m_children;
    private:
        friend class Hyperion::UI::UIElement;
    };

    struct UIElementState {
        bool8 is_raycast_target = false;

        bool8 is_interactable = true;
        bool8 is_highlighted = false;
        bool8 is_pressed = false;
        bool8 is_selected = false;
    };

    class UIElement : public Object {
        HYP_OBJECT(UIElement);
    public:
        UIElement();
    public:
        inline bool8 IsDirty() const { return m_is_dirty; }
        inline void MarkDirty() { m_is_dirty = true; }
        void MarkHierarchyDirty();

        inline bool8 IsRaycastTarget() const { return m_state.is_raycast_target; }
        void SetRaycastTarget(bool8 is_raycast_target) { m_state.is_raycast_target = is_raycast_target; }
        inline bool8 IsInteractable() const { return m_state.is_interactable; }
        void SetInteractable(bool8 is_interactable);
        inline bool8 IsHighlighted() const { return m_state.is_highlighted; }
        inline bool8 IsPressed() const { return m_state.is_pressed; }
        inline bool8 IsSelected() const { return m_state.is_selected; }

        inline Vector2 GetPosition() const { return m_local_position; }
        inline Vector2 GetDerivedPosition() const { return m_derived_position; }
        void SetPosition(Vector2 position);
        inline float32 GetRotation() const { return m_local_rotation; }
        inline Quaternion GetDerivedRotation() const { return m_derived_rotation; }
        void SetRotation(float32 rotation);
        inline Vector2 GetScale() const { return m_local_scale; }
        inline Vector2 GetDerivedScale() const { return m_derived_scale; }
        void SetScale(Vector2 scale);

        inline Rect GetLocalRect() const { return m_rect; }
        Rect GetWorldRect() const;
        inline Vector2 GetSize() const { return m_size; }
        void SetSize(Vector2 size);
        inline Vector2 GetPivot() const { return m_pivot; }
        void SetPivot(Vector2 pivot);
        inline Vector2 GetAnchorMin() const { return m_anchor_min; }
        void SetAnchorMin(Vector2 anchor_min);
        inline Vector2 GetAnchorMax() const { return m_anchor_max; }
        void SetAnchorMax(Vector2 anchor_max);
        inline Vector2 GetAnchorOffsetMin() const { return m_anchor_offset_min; }
        void SetAnchorOffsetMin(Vector2 anchor_offset_min);
        inline Vector2 GetAnchorOffsetMax() const { return m_anchor_offset_max; }
        void SetAnchorOffsetMax(Vector2 anchor_offset_max);

        void SetAnchorPreset(AnchorPreset preset);
        void GetLocalCorners(Vector3 corners[4]) const;
        void GetWorldCorners(Vector3 corners[4]) const;
        void TransformCorners(Vector3 corners[4]) const;

        inline UIElementRenderer &GetRenderer() { return m_renderer; }
        inline const UIElementRenderer &GetRenderer() const { return m_renderer; }
        inline UIElementLayout &GetLayout() { return m_layout; }
        inline const UIElementLayout &GetLayout() const { return m_layout; }
        inline UIElementStyle &GetStyle() { return m_style; }
        inline const UIElementStyle &GetStyle() const { return m_style; }
        inline UIElementHierarchy &GetHierarchy() { return m_hierarchy; }
        inline const UIElementHierarchy &GetHierarchy() const { return m_hierarchy; }

        virtual void OnEvent(UIEvent &event);
        virtual bool8 ContainsScreenPoint(Vector2 screen_point);

        void Rebuild(float32 ui_scale, MeshBuilder &mesh_builder);

        template<typename T> T *Q(const String &name = "") { return UIQuery::Query<T>(this, name); }
    public:
        static Vector2 ScreenPointToUISpacePoint(Vector2 screen_point);
    protected:
        SelectionState GetSelectionState() const;

        virtual void DoStateTransition(SelectionState state);

        virtual void OnRebuildLayout();
        virtual void OnRebuildGeometry(MeshBuilder &mesh_builder);

        void AddQuad(MeshBuilder &mesh_builder, Vector3 corners[4], Color color);
    private:
        void RecalculateTransform(float32 ui_scale);
    private:
        static Vector2 GetLayoutPosition(ChildAlignment child_alignment, Vector2 container_size, Vector2 layout_size);
    private:
        bool8 m_is_dirty = true;

        Vector2 m_local_position = Vector2(0.0f, 0.0f);
        float32 m_local_rotation = 0.0f;
        Vector2 m_local_scale = Vector2(1.0f, 1.0f);

        Vector3 m_derived_position;
        Quaternion m_derived_rotation;
        Vector3 m_derived_scale;
        // TODO: We should be able to just use a 3x3 matrix.
        Matrix4x4 m_transform = Matrix4x4::Identity();

        Rect m_rect = Rect(0.0f, 0.0f, 100.0f, 100.0f);
        Vector2 m_size = Vector2(100.0f, 100.0f);
        Vector2 m_pivot = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_min = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_max = Vector2(0.5f, 0.5f);
        Vector2 m_anchor_offset_min = Vector2(0.0f, 0.0f);
        Vector2 m_anchor_offset_max = Vector2(0.0f, 0.0f);

        UIElementRenderer m_renderer;
        UIElementLayout m_layout;
        UIElementStyle m_style;
        UIElementHierarchy m_hierarchy;
        UIElementState m_state;
    private:
        friend class Hyperion::UI::UIElementHierarchy;
    };

    template<typename T>
    T *UIQuery::Query(UIElement *element, const String &name) {
        if (element) {
            if (element->GetType().IsDerivedFrom(MetaRegistry::Resolve<T>())) {
                if (name != "") {
                    if (element->GetName() == name) {
                        return static_cast<T *>(element);
                    }
                } else {
                    return static_cast<T *>(element);
                }
            }

            for (UIElement *child : element->GetHierarchy().GetChildren()) {
                T *canidate = Query<T>(child, name);
                if (canidate) {
                    return canidate;
                }
            }

            return nullptr;
        }

        return nullptr;
    }

}