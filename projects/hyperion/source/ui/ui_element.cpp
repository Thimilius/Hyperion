//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_element.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/utilities/mesh_builder.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    void UIElementRenderer::RebuildMesh() {
        if (m_mesh == nullptr) {
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

            m_mesh = mesh_builder.CreateMesh();
        }
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
    void UIElement::RecalculateTransform() {
        Vector2 pivot = m_pivot;
        Vector2 size = m_size;

        UIElement *parent = m_hierarchy.GetParent();
        Vector2 parent_pivot = parent ? parent->m_pivot : Vector2(0.5f, 0.5f);
        Vector2 parent_size = parent ? parent->m_rect.size : Vector2(0.0f, 0.0f);
        Vector2 half_parent_size = parent_size / 2.0f;

        float32 anchor_x = m_anchor_max.x - m_anchor_min.x;
        float32 anchor_y = m_anchor_max.y - m_anchor_min.y;
        float32 anchor_x_size = anchor_x * parent_size.x;
        float32 anchor_y_size = anchor_y * parent_size.y;
        size.x += anchor_x * parent_size.x;
        size.y += anchor_y * parent_size.y;
        m_rect = Rect(-pivot * size, size);

        // First we have our anchored position which acts as a simple offset.
        Vector3 local_position = m_anchored_position;
        // We have to take into account the parent pivot which has an effect on our local position.
        local_position.x += (1.0f - parent_pivot.x) * parent_size.x - half_parent_size.x;
        local_position.y += (1.0f - parent_pivot.y) * parent_size.y - half_parent_size.y;
        // Now we need to take into account our own pivot.
        local_position.x += m_anchor_min.x * parent_size.x - half_parent_size.x;
        local_position.y += m_anchor_min.y * parent_size.y - half_parent_size.y;
        // And finally our anchoring.
        local_position.x += (m_pivot.x) * anchor_x_size;
        local_position.y += (m_pivot.y) * anchor_y_size;

        m_transform = Matrix4x4::TRS(local_position, Quaternion::FromAxisAngle(Vector3::Back(), m_rotation), Vector3::One());
    }

}