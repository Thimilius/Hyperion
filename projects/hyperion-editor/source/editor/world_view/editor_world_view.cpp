//--------------------- Definition Include ---------------------
#include "hyperion/editor/world_view/editor_world_view.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/entity/world_manager.hpp>
#include <hyperion/entity/components/rect_transform.hpp>
#include <hyperion/entity/components/physics/box_collider.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>
#include <hyperion/entity/components/rendering/widget_renderer.hpp>
#include <hyperion/entity/components/ui/text.hpp>
#include <hyperion/entity/components/ui/graphic.hpp>
#include <hyperion/rendering/immediate_renderer.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/world_view/editor_world_view_grid.hpp"
#include "hyperion/editor/world_view/editor_world_view_camera_controller.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorWorldView::Initialize() {
        s_editor_world = WorldManager::CreateWorld();
        WorldManager::SetActiveWorld(s_editor_world);

        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Camera);
        s_editor_camera = entity->GetComponent<Camera>();
        s_editor_camera_controller = entity->AddComponent<EditorLookAroundCameraController>();

        Font *font = FontLoader::LoadFont("data/fonts/robotomono_regular.ttf", 32, FontCharacterSet::LatinSupplement);
        Entity *canvas_entity = Entity::Create("Canvas");
        s_editor_canvas = canvas_entity->AddComponent<Canvas>();
        {
            Entity *graphic_0_entity = Entity::Create("Graphic_0");
            graphic_0_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Graphic *graphic_0 = graphic_0_entity->AddComponent<Graphic>();
            graphic_0->GetRectTransform()->SetSize(Vec2(0, 300));
            graphic_0->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::Center);
            graphic_0->GetRectTransform()->SetAnchorMin(Vec2(0.0f, 0.5f));
            graphic_0->GetRectTransform()->SetAnchorMax(Vec2(1.0f, 0.5f));
            graphic_0->GetRectTransform()->SetPivot(Vec2(0.0f, 0.5f));
            graphic_0->SetColor(Color::White());
            s_graphic = graphic_0;

            Entity *graphic_1_entity = Entity::Create("Graphic_1");
            graphic_1_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
            Graphic *graphic_1 = graphic_1_entity->AddComponent<Graphic>();
            graphic_1->GetRectTransform()->SetSize(Vec2(200, 200));
            graphic_1->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftCenter);
            graphic_1->SetColor(Color::Cyan());

            Entity *graphic_2_entity = Entity::Create("Graphic_2");
            graphic_2_entity->GetTransform()->SetParent(graphic_1_entity->GetTransform());
            Graphic *graphic_2 = graphic_2_entity->AddComponent<Graphic>();
            graphic_2->GetRectTransform()->SetSize(Vec2(100, 100));
            graphic_2->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::BottomRightCorner);
            graphic_2->SetColor(Color::Yellow());
        }

        EditorWorldViewGrid::Initialize();
    }

    //--------------------------------------------------------------
    void EditorWorldView::Update(float32 delta_time) {
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_should_draw_grid = !s_should_draw_grid;
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            s_should_draw_physics_debug = !s_should_draw_physics_debug;
        }

        s_editor_camera_controller->OnUpdate(delta_time);

        if (RectTransformUtility::RectangleContainsScreenPoint(s_graphic->GetRectTransform(), Input::GetMousePosition())) {
            s_graphic->SetColor(Color::Red());
        } else {
            s_graphic->SetColor(Color::White());
        }
    }

    //--------------------------------------------------------------
    void EditorWorldView::Render(IRenderDriver *render_driver) {
        if (s_should_draw_grid) {
            EditorWorldViewGrid::Render(render_driver, s_editor_camera_controller->GetTargetPosition());
        }

        if (s_should_draw_physics_debug) {
            render_driver->Clear(ClearFlags::Depth, Color::Black());

            ImmediateRenderer::Begin(MeshTopology::Lines);
            WorldManager::GetActiveWorld()->GetPhysicsWorld()->DebugDraw();
            ImmediateRenderer::End();
        }
    }

}
