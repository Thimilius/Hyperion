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
        AnchoringPreset anchoring_preset = AnchoringPreset::TopCenter;
        {
            Entity *graphic_entity = Entity::Create("Graphic");
            graphic_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Graphic *graphic = graphic_entity->AddComponent<Graphic>();
            graphic->GetRectTransform()->SetAnchoringPreset(anchoring_preset);
            graphic->GetRectTransform()->SetSize(Vec2(100, 100));
            graphic->SetColor(Color::Cyan());
        }
        String text_literal = "Hello";
        {
            Entity *text_entity = Entity::Create("Text");
            text_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Text *text = text_entity->AddComponent<Text>();
            text->GetRectTransform()->SetAnchoringPreset(anchoring_preset);
            text->GetRectTransform()->SetAnchoredPosition(Vec3(1.0f, -1.0f, 0.0f));
            text->SetFont(font);
            text->SetText(text_literal);
            text->SetColor(Color::Black());
        }
        {
            Entity *text_entity = Entity::Create("Text");
            text_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Text *text = text_entity->AddComponent<Text>();
            text->GetRectTransform()->SetAnchoringPreset(anchoring_preset);
            text->SetFont(font);
            text->SetText(text_literal);
            text->SetColor(Color::White());
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
