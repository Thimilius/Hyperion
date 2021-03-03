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
#include <hyperion/entity/components/ui/button.hpp>
#include <hyperion/entity/components/ui/graphic.hpp>
#include <hyperion/rendering/immediate_renderer.hpp>
#include <hyperion/rendering/render_engine.hpp>

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

        Entity::CreatePrimitive(EntityPrimitive::Cube);

        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Camera);
        s_editor_camera = entity->GetComponent<Camera>();
        s_editor_camera_controller = entity->AddComponent<EditorLookAroundCameraController>();

        BuildInterface();

        EditorWorldViewGrid::Initialize();
    }

    //--------------------------------------------------------------
    void EditorWorldView::Update(float32 delta_time) {
        s_editor_camera_controller->Update(delta_time);
        
        if (Input::IsKeyDown(KeyCode::F2)) {
            bool vsync_on = RenderEngine::GetVSyncMode() != VSyncMode::DontSync;
            s_vsync_toggle->SetIsOn(!vsync_on);
        }
        if (Input::IsKeyDown(KeyCode::F3)) {
            s_grid_toggle->SetIsOn(!s_should_draw_grid);
        }
        if (Input::IsKeyDown(KeyCode::F4)) {
            s_physics_debug_toggle->SetIsOn(!s_should_draw_physics_debug);
        }
        if (Input::IsKeyDown(KeyCode::R)) {
            s_editor_camera_controller->Reset();
        }

        String text = StringUtils::Format("FPS: {} ({:.2f}ms)", Time::GetFPS(), Time::GetFrameTime());
        s_stats_text->SetText(text);
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

    //--------------------------------------------------------------
    void EditorWorldView::BuildInterface() {

        Font *text_font = FontLoader::LoadFont("data/fonts/robotomono_regular.ttf", 10, FontCharacterSet::LatinSupplement);
        Font *icon_font = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", 11, FontCharacterSet::All);
        Entity *canvas_entity = Entity::Create("Canvas");
        s_editor_canvas = canvas_entity->AddComponent<Canvas>();
        s_editor_canvas->SetScaleMode(CanvasScaleMode::ConstantPixelSize);
        {
            Entity *graphic_0_entity = Entity::CreateEmpty();
            graphic_0_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Graphic *graphic_0 = graphic_0_entity->AddComponent<Graphic>();
            graphic_0->SetColor(Color(0.137f, 0.153f, 0.161f, 1.0f));
            graphic_0->SetIsRaycastTarget(false);
            graphic_0->GetRectTransform()->SetSize(Vec2(0, 19));
            graphic_0->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::TopStretchHorizontal);

            Entity *graphic_1_entity = Entity::CreateEmpty();
            graphic_1_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
            Graphic *graphic_1 = graphic_1_entity->AddComponent<Graphic>();
            graphic_1->SetColor(Color(0.012f, 0.439f, 0.643f, 1.0f));
            graphic_1->SetIsRaycastTarget(false);
            graphic_1->GetRectTransform()->SetSize(Vec2(0, 1));
            graphic_1->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::BottomStretchHorizontal);
            graphic_1->GetRectTransform()->SetAnchoredPosition(Vec3(0.0f, -1.0f, 0.0f));

            std::function<Toggle *(const String &, bool, float32, OnIsOnChangedCallback)> make_toggle_button = [graphic_0_entity, icon_font]
            (const String &text, bool is_on, float32 x_position, OnIsOnChangedCallback callback) {
                Entity *toggle_entity = Entity::CreateEmpty();
                toggle_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Toggle *toggle = toggle_entity->AddComponent<Toggle>();
                toggle->SetIsOnChangedCallback(callback);
                toggle->GetRectTransform()->SetSize(Vec2(20, 0));
                toggle->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftStretchVertical);
                toggle->GetRectTransform()->SetAnchoredPosition(Vec3(x_position, 0.0f, 0.0f));

                Entity *toggle_text_entity = Entity::CreateEmpty();
                toggle_text_entity->GetTransform()->SetParent(toggle_entity->GetTransform());
                Text *toggle_text = toggle_text_entity->AddComponent<Text>();
                toggle_text->SetIsRaycastTarget(false);
                toggle_text->SetFont(icon_font);
                toggle_text->SetText(text);
                toggle_text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::StretchAll);

                toggle->SetTargetWidget(toggle_text);
                toggle->SetIsOn(is_on);

                return toggle;
            };

            s_vsync_toggle = make_toggle_button(u8"\uf108", RenderEngine::GetVSyncMode() != VSyncMode::DontSync, 0.0f, [](bool is_on) { RenderEngine::SetVSyncMode(is_on ? VSyncMode::EveryVBlank : VSyncMode::DontSync); });
            s_grid_toggle = make_toggle_button(u8"\uf00a", s_should_draw_grid, 20.0f, [](bool is_on) { s_should_draw_grid = is_on; });
            s_physics_debug_toggle = make_toggle_button(u8"\uf5cb", s_physics_debug_toggle, 40.0f, [](bool is_on) { s_should_draw_physics_debug = is_on; });

            {
                Entity *button_entity = Entity::CreateEmpty();
                button_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Button *button = button_entity->AddComponent<Button>();
                button->SetClickCallback([]() { s_editor_camera_controller->Reset(); });
                button->GetRectTransform()->SetSize(Vec2(20, 0));
                button->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftStretchVertical);
                button->GetRectTransform()->SetAnchoredPosition(Vec3(60.0f, 0.0f, 0.0f));

                Entity *button_text_entity = Entity::CreateEmpty();
                button_text_entity->GetTransform()->SetParent(button_entity->GetTransform());
                Text *button_text = button_text_entity->AddComponent<Text>();
                button_text->SetIsRaycastTarget(false);
                button_text->SetFont(icon_font);
                button_text->SetText(u8"\uf2ea");
                button_text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::StretchAll);

                button->SetTargetWidget(button_text);
            }

            {
                Entity *text_entity = Entity::CreateEmpty();
                text_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Text *text = text_entity->AddComponent<Text>();
                text->SetFont(text_font);
                text->GetRectTransform()->SetSize(Vec2(125, 0));
                text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::RightStretchVertical);
                s_stats_text = text;
            }
        }
    }

}
