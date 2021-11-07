//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ui/ui_label.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/utilities/text_mesh_generator.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    //--------------------------------------------------------------
    UILabel::UILabel() {
        GetRenderer().material = AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font);
    }

    //--------------------------------------------------------------
    void UILabel::SetFont(Font *font) {
        if (m_font != font) {
            m_font = font;

            MarkDirty();
        }
    }

    //--------------------------------------------------------------
    void UILabel::SetText(const String &text) {
        if (m_text != text) {
            m_text = text;

            MarkDirty();
        }
    }

    //--------------------------------------------------------------
    void UILabel::SetTextAlignment(UI::TextAlignment alignment) {
        if (m_alignment != alignment) {
            m_alignment = alignment;

            MarkDirty();
        }
    }

    //--------------------------------------------------------------
    void UILabel::OnRebuildShadowGeometry(MeshBuilder &mesh_builder, Color shadow_color, Vector2 shadow_offset) {
        if (!ValidateGeometryToRebuild()) {
            return;
        }

        Rect rect = GetLocalRect();
        rect.position += shadow_offset;
        RebuildTextGeometry(mesh_builder, shadow_color, rect);
    }

    //--------------------------------------------------------------
    void UILabel::OnRebuildGeometry(MeshBuilder &mesh_builder) {
        if (!ValidateGeometryToRebuild()) {
            return;
        }

        Rect rect = GetLocalRect();
        RebuildTextGeometry(mesh_builder, GetStyle().GetColor(), rect);

        // This transformation also affects the shadow vertices that may be present.
        mesh_builder.Transform(GetTransform());

        GetRenderer().texture = m_font->GetTexture();
    }

    //--------------------------------------------------------------
    void UILabel::RebuildTextGeometry(MeshBuilder &mesh_builder, Color color, Rect rect) {
        TextMeshGenerationSettings settings;
        settings.text = m_text;
        settings.font = m_font;
        settings.alignment = m_alignment;
        settings.scale = GetScale();
        settings.color = color;
        settings.rect = rect;
        TextMeshGenerator::GenerateMesh(settings, mesh_builder);
    }

    //--------------------------------------------------------------
    bool8 UILabel::ValidateGeometryToRebuild() {
        if (m_font == nullptr) {
            GetRenderer().texture = nullptr;
            return false;
        }
        if (m_text == "") {
            return false;
        }

        return true;
    }

}