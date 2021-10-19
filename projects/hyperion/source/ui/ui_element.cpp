//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_element.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/utilities/mesh_builder.hpp"
#include "hyperion/core/app/display.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    void UIElementRenderer::RebuildMesh() {
        m_element->RecalculateTransform();

        MeshBuilder mesh_builder;

        Color color = Color::White();

        Vector3 world_corners[4];
        m_element->GetWorldCorners(world_corners);
        mesh_builder.AddVertex(world_corners[0], color, Vector2(1.0f, 1.0f));
        mesh_builder.AddVertex(world_corners[1], color, Vector2(1.0f, 0.0f));
        mesh_builder.AddVertex(world_corners[2], color, Vector2(0.0f, 0.0f));
        mesh_builder.AddVertex(world_corners[3], color, Vector2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        AssetManager::Unload(m_mesh);
        m_mesh = mesh_builder.CreateMesh();
    }

    //--------------------------------------------------------------
    UIElement::UIElement() {
        m_renderer.m_element = this;

        RecalculateTransform();
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

        RecalculateTransform();
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
    void UIElement::RecalculateTransform() {
        float32 display_width = static_cast<float32>(Display::GetWidth());
        float32 display_height = static_cast<float32>(Display::GetHeight());

        float32 scale = 1.0f;
        {
            float32 log_base = 2;
            float32 log_width = Math::Log(display_width / 1280) / Math::Log(log_base);
            float32 log_height = Math::Log(display_height / 720) / Math::Log(log_base);
            float32 log_weighted_average = Math::Lerp(log_width, log_height, 0.5f);
            float32 computed_scale = Math::Pow(log_base, log_weighted_average);
            scale = computed_scale;
        }
        float32 inverse_scale = 1.0f / scale;

        Vector2 pivot = m_pivot;
        Vector2 size = m_size;

        UIElement *parent = m_hierarchy.GetParent();
        Vector2 parent_pivot = parent ? parent->m_pivot : Vector2(0.5f, 0.5f);
        Vector2 parent_size = parent ? parent->m_rect.size : Vector2(inverse_scale * display_width, inverse_scale * display_height);
        Vector2 half_parent_size = parent_size / 2.0f;

        float32 anchor_x = m_anchor_max.x - m_anchor_min.x;
        float32 anchor_y = m_anchor_max.y - m_anchor_min.y;
        float32 anchor_x_size = anchor_x * parent_size.x;
        float32 anchor_y_size = anchor_y * parent_size.y;
        size.x += anchor_x * parent_size.x;
        size.y += anchor_y * parent_size.y;
        m_rect = Rect(-pivot * size, size);

        // First we have our anchored position which acts as a simple offset.
        Vector3 local_position = Vector3(m_local_position, 0.0f);
        // We have to take into account the parent pivot which has an effect on our local position.
        local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
        local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
        // Now we need to take into account our own pivot.
        local_position.x += m_anchor_min.x * parent_size.x - half_parent_size.x;
        local_position.y += m_anchor_min.y * parent_size.y - half_parent_size.y;
        // And finally our anchoring.
        local_position.x += (m_pivot.x) * anchor_x_size;
        local_position.y += (m_pivot.y) * anchor_y_size;

        Vector3 derived_position = scale * local_position;
        Quaternion derived_rotation = Quaternion::FromAxisAngle(Vector3::Forward(), m_local_rotation);
        Vector3 derived_scale = Vector3(scale * m_local_scale, 0.0f);

        m_transform = Matrix4x4::TRS(derived_position, derived_rotation, derived_scale);
    }

}