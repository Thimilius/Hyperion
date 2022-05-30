//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_ui.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/assets/loader/font_loader.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/render/render_engine.hpp>
#include <hyperion/ui/immediate/ui_immediate.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_application.hpp"
#include "hyperion/editor/editor_camera.hpp"
#include "hyperion/editor/editor_selection.hpp"
#include "hyperion/editor/editor_style.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Rendering;
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  //--------------------------------------------------------------
  void EditorUI::Initialize() {
    UI::UIImmediateTheme *theme = UI::UIImmediate::GetDefaultTheme();
    theme->separator_color = EditorStyle::COLOR_HIGHLIGHT;
    theme->panel_color = EditorStyle::COLOR_NORMAL;
    theme->panel_color_hovered = EditorStyle::COLOR_NORMAL;
    theme->panel_color_pressed = EditorStyle::COLOR_NORMAL;
    theme->button_color = EditorStyle::COLOR_NORMAL_DARK;
    theme->button_color_hovered = EditorStyle::COLOR_HIGHLIGHT;
    theme->button_color_pressed = EditorStyle::COLOR_HIGHLIGHT;
    theme->toggle_normal_color = EditorStyle::COLOR_NORMAL_DARK;
    theme->toggle_normal_color_hovered = EditorStyle::COLOR_NORMAL_DARK;
    theme->toggle_normal_color_pressed = EditorStyle::COLOR_NORMAL_DARK;
    theme->toggle_toggled_color = EditorStyle::COLOR_HIGHLIGHT;
    theme->toggle_toggled_color_hovered = EditorStyle::COLOR_HIGHLIGHT;
    theme->toggle_toggled_color_pressed = EditorStyle::COLOR_HIGHLIGHT;
    theme->toggle_toggled_text_color = Color::White();
    theme->font = FontLoader::LoadFont("data/fonts/space_mono_regular.ttf", EditorStyle::FONT_SIZE, FontCharacterSet::LatinSupplement);

    UI::UIImmediateTheme *icon_theme = UI::UIImmediate::CreateTheme("Icon");
    icon_theme->font = FontLoader::LoadFont("data/fonts/font_awesome_solid.otf", EditorStyle::FONT_SIZE, FontCharacterSet::All);
  }

  //--------------------------------------------------------------
  void EditorUI::Update() {
    UIImmediateTheme *icon_theme = UIImmediate::GetTheme("Icon");
    
    UIImmediate::Begin();
    {
      Size header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
      UIImmediate::BeginPanel("Header Panel", header_panel_size);
      {
        UIImmediate::FillSpace();
        
        UIImmediate::BeginCenter();
        {
          
        }
        UIImmediate::EndCenter();
      }
      UIImmediate::EndPanel();

      UIImmediate::Separator();
      
      Size center_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
      UIImmediate::BeginPanel("Center Panel", center_panel_size);
      {
        Size left_panel_size[2] = { { SizeKind::PercentOfParent, 0.3f }, { SizeKind::AutoFill, 0.0f } };
        UIImmediate::BeginPanel("Left Panel", left_panel_size, ChildLayout::Vertical);
        {
          Size upper_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
          UIImmediate::BeginPanel("Upper Panel", upper_panel_size);
          {
            
          }
          UIImmediate::EndPanel();

          UIImmediate::Separator();
          
          Size lower_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
          UIImmediate::BeginPanel("Lower Panel", lower_panel_size);
          {
          
          }
          UIImmediate::EndPanel();
        }
        UIImmediate::EndPanel();

        UIImmediate::Separator();
        
        Size right_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
        UIImmediate::BeginPanel("Right Panel", right_panel_size, ChildLayout::Vertical);
        {
          Size preview_container_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
          UIImmediate::BeginPanel("Preview Container", preview_container_panel_size, ChildLayout::Vertical);
          {
            Size preview_header_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
            UIImmediate::BeginPanel("Preview Header", preview_header_panel_size);
            {
              bool8 is_vsync = Rendering::RenderEngine::GetVSyncMode() != Rendering::VSyncMode::DontSync;
              if (UIImmediate::TextToggle(is_vsync, "\uf108", true, icon_theme).clicked) {
                Rendering::RenderEngine::SetVSyncMode(
                  Rendering::RenderEngine::GetVSyncMode() == Rendering::VSyncMode::DontSync
                    ? Rendering::VSyncMode::EveryVBlank
                    : Rendering::VSyncMode::DontSync
                );
              }
              bool8 should_draw_grid = Rendering::RenderGizmos::GetShouldDrawGrid();
              if (UIImmediate::TextToggle(should_draw_grid, "\uf850", true, icon_theme).clicked) {
                Rendering::RenderGizmos::SetShouldDrawGrid(should_draw_grid);
              }
              bool8 should_draw_bounds = Rendering::RenderGizmos::GetShouldDrawAllBounds();
              if (UIImmediate::TextToggle(should_draw_bounds, "\uf247", true, icon_theme).clicked) {
                Rendering::RenderGizmos::SetShouldDrawAllBounds(should_draw_bounds);
              }
              if (UIImmediate::Button("\uf03d", true, icon_theme).clicked) {
                EditorCamera::Reset();
              }
              
              UIImmediate::FillSpace();

              String stats_format = "FPS: {} ({:.2f}ms)";
              String stats_text = StringUtils::Format(stats_format, Time::GetFPS(), Time::GetFrameTime());
              UIImmediate::Text(stats_text, TextAlignment::MiddleCenter, true);

              UIImmediate::Space(SizeKind::Pixels, 5.0f);
            }
            UIImmediate::EndPanel();

            UIImmediate::Separator();
            
            String image_id = "Preview Image";
            Texture *texture = EditorApplication::GetRenderPipeline()->GetEditorTargetRenderTexture();
            s_preview_element = UIImmediate::GetId(image_id);
            Size preview_panel_size[2] = { { SizeKind::AutoFill, 1.0f }, { SizeKind::AutoFill, 1.0f } };
            UIImmediate::Image(image_id, texture, preview_panel_size, false);
          }
          UIImmediate::EndPanel();

          UIImmediate::Separator();
          
          Size lower_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::PercentOfParent, 0.3f } };
          UIImmediate::BeginPanel("Lower Panel", lower_panel_size);
          {
            Size left_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Left Panel", left_panel_size);
            {
              
            }
            UIImmediate::EndPanel();

            UIImmediate::Separator();
          
            Size right_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::AutoFill, 0.0f } };
            UIImmediate::BeginPanel("Right Panel", right_panel_size);
            {
              
            }
            UIImmediate::EndPanel();
          }
          UIImmediate::EndPanel();
        }
        UIImmediate::EndPanel();
      }
      UIImmediate::EndPanel();

      UIImmediate::Separator();
      
      Size footer_panel_size[2] = { { SizeKind::AutoFill, 0.0f }, { SizeKind::Pixels, 25.0f } };
      UIImmediate::BeginPanel("Footer Panel", footer_panel_size);
      {
        
      }
      UIImmediate::EndPanel();
    }
    UIImmediate::End();
  }

  //--------------------------------------------------------------
  RectInt EditorUI::GetPreviewRect() {
    UIImmediateElement *element = UIImmediate::GetElement(s_preview_element);
    if (element == nullptr) {
      return { 0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()) };  
    } else {
      Rect rect = element->layout.rect;
      return { static_cast<int32>(rect.x), static_cast<int32>(rect.y), static_cast<int32>(rect.width), static_cast<int32>(rect.height) };
    }
  }

}
