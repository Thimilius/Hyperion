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

        Font *text_font = FontLoader::LoadFont("data/fonts/robotomono_regular.ttf", 10, FontCharacterSet::LatinSupplement);
        Font *icon_font = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", 11, FontCharacterSet::All);
        Entity *canvas_entity = Entity::Create("Canvas");
        s_editor_canvas = canvas_entity->AddComponent<Canvas>();
        s_editor_canvas->SetScaleMode(CanvasScaleMode::ConstantPixelSize);
        {
            Entity *graphic_0_entity = Entity::Create("Graphic_0");
            graphic_0_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Graphic *graphic_0 = graphic_0_entity->AddComponent<Graphic>();
            graphic_0->SetColor(Color(0.137f, 0.153f, 0.161f, 1.0f));
            graphic_0->SetIsRaycastTarget(false);
            graphic_0->GetRectTransform()->SetSize(Vec2(0, 19));
            graphic_0->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::TopStretchHorizontal);

            Entity *graphic_1_entity = Entity::Create("Graphic_1");
            graphic_1_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
            Graphic *graphic_1 = graphic_1_entity->AddComponent<Graphic>();
            graphic_1->SetColor(Color(0.012f, 0.439f, 0.643f, 1.0f));
            graphic_1->SetIsRaycastTarget(false);
            graphic_1->GetRectTransform()->SetSize(Vec2(0, 1));
            graphic_1->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::BottomStretchHorizontal);
            graphic_1->GetRectTransform()->SetAnchoredPosition(Vec3(0.0f, -1.0f, 0.0f));
            
            {
                Entity *text_entity = Entity::Create("Text");
                text_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Text *text = text_entity->AddComponent<Text>();
                text->SetColor(Color::White());
                text->SetFont(text_font);
                text->GetRectTransform()->SetSize(Vec2(100, 0));
                text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::CenterStretchVertical);
                s_text = text;
            }
            {
                Entity *text_entity = Entity::Create("Text");
                text_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Text *text = text_entity->AddComponent<Text>();
                text->SetFont(icon_font);
                text->SetText(u8"\uf00a");
                text->SetColor(Color::White());
                text->GetRectTransform()->SetSize(Vec2(20, 0));
                text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftStretchVertical);
            }
            {
                Entity *text_entity = Entity::Create("Text");
                text_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Text *text = text_entity->AddComponent<Text>();
                text->SetFont(icon_font);
                text->SetText(u8"\uf5cb");
                text->SetColor(Color::White());
                text->GetRectTransform()->SetSize(Vec2(20, 0));
                text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftStretchVertical);
                text->GetRectTransform()->SetAnchoredPosition(Vec3(20.0f, 0.0f, 0.0f));
            }
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

        String text = StringUtils::Format("Hyperion - FPS: {} ({:.2f}ms)", Time::GetFPS(), Time::GetFrameTime());
        s_text->SetText(text);

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
