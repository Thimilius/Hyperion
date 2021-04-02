//--------------------- Definition Include ---------------------
#include "hyperion/editor/world_view/editor_world_view.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/assets/loader/mesh_loader.hpp>
#include <hyperion/core/random.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/system/operating_system.hpp>
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
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/world_view/editor_world_view_grid.hpp"
#include "hyperion/editor/world_view/editor_world_view_camera_controller.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorWorldView::Initialize() {
        WorldManager::SetActiveWorld(WorldManager::GetEditorWorld());

        Entity *camera_entity = Entity::CreatePrimitive(EntityPrimitive::Camera);
        s_editor_camera = camera_entity->GetComponent<Camera>();
        s_editor_camera_controller = camera_entity->AddComponent<EditorLookAroundCameraController>();

        BuildInterface();

        EditorWorldViewGrid::Initialize();
    }

    //--------------------------------------------------------------
    void EditorWorldView::Update(float32 delta_time) {
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

        String text = StringUtils::Format("VSync: {} - FPS: {} ({:.2f}ms)", RenderEngine::GetVSyncMode() == VSyncMode::DontSync ? "Off" : "On", Time::GetFPS(), Time::GetFrameTime());
        s_stats_text->SetText(text);
    }

    //--------------------------------------------------------------
    void EditorWorldView::Render(IRenderDriver *render_driver) {
        RetrieveEntityUnderMouse(render_driver);

        // We want to draw a highlight of our selection.
        Entity *selected_entity = EditorSelection::GetSelectedEntity();
        if (selected_entity != nullptr) {
            MeshRenderer *mesh_renderer = selected_entity->GetComponent<MeshRenderer>();

            RasterizerState rasterizer_state;
            rasterizer_state.depth_test_enabled = false;
            rasterizer_state.polygon_mode = PolygonMode::Line;
            render_driver->SetRasterizerState(rasterizer_state);

            Material *overwrite_material = AssetManager::GetMaterialPrimitive(MaterialPrimitive::Unlit);
            overwrite_material->SetColor("u_color", s_selection_color);

            EditorApplication::GetRenderPipeline()->DrawMeshRenderer(render_driver, mesh_renderer, overwrite_material);
            rasterizer_state.depth_test_enabled = true;
            rasterizer_state.polygon_mode = PolygonMode::Fill;
            render_driver->SetRasterizerState(rasterizer_state);
        }

        if (s_should_draw_grid) {
            EditorWorldViewGrid::Render(render_driver, s_editor_camera_controller->GetTargetPosition());
        }

        if (s_should_draw_physics_debug) {
            render_driver->Clear(ClearFlags::Depth, Color::Black());

            ImmediateRenderer::Begin(MeshTopology::Lines);
            EditorApplication::GetEditingWorld()->GetPhysicsWorld()->DebugDraw();
            ImmediateRenderer::End();
        }
    }

    //--------------------------------------------------------------
    void EditorWorldView::LoadWorld() {
        String path = OperatingSystem::GetInstance()->OpenFileDialog("Load world", "");
    }

    //--------------------------------------------------------------
    void EditorWorldView::SaveWorld() {
        String path = OperatingSystem::GetInstance()->SaveFileDialog("Save world", "");
    }

    //--------------------------------------------------------------
    void EditorWorldView::RetrieveEntityUnderMouse(Rendering::IRenderDriver *render_driver) {
        Vec2Int mouse_position = Vec2Int(static_cast<int32>(Input::GetMousePosition().x), static_cast<int32>(Input::GetMousePosition().y));

        // Make sure we are in bounds.
        if (mouse_position.x < 0 || mouse_position.x >= static_cast<int32>(Display::GetWidth()) || mouse_position.y < 0 || mouse_position.y >= static_cast<int32>(Display::GetHeight())) {
            return;
        }

        if (Input::IsMouseButtonDown(MouseButtonCode::Left)) {
            ResourceId render_texture_id = EditorApplication::GetRenderPipeline()->GetRenderTexture()->GetResourceId();
            render_driver->GetRenderTextureSubData(render_texture_id, 1, RectInt(mouse_position, Vec2Int(1, 1)), &s_render_texture_entity_id_buffer, [](Vector<uint8> *data) {
                ObjectId object_id = 0;
                if (data->size() > 0) {
                    object_id = *reinterpret_cast<uint32 *>(data->data());
                }

                Object *selected_object = nullptr;
                if (object_id != 0) {
                    selected_object = ObjectManager::Get(object_id);
                }

                EditorSelection::SetSelectedObject(selected_object);
            });
        }
    }

    //--------------------------------------------------------------
    void EditorWorldView::BuildInterface() {
        Font *text_font = FontLoader::LoadFont("data/fonts/robotomono_regular.ttf", 10, FontCharacterSet::LatinSupplement);
        Font *icon_font = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", 11, FontCharacterSet::All);
        Entity *canvas_entity = Entity::Create();
        s_editor_canvas = canvas_entity->AddComponent<Canvas>();
        s_editor_canvas->SetScaleMode(CanvasScaleMode::ConstantPixelSize);
        {
            Entity *graphic_0_entity = Entity::Create();
            graphic_0_entity->GetTransform()->SetParent(canvas_entity->GetTransform());
            Graphic *graphic_0 = graphic_0_entity->AddComponent<Graphic>();
            graphic_0->SetColor(Color(0.137f, 0.153f, 0.161f, 1.0f));
            graphic_0->SetIsRaycastTarget(false);
            graphic_0->GetRectTransform()->SetSize(Vec2(0, 19));
            graphic_0->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::TopStretchHorizontal);

            Entity *graphic_1_entity = Entity::Create();
            graphic_1_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
            Graphic *graphic_1 = graphic_1_entity->AddComponent<Graphic>();
            graphic_1->SetColor(Color(0.012f, 0.439f, 0.643f, 1.0f));
            graphic_1->SetIsRaycastTarget(false);
            graphic_1->GetRectTransform()->SetSize(Vec2(0, 1));
            graphic_1->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::BottomStretchHorizontal);
            graphic_1->GetRectTransform()->SetAnchoredPosition(Vec3(0.0f, -1.0f, 0.0f));

            std::function<Button *(const String &, Font *, float32, OnClickCallback)> make_button = [graphic_0_entity]
            (const String &text, Font *font, float32 x_position, OnClickCallback callback) {
                Entity *button_entity = Entity::Create();
                button_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Button *button = button_entity->AddComponent<Button>();
                button->SetClickCallback(callback);
                button->GetRectTransform()->SetSize(Vec2(20, 0));
                button->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftStretchVertical);
                button->GetRectTransform()->SetAnchoredPosition(Vec3(x_position, 0.0f, 0.0f));

                Entity *button_text_entity = Entity::Create();
                button_text_entity->GetTransform()->SetParent(button_entity->GetTransform());
                Text *button_text = button_text_entity->AddComponent<Text>();
                button_text->SetIsRaycastTarget(false);
                button_text->SetFont(font);
                button_text->SetText(text);
                button_text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::StretchAll);

                button->SetTargetWidget(button_text);

                return button;
            };

            make_button(u8"\uf07c", icon_font, 0.0f, LoadWorld);
            make_button(u8"\uf0c7", icon_font, 20.0f, SaveWorld);
            make_button(u8"|", text_font, 40.0f, []() { });

            std::function<Toggle *(const String &, bool, float32, OnIsOnChangedCallback)> make_toggle_button = [graphic_0_entity, icon_font]
            (const String &text, bool is_on, float32 x_position, OnIsOnChangedCallback callback) {
                Entity *toggle_entity = Entity::Create();
                toggle_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Toggle *toggle = toggle_entity->AddComponent<Toggle>();
                toggle->SetIsOnChangedCallback(callback);
                toggle->GetRectTransform()->SetSize(Vec2(20, 0));
                toggle->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::LeftStretchVertical);
                toggle->GetRectTransform()->SetAnchoredPosition(Vec3(x_position, 0.0f, 0.0f));

                Entity *toggle_text_entity = Entity::Create();
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

            s_vsync_toggle = make_toggle_button(u8"\uf108", RenderEngine::GetVSyncMode() != VSyncMode::DontSync, 60.0f, [](bool is_on) { RenderEngine::SetVSyncMode(is_on ? VSyncMode::EveryVBlank : VSyncMode::DontSync); });
            s_grid_toggle = make_toggle_button(u8"\uf00a", s_should_draw_grid, 80.0f, [](bool is_on) { s_should_draw_grid = is_on; });
            s_physics_debug_toggle = make_toggle_button(u8"\uf5cb", s_physics_debug_toggle, 100.0f, [](bool is_on) { s_should_draw_physics_debug = is_on; });

            {
                Entity *text_entity = Entity::Create();
                text_entity->GetTransform()->SetParent(graphic_0_entity->GetTransform());
                Text *text = text_entity->AddComponent<Text>();
                text->SetFont(text_font);
                text->SetTextAlignment(TextAlignment::MiddleRight);
                text->GetRectTransform()->SetAnchoringPreset(AnchoringPreset::RightStretchVertical);
                s_stats_text = text;
            }
        }
    }

}
