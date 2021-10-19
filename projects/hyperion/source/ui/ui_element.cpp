//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_element.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    void UIElementStyle::SetColor(Color color) {
        m_color = color;
        m_element->MarkDirty();
    }

    //--------------------------------------------------------------
    void UIElementStyle::SetOpacity(float32 opacity) {
        m_opacity = opacity;
        m_element->MarkDirty();
    }

    //--------------------------------------------------------------
    void UIElementHierarchy::AddChild(UIElement *child) {
        m_children.Add(child);
        if (child->GetHierarchy().m_parent != nullptr) {
            child->GetHierarchy().m_parent->GetHierarchy().m_children.Remove(child);
        }
        child->GetHierarchy().m_parent = m_element;

        m_element->MarkHierarchyDirty();
    }

    //--------------------------------------------------------------
    UIElement::UIElement() {
        m_style.m_element = this;
        m_hierarchy.m_element = this;
    }

    //--------------------------------------------------------------
    void UIElement::MarkHierarchyDirty() {
        MarkDirty();

        for (UIElement *child : m_hierarchy.m_children) {
            child->MarkHierarchyDirty();
        }
    }

    //--------------------------------------------------------------
    void UIElement::GetLocalCorners(Vector3 corners[4]) const {
        HYP_ASSERT(corners);

        Vector2 min = m_rect.GetMin();
        Vector2 max = m_rect.GetMax();

        corners[0] = Vector3(max.x, max.y, 0.0f);
        corners[1] = Vector3(max.x, min.y, 0.0f);
        corners[2] = Vector3(min.x, min.y, 0.0f);
        corners[3] = Vector3(min.x, max.y, 0.0f);
    }

    //--------------------------------------------------------------
    void UIElement::GetWorldCorners(Vector3 corners[4]) const {
        HYP_ASSERT(corners);

        GetLocalCorners(corners);
        for (uint64 i = 0; i < 4; i++) {
            corners[i] = m_transform * corners[i];
        }
    }

    //--------------------------------------------------------------
    void UIElement::SetAnchorPreset(UIAnchorPreset anchor_preset) {
        switch (anchor_preset) {
            case UIAnchorPreset::TopLeft: {
                m_anchor_min = Vector2(0.0f, 1.0f);
                m_anchor_max = Vector2(0.0f, 1.0f);
                m_pivot = Vector2(0.0f, 1.0f);
                break;
            }
            case UIAnchorPreset::TopCenter: {
                m_anchor_min = Vector2(0.5f, 1.0f);
                m_anchor_max = Vector2(0.5f, 1.0f);
                m_pivot = Vector2(0.5f, 1.0f);
                break;
            }
            case UIAnchorPreset::TopRight: {
                m_anchor_min = Vector2(1.0f, 1.0f);
                m_anchor_max = Vector2(1.0f, 1.0f);
                m_pivot = Vector2(1.0f, 1.0f);
                break;
            }
            case UIAnchorPreset::MiddleLeft: {
                m_anchor_min = Vector2(0.0f, 0.5f);
                m_anchor_max = Vector2(0.0f, 0.5f);
                m_pivot = Vector2(0.0f, 0.5f);
                break;
            }
            case UIAnchorPreset::MiddleCenter: {
                m_anchor_min = Vector2(0.5f, 0.5f);
                m_anchor_max = Vector2(0.5f, 0.5f);
                m_pivot = Vector2(0.5f, 0.5f);
                break;
            }
            case UIAnchorPreset::MiddleRight: {
                m_anchor_min = Vector2(1.0f, 0.5f);
                m_anchor_max = Vector2(1.0f, 0.5f);
                m_pivot = Vector2(1.0f, 0.5f);
                break;
            }
            case UIAnchorPreset::BottomLeft: {
                m_anchor_min = Vector2(0.0f, 0.0f);
                m_anchor_max = Vector2(0.0f, 0.0f);
                m_pivot = Vector2(0.0f, 0.0f);
                break;
            }
            case UIAnchorPreset::BottomCenter: {
                m_anchor_min = Vector2(0.5f, 0.0f);
                m_anchor_max = Vector2(0.5f, 0.0f);
                m_pivot = Vector2(0.5f, 0.0f);
                break;
            }
            case UIAnchorPreset::BottomRight: {
                m_anchor_min = Vector2(1.0f, 0.0f);
                m_anchor_max = Vector2(1.0f, 0.0f);
                m_pivot = Vector2(1.0f, 0.0f);
                break;
            }
            case UIAnchorPreset::TopStretchHorizontal: {
                m_anchor_min = Vector2(0.0f, 1.0f);
                m_anchor_max = Vector2(1.0f, 1.0f);
                m_pivot = Vector2(0.5f, 1.0f);
                m_size.x = 0.0f;
                break;
            }
            case UIAnchorPreset::MiddleStretchHorizontal: {
                m_anchor_min = Vector2(0.0f, 0.5f);
                m_anchor_max = Vector2(1.0f, 0.5f);
                m_pivot = Vector2(0.5f, 0.5f);
                m_size.x = 0.0f;
                break;
            }
            case UIAnchorPreset::BottomStretchHorizontal: {
                m_anchor_min = Vector2(0.0f, 0.0f);
                m_anchor_max = Vector2(1.0f, 0.0f);
                m_pivot = Vector2(0.5f, 0.0f);
                m_size.x = 0.0f;
                break;
            }
            case UIAnchorPreset::LeftStretchVertical: {
                m_anchor_min = Vector2(0.0f, 0.0f);
                m_anchor_max = Vector2(0.0f, 1.0f);
                m_pivot = Vector2(0.0f, 0.5f);
                m_size.y = 0.0f;
                break;
            }
            case UIAnchorPreset::CenterStretchVertical: {
                m_anchor_min = Vector2(0.5f, 0.0f);
                m_anchor_max = Vector2(0.5f, 1.0f);
                m_pivot = Vector2(0.5f, 0.5f);
                m_size.y = 0.0f;
                break;
            }
            case UIAnchorPreset::RightStretchVertical: {
                m_anchor_min = Vector2(1.0f, 0.0f);
                m_anchor_max = Vector2(1.0f, 1.0f);
                m_pivot = Vector2(1.0f, 0.5f);
                m_size.y = 0.0f;
                break;
            }
            case UIAnchorPreset::StretchAll: {
                m_anchor_min = Vector2(0.0f, 0.0f);
                m_anchor_max = Vector2(1.0f, 1.0f);
                m_pivot = Vector2(0.5f, 0.5f);
                m_size = Vector2(0.0f, 0.0f);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        MarkHierarchyDirty();
    }

    //--------------------------------------------------------------
    bool8 UIElement::ContainsScreenPoint(Vector2 screen_point) {
        // We have to transform the screen point to have the origin in the middle of the screen.
        float32 display_width = static_cast<float32>(Display::GetWidth());
        float32 display_height = static_cast<float32>(Display::GetHeight());
        screen_point.x -= display_width / 2.0f;
        screen_point.y -= display_height / 2.0f;

        auto is_left = [](Vector2 p0, Vector2 p1, Vector2 p2) {
            return ((p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y));
        };

        Vector3 world_corners[4];
        GetWorldCorners(world_corners);
        Vector2 p1 = world_corners[0];
        Vector2 p2 = world_corners[1];
        Vector2 p3 = world_corners[2];
        Vector2 p4 = world_corners[3];

        // NOTE: Counter clockwise order of points is important.
        return (is_left(p1, p4, screen_point) >= 0.0f && is_left(p4, p3, screen_point) >= 0.0f && is_left(p3, p2, screen_point) >= 0.0f && is_left(p2, p1, screen_point) >= 0.0f);
    }

    //--------------------------------------------------------------
    void UIElement::Rebuild(float32 ui_scale, MeshBuilder &mesh_builder) {
        m_is_dirty = false;

        RecalculateTransform(ui_scale);

        OnRebuildLayout();
        OnRebuildGeometry(mesh_builder);
    }

    //--------------------------------------------------------------
    void UIElement::OnRebuildLayout() {

    }

    //--------------------------------------------------------------
    void UIElement::OnRebuildGeometry(MeshBuilder &mesh_builder) {
        mesh_builder.Clear();

        Vector3 world_corners[4];
        GetWorldCorners(world_corners);
        Color color = m_style.m_color;

        mesh_builder.AddVertex(world_corners[0], color, Vector2(1.0f, 1.0f));
        mesh_builder.AddVertex(world_corners[1], color, Vector2(1.0f, 0.0f));
        mesh_builder.AddVertex(world_corners[2], color, Vector2(0.0f, 0.0f));
        mesh_builder.AddVertex(world_corners[3], color, Vector2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        AssetManager::Unload(m_renderer.mesh);
        m_renderer.mesh = mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    void UIElement::RecalculateTransform(float32 ui_scale) {
        Vector2 size = m_size;
        UIElement *parent = m_hierarchy.GetParent();

        Vector3 final_local_position;
        {
            float32 inverse_ui_scale = 1.0f / ui_scale;
            Vector2 parent_pivot = parent ? parent->m_pivot : Vector2(0.5f, 0.5f);
            Vector2 parent_size = parent ? parent->m_rect.size : Vector2(inverse_ui_scale * Display::GetWidth(), inverse_ui_scale * Display::GetHeight());
            Vector2 half_parent_size = parent_size / 2.0f;

            float32 anchor_x = m_anchor_max.x - m_anchor_min.x;
            float32 anchor_y = m_anchor_max.y - m_anchor_min.y;
            float32 anchor_x_size = anchor_x * parent_size.x;
            float32 anchor_y_size = anchor_y * parent_size.y;
            size.x += anchor_x * parent_size.x;
            size.y += anchor_y * parent_size.y;
            m_rect = Rect(-m_pivot * size, size);

            // First we have our local position which acts as a simple offset.
            final_local_position = Vector3(m_local_position, 0.0f);
            // We have to take into account the parent pivot which has an effect on our local position.
            final_local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
            final_local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
            // Now we need to take into account our own pivot.
            final_local_position.x += m_anchor_min.x * parent_size.x - half_parent_size.x;
            final_local_position.y += m_anchor_min.y * parent_size.y - half_parent_size.y;
            // And finally our anchoring.
            final_local_position.x += m_pivot.x * anchor_x_size;
            final_local_position.y += m_pivot.y * anchor_y_size;
        }

        {
            if (parent == nullptr) {
                m_derived_position = ui_scale * final_local_position;
                m_derived_rotation = Quaternion::FromAxisAngle(Vector3::Forward(), m_local_rotation);
                m_derived_scale = Vector3(ui_scale * m_local_scale, 1.0f);
            } else {
                m_derived_position = parent->m_derived_rotation * (parent->m_derived_scale * final_local_position);
                m_derived_position += parent->m_derived_position;
                m_derived_rotation = parent->m_derived_rotation * Quaternion::FromAxisAngle(Vector3::Forward(), m_local_rotation);
                m_derived_scale = parent->m_derived_scale * Vector3(m_local_scale, 1.0f);
            }
            m_transform = Matrix4x4::TRS(m_derived_position, m_derived_rotation, m_derived_scale);
        }
    }

}