//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/text.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/utilities/text_mesh_generator.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Text::SetText(const String &text) {
        if (m_text == text) {
            return;
        }

        m_text = text;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Text::SetFont(Font *font) {
        if (m_font == font) {
            return;
        }

        m_font = font;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Text::SetTextAlignment(TextAlignment text_alignment) {
        if (m_text_alignment == text_alignment) {
            return;
        }

        m_text_alignment = text_alignment;
        SetDirty();
    }

    //--------------------------------------------------------------
    void Text::OnCreate() {
        Widget::OnCreate();

        GetWidgetRenderer()->SetMaterial(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Font));
    }

    //--------------------------------------------------------------
    void Text::OnRebuildMesh(MeshBuilder &mesh_builder) {
        Mesh *old_mesh = GetWidgetRenderer()->GetMesh();
        Object::Destroy(old_mesh);

        if (m_font == nullptr || m_text == "") {
            return;
        }

        Vec3 world_corners[4];
        GetRectTransform()->GetWorldCorners(world_corners);

        TextMeshGenerationSettings settings;
        settings.text = m_text;
        settings.font = m_font;
        settings.alignment = m_text_alignment;
        settings.color = m_color;
        settings.scale = GetCanvas()->GetScale();
        settings.rect = Rect(Vec2(world_corners[2]), Vec2(world_corners[0] - world_corners[2]));

        Mesh *new_mesh = TextMeshGenerator::GenerateMesh(settings, mesh_builder);
        GetWidgetRenderer()->SetMesh(new_mesh);
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Text)
    HYP_REFLECT_BASE(Widget)
    HYP_REFLECT_CONSTRUCTOR([]() { return new Text(); })
    HYP_REFLECT_END()

}