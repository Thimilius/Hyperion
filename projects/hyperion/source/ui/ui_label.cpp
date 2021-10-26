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
    void UILabel::SetAlignment(UI::UITextAlignment alignment) {
        if (m_alignment != alignment) {
            m_alignment = alignment;

            MarkDirty();
        }
    }

    //--------------------------------------------------------------
    void UILabel::OnRebuildGeometry(MeshBuilder &mesh_builder) {
        if (m_font == nullptr || m_text == "") {
            return;
        }

        mesh_builder.Clear();

        TextMeshGenerationSettings settings;
        settings.text = m_text;
        settings.font = m_font;
        settings.alignment = m_alignment;
        settings.color = GetStyle().GetColor();
        settings.rect = GetWorldRect();
        settings.scale = GetDerivedScale();
        settings.rotation = GetDerivedRotation();

        AssetManager::Unload(GetRenderer().mesh);
        GetRenderer().mesh = TextMeshGenerator::GenerateMesh(settings, mesh_builder);
        GetRenderer().texture = m_font->GetTexture();
    }

}