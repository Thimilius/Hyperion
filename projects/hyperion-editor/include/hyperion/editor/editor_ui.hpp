//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/color.hpp>
#include <hyperion/ecs/component/components/ui_components.hpp>
#include <hyperion/ui/ui_factory.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorUI final {
    public:
        static void Initialize();
        static void Update();
    private:
        EditorUI() = delete;
        ~EditorUI() = delete;
    private:
        static void UpdateStats();
    private:
        inline static Font *s_font_text;
        inline static Font *s_font_icon;

        inline static UI::UIViewComponent s_ui_view;
        inline static UI::UIElement *s_root_element;
        inline static UI::UIElement *s_header_ui_element;
        inline static UI::UIElement *s_render_runtime_ui_element;
        inline static UI::UIElement *s_render_editor_ui_element;
        inline static UI::UIButton *s_child_ui_element;
        inline static UI::UILabel *s_label_stats;
        inline static UI::UIToggle *s_toggle_vsync;
        inline static UI::UIToggle *s_toggle_grid;
        inline static UI::UIToggle *s_toggle_grounds;
    };

}