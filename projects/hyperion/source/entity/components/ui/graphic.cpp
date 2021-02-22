//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/ui/graphic.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/rendering/widget_renderer.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Graphic::OnCreate() {
        Widget::OnCreate();

        GetEntity()->GetComponent<WidgetRenderer>()->SetMaterial(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Ui));
    }

    //--------------------------------------------------------------
    void Graphic::OnRebuildMesh(MeshBuilder &mesh_builder) {
        WidgetRenderer *widget_renderer = GetEntity()->GetComponent<WidgetRenderer>();
        Mesh *old_mesh = widget_renderer->GetMesh();
        Object::Destroy(old_mesh);

        mesh_builder.Clear();

        Vec3 world_corners[4];
        RectTransform *rect_transform = GetEntity()->GetComponent<RectTransform>();
        rect_transform->GetWorldCorners(world_corners);
        mesh_builder.AddVertex(world_corners[0], m_color, Vec2(1.0f, 1.0f));
        mesh_builder.AddVertex(world_corners[1], m_color, Vec2(1.0f, 0.0f));
        mesh_builder.AddVertex(world_corners[2], m_color, Vec2(0.0f, 0.0f));
        mesh_builder.AddVertex(world_corners[3], m_color, Vec2(0.0f, 1.0f));
        mesh_builder.AddTriangle(0, 1, 2);
        mesh_builder.AddTriangle(0, 2, 3);

        Mesh *new_mesh = mesh_builder.CreateMesh();
        widget_renderer->SetMesh(new_mesh);
    }

    //--------------------------------------------------------------
    Graphic *Graphic::Create() {
        return new Graphic();
    }

}