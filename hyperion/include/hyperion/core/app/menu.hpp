//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct MenuItem;

  using MenuItemCallback = std::function<void()>;

  enum class MenuItemFlags {
    None,

    Separator = BIT(0),
    Disabled = BIT(1),
    Checked = BIT(2),
  };
  HYP_CREATE_ENUM_FLAG_OPERATORS(MenuItemFlags);
  
  struct MenuItem {
    String text;
    String keyboard_shortcut;
    MenuItemCallback callback;
    MenuItemFlags flags = MenuItemFlags::None;
    Array<MenuItem> sub_items;

    static MenuItem Separator() {
      return { "", "", { }, MenuItemFlags::Separator, { } };
    }
  };

  struct Menu {
    Array<MenuItem> items;
  };

}
